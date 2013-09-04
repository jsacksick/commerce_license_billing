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
   * Returns a billing cycle entity with the provided start time.
   *
   * If an existing billing cycle matches the expected start and end, it will
   * be returned instead.
   *
   * @param $start
   *   The unix timestamp when the billing cycle needs to start.
   *
   * @return
   *   A cl_billing_cycle entity.
   */
  public function getBillingCycle($start = REQUEST_TIME) {
    $periodicity = $this->wrapper->pce_periodicity->value();
    if (!$this->wrapper->pce_async->value()) {
      // This is a synchronous billing cycle, normalize the start timestamp.
      switch ($periodicity) {
        case 'daily':
          $start = strtotime('today');
          break;
        case 'weekly':
          $day = date('d', $start);
          $month = date('m', $start);
          $year = date('Y', $start);
          $start = strtotime('this week', mktime(0, 0, 0, $month, $day, $year));
          break;
        case 'monthly':
          $start = strtotime(date('F Y', $start));
          break;
      }
    }
    // Calculate the end timestamp.
    $periodicity_mapping = array(
      'daily' => '+1 day',
      'weekly' => '+1 week',
      'monthly' => '+1 month',
    );
    // The 1 is substracted to make sure that the billing cycle ends 1s before
    // the next one starts (January 31st 23:59:59, for instance, with the
    // next one starting on February 1st 00:00:00).
    $end = strtotime($periodicity_mapping[$periodicity], $start) - 1;

    // Try to find an existing billing cycle matching our parameters.
    $query = new EntityFieldQuery;
    $query
      ->entityCondition('entity_type', 'cl_billing_cycle')
      ->entityCondition('bundle', $this->name)
      ->propertyCondition('start', $start)
      ->propertyCondition('end', $end);
    $result = $query->execute();
    if ($result) {
      $billing_cycle_ids = array_keys($result['cl_billing_cycle']);
      $billing_cycle_id = reset($billing_cycle_ids);
      $billing_cycle = entity_load_single('cl_billing_cycle', $billing_cycle_id);
    }
    else {
      // No existing billing cycle found. Create a new one.
      $billing_cycle = entity_create('cl_billing_cycle', array('type' => $this->name));
      $billing_cycle->status = 1;
      $billing_cycle->start = $start;
      $billing_cycle->end = $end;
      $billing_cycle->save();
    }

    return $billing_cycle;
  }

  /**
   * Returns a label for a billing cycle with the provided start and end.
   *
   * @param $start
   *   The unix timestmap when the billing cycle starts.
   * @param $end
   *   The unix timestamp when the billing cycle ends.
   *
   * @return
   *   The billing cycle label.
   */
  public function getBillingCycleLabel($start, $end) {
    $async = $this->wrapper->pce_async->value();
    $periodicity = $this->wrapper->pce_periodicity->value();
    // Example: January 15th 2013
    if ($periodicity == 'daily') {
      return date('F jS Y', $end);
    }

    if ($async) {
      // Example: January 1st 2013 - January 31st 2013.
      return date('F jS Y', $start) . ' - ' . date('F jS Y', $end);
    }
    else {
      if ($periodicity == 'weekly') {
        // Example: January 1st 2013 - January 7th 2013.
        return date('F jS Y', $start) . ' - ' . date('F jS Y', $end);
      }
      elseif ($periodicity == 'monthly') {
        // Example: January 2013.
        return date('F Y');
      }
    }
  }
}
