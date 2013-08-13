<?php

/**
 * A perodic billing cycle engine API.
 */
class CommerceLicenseBillingPeriodicCycleType extends CommerceLicenseBillingCycleTypeAbstract implements EntityBundlePluginProvideFieldsInterface {

  /**
   * Implements EntityBundlePluginProvideFieldsInterface::fields().
   */
  static function fields() {
    $fields['cu_periodic_sync']['field'] = array(
      'type' => 'list_text',
      'cardinality' => '1',
      'translatable' => '0',
      'settings' => array(
        'allowed_values' => array(
          'sync' => t('Synchronous'),
          'async' => t('Asynchronous'),
        ),
      ),
    );
    $fields['cu_periodic_sync']['instance'] = array(
      'label' => 'Synchronization',
      'required' => TRUE,
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '6',
      ),
    );
    $fields['cu_periodic_periodicity']['field'] = array(
      'type' => 'list_text',
      'cardinality' => '1',
      'translatable' => '0',
      'settings' => array(
        'allowed_values' => array(
          'daily' => t('Daily'),
          'weekly' => t('Weekly'),
          'monthly' => t('Monthly'),
          'quarter' => t('Quarter'),
          'yearly' => t('Yearly'),
        ),
      ),
    );
    $fields['cu_periodic_periodicity']['instance'] = array(
      'label' => 'Periodicity',
      'required' => TRUE,
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '6',
      ),
    );
    return $fields;
  }

  /**
   * Handle async billing cycle for now. Todo: sync billing cycle.
   */
  public function getBillingCycle(CommerceLicenseBillingCycle $old_billing_cycle = NULL) {
    $request_time = REQUEST_TIME;
    $periodicity_mapping = array(
      'daily' => '+ 1 day',
      'weekly' => '+ 1 week',
      'monthly' => '+ 1 month',
      'quarter' => '+ 3 month',
      'yearly' => '+ 1 year',
    );
    $expire = strtotime($periodicity_mapping[$this->wrapper->cu_periodic_periodicity->value()], $request_time);
    $existing_cycle = entity_load('commerce_license_billing_cycle', FALSE, array('type' => $this->{$this->nameKey}, 'expire' => $expire));
    if ($existing_cycle) {
      // Return the existing cycle.
      return reset($existing_cycle);
    }
    else {
      if ($this->wrapper->cu_periodic_sync->value() == 'async') {
        // Return the next billing cycle.
        if (!empty($old_billing_cycle)) {
          $expire = strtotime($periodicity_mapping[$this->wrapper->cu_periodic_periodicity->value()], $old_billing_cycle->expire);
        }
        $title = format_date($expire, 'short') . ' - ' . ucfirst($this->wrapper->cu_periodic_periodicity->value());
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
