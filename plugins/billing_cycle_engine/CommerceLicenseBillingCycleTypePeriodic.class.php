<?php

/**
 * A perodic billing cycle engine API.
 */
class CommerceLicenseBillingCycleTypePeriodic extends CommerceLicenseBillingCycleTypeBase {

  /**
   * Implements EntityBundlePluginProvideFieldsInterface::fields().
   */
  static function fields() {
    $fields['pce_periodicity']['field'] = array(
      'type' => 'list_text',
      'cardinality' => '1',
      'translatable' => '0',
      'settings' => array(
        'allowed_values' => array(
          'daily' => 'Daily',
          'weekly' => 'Weekly',
          'monthly' => 'Monthly',
          'quarter' => 'Quarterly',
          'yearly' => 'Yearly',
        ),
      ),
    );
    $fields['pce_periodicity']['instance'] = array(
      'label' => 'Periodicity',
      'required' => TRUE,
      'widget' => array(
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
      ),
    );
    $fields['pce_async']['field'] = array(
      'type' => 'list_boolean',
      'cardinality' => '1',
      'translatable' => '0',
      'settings' => array(
        'allowed_values' => array(
          0 => 'Synchronous',
          1 => 'Asynchronous'
        ),
      ),
    );
    $fields['pce_async']['instance'] = array(
      'label' => 'Asynchronous',
      'required' => FALSE,
      'widget' => array(
        'module' => 'options',
        'type' => 'options_onoff',
        'settings' => array(
          'display_label' => FALSE,
         ),
        'weight' => 399,
      ),
    );
    return $fields;
  }

  /**
   * Handle async billing cycle for now. Todo: sync billing cycle.
   */
  public function getBillingCycle(CommerceLicenseBillingCycle $old_billing_cycle = NULL) {
    $periodicity_mapping = array(
      'daily' => '+1 day',
      'weekly' => '+1 week',
      'monthly' => '+1 month',
      'quarter' => '+3 month',
      'yearly' => '+1 year',
    );
    $expire = strtotime($periodicity_mapping[$this->wrapper->pce_periodicity->value()], REQUEST_TIME);
    $existing_cycle = entity_load('commerce_license_billing_cycle', FALSE, array('type' => $this->{$this->nameKey}, 'expire' => $expire));
    if ($existing_cycle) {
      // Return the existing cycle.
      return reset($existing_cycle);
    }
    else {
      if ($this->wrapper->pce_async->value()) {
        // Return the next billing cycle.
        if (!empty($old_billing_cycle)) {
          $expire = strtotime($periodicity_mapping[$this->wrapper->pce_periodicity->value()], $old_billing_cycle->expire);
        }
        $title = format_date($expire, 'short') . ' - ' . ucfirst($this->wrapper->pce_periodicity->value());
        // Else, create a new one.
        $billing_cycle = entity_create('commerce_license_billing_cycle', array(
          'type' => $this->{$this->nameKey},
          'title' => $title,
          'status' => 1,
          'expire' => $expire,
        ));
        $billing_cycle->save();
        return $billing_cycle;
      }
    }
  }
}
