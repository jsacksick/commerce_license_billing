<?php
/**
 * @file
 * EntityAPI Controller for the Billing Cycle type entity.
 */

class CommerceLicenseBillingCycleTypeController extends EntityBundlePluginEntityControllerExportable {

  /**
   * Create a default Billing Cycle Type entity.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   * @return
   *   A cycle type object with all default fields initialized.
   */
  public function create(array $values = array()) {
    $values += array(
      'id' => NULL,
      'title' => '',
      'name' => '',
      'status' => ENTITY_IN_DB,
    );
    if (!isset($values['engine']) && $engines = commerce_license_billing_cycle_engine_plugins()) {
      $values['engine'] = key($engines);
    }
    return parent::create($values);
  }

}
