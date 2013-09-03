<?php

/**
 * The class for the billing cycle entity.
 */
class CommerceLicenseBillingCycle extends Entity {

  /**
   * The billing cycle id.
   *
   * @var integer
   */
  public $billing_cycle_id;

  /**
   * The billing cycle type (bundle).
   *
   * @var string
   */
  public $type;

  /**
   * The human-readable name of the billing cycle.
   *
   * @var string
   */
  public $title;

  /**
   * Whether the billing cycle is open or closed.
   *
   * @var boolean
   */
  public $status;

  /**
   * The date (unix timestamp) when the billing cycle expires.
   *
   * @var integer
   */
  public $expires;

  /**
   * Metadata wrapper.
   *
   * @var EntityDrupalWrapper
   */
  public $wrapper;

  /**
   * Constructor.
   *
   * @see Entity::__construct()
   */
  public function __construct(array $values) {
    parent::__construct($values, 'cl_billing_cycle');
    $this->wrapper = entity_metadata_wrapper($this->entityType, $this);
  }
}
