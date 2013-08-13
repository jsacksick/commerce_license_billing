<?php

/**
 * A test implementation of the billing cycle engine API.
 *
 * This is a cycle billing cycle engine that just create synchronous
 * billing cycles whose names are numbered starting from 0.
 */
class CommerceUsageBillingCycleType_Test extends CommerceUsageBillingCycleTypeAbstract {
  public function getBillingCycle(CommerceUsageBillingCycle $old_billing_cycle = NULL) {
    // Build a numbered title.
    $new_title = $old_billing_cycle ? $old_billing_cycle->title + 1 : '0';

    // Check if there is already a billing cycle having this title.
    $existing_id = db_select('commerce_usage_cycle', 'c')
      ->fields('c', array('id'))
      ->condition('type', $this->name)
      ->condition('title', $new_title)
      ->execute()
      ->fetchField();

    if ($existing_id) {
      // Return the existing cycle.
      return entity_load_single('commerce_usage_cycle', $existing_id);
    }
    else {
      // Else, create a new one.
      $billing_cycle = entity_create('commerce_usage_cycle', array(
        'type' => $this->name,
        'title' => $old_billing_cycle ? $old_billing_cycle->title + 1 : '0',
        'status' => 1,
        'expire' => REQUEST_TIME,
      ));
      $billing_cycle->save();
      return $billing_cycle;
    }
  }
}
