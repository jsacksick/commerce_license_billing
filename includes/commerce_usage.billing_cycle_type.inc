<?php

/**
 * @file
 * The class for the billing cycle type entity.
 */
abstract class CommerceUsageBillingCycleTypeAbstract extends Entity implements CommerceUsageBillingCycleTypeInterface {

  /**
   * Constructor.
   *
   * @see Entity::__construct()
   */
  public function __construct(array $values) {
    parent::__construct($values, 'commerce_usage_cycle_type');
    $this->wrapper = entity_metadata_wrapper($this->entityType, $this);
  }

}

interface CommerceUsageBillingCycleTypeInterface {

  /**
   * Return the Billing cycle.
   */
  public function getBillingCycle(CommerceUsageBillingCycle $old_billing_cycle = NULL);

}
