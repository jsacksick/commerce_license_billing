<?php

/**
 * Gauge usage group.
 *
 * Prorates and charges each discrete value for each plan in a billing cycle.
 */
class CommerceLicenseBillingGaugeUsageGroup extends CommerceLicenseBillingUsageGroupBase {

  /**
   * Overrides CommerceLicenseBillingUsageBase::addUsage().
   */
  public function addUsage($revisionId, $quantity, $start, $end = 0) {
    // Close the previous usage.
    $previous_end = $start - 1;
    db_update('cl_billing_usage')
      ->fields(array(
        'end' => $previous_end,
      ))
      ->condition('license_id', $this->license->license_id)
      ->condition('revision_id', $revisionId)
      ->condition('usage_group', $this->groupName)
      ->condition('end', 0)
      ->execute();

    // Open the new usage.
    parent::addUsage($revisionId, $quantity, $start, $end);
  }

  /**
   * Implements CommerceLicenseBillingUsageGroupInterface::currentUsage().
   */
  public function currentUsage() {
    $data = array(
      ':license_id' => $this->license->license_id,
      ':group' => $this->groupName,
    );
    $usage = db_query("SELECT quantity FROM {cl_billing_usage}
                    WHERE license_id = :license_id AND usage_group = :group
                      ORDER BY start DESC, usage_id DESC LIMIT 1", $data)->fetchColumn();

    return $usage;
  }

  /**
   * Implements CommerceLicenseBillingUsageGroupInterface::chargeableUsage().
   */
  public function chargeableUsage(CommerceLicenseBillingCycle $billingCycle) {
    $usage = $this->usageHistory($billingCycle);
    $free_quantities = $this->freeQuantities($billingCycle);
    // Remove any usage that is free according to the active plan.
    foreach ($usage as $index => $usage_record) {
      $revision_id = $usage_record['revision_id'];
      if ($usage_record['quantity'] <= $free_quantities[$revision_id]) {
        unset($usage[$index]);
      }
    }

    return $usage;
  }

  /**
   * Implements CommerceLicenseBillingUsageGroupInterface::onRevisionChange().
   */
  public function onRevisionChange() {
    $previous_status = $this->license->original->status;
    $new_status = $this->license->status;
    $current_time = commerce_license_get_time();
    // The license was activated for the first time. Register initial usage.
    if ($previous_status < COMMERCE_LICENSE_ACTIVE && $new_status == COMMERCE_LICENSE_ACTIVE) {
      $initial_usage = $this->initialUsage();
      if (!is_null($initial_usage)) {
        $this->addUsage($this->license->revision_id, $initial_usage, $current_time);
      }
    }
    // A new revision was created, and the previous revision was active.
    // Close previous open usage, reopen for the new revision if still active.
    elseif ($previous_status == COMMERCE_LICENSE_ACTIVE) {
      // Get the quantities of any open usage.
      $data = array(
        ':group_name' => $this->groupName,
        ':revision_id' => $this->license->original->revision_id,
      );
      $query = db_query('SELECT quantity FROM {cl_billing_usage}
                            WHERE usage_group = :group_name
                              AND revision_id = :revision_id
                                AND end = 0', $data);
      $previous_usage = $query->fetchAssoc();

      // Close the open usage for the previous revision (plan).
      db_update('cl_billing_usage')
        ->fields(array(
          'end' => $current_time - 1,
        ))
        ->condition('revision_id', $this->license->original->revision_id)
        ->condition('end', '0')
        ->execute();

      // Reset the usage history static cache.
      drupal_static_reset('commerce_license_billing_usage_history_list');

      // If the license is still active, reopen the usage.
      if ($new_status == COMMERCE_LICENSE_ACTIVE) {
        foreach ($previous_usage as $quantity) {
          $this->addUsage($this->license->revision_id, $quantity, $current_time);
        }
      }
    }
    // A new revision has been created, unsuspending the license. Reopen
    // previous usage.
    elseif ($previous_status == COMMERCE_LICENSE_SUSPENDED && $new_status == COMMERCE_LICENSE_ACTIVE) {
      // Get the last closed usage quantity for this group.
      $data = array(
        ':group_name' => $this->groupName,
        ':license_id' => $this->license->license_id,
      );
      $query = db_query('SELECT quantity FROM {cl_billing_usage}
                            WHERE usage_group = :group_name
                              AND license_id = :license_id
                                ORDER BY end DESC, usage_id DESC LIMIT 1', $data);
      $previous_quantity = $query->fetchField();
      $this->addUsage($this->license->revision_id, $previous_quantity, $current_time);
    }
  }

  /**
   * Returns the initial usage.
   *
   * @return
   *   The initial usage to register, or NULL if none found.
   */
  protected function initialUsage() {
    // Try to get the initial usage from a hook.
    $usage_hook = 'commerce_license_billing_initial_usage';
    $initial_usage = NULL;
    foreach (module_implements($usage_hook) as $module) {
      $initial_usage = module_invoke($module, $usage_hook, $this->license, $this->groupName);
      if (!is_null($initial_usage)) {
        // Usage found, stop the search here.
        break;
      }
    }
    // If no module provided the initial usage, try looking at the group info.
    if (is_null($initial_usage) && !empty($this->groupInfo['initial_quantity'])) {
      $initial_usage = $this->groupInfo['initial_quantity'];
    }

    return $initial_usage;
  }
}
