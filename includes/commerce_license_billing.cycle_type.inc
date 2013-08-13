<?php

/**
 * @file
 * The class for the billing cycle type entity.
 */
abstract class CommerceLicenseBillingCycleTypeAbstract extends Entity implements CommerceUsageBillingCycleTypeInterface {

  /**
   * Constructor.
   *
   * @see Entity::__construct()
   */
  public function __construct(array $values) {
    parent::__construct($values, 'commerce_license_billing_cycle_type');
    $this->wrapper = entity_metadata_wrapper($this->entityType, $this);
  }

}

interface CommerceLicenseBillingCycleTypeInterface {

  /**
   * Return the Billing cycle.
   */
  public function getBillingCycle(CommerceLicenseBillingCycle $old_billing_cycle = NULL);

}
