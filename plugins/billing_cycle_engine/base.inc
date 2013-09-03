<?php

/**
 * Ensures basic required behavior for a billing cycle engine.
 *
 * EntityBundlePluginProvideFieldsInterface also mandates a fields() method.
 */
interface CommerceLicenseBillingCycleEngineInterface extends EntityBundlePluginProvideFieldsInterface {

  /**
   * Returns the billing cycle.
   */
  public function getBillingCycle(CommerceLicenseBillingCycle $old_billing_cycle = NULL);

}

/**
 * Billing cycle type base class.
 */
abstract class CommerceLicenseBillingCycleTypeBase extends Entity implements CommerceLicenseBillingCycleEngineInterface {

  /**
   * The billing cycle type id.
   *
   * @var integer
   */
  public $billing_cycle_type_id;

  /**
   * The billing cycle engine (bundle).
   *
   * @var string
   */
  public $engine;

  /**
   * The machine-readable name of this billing cycle type.
   *
   * @var string
   */
  public $name;

  /**
   * The human-readable name of the billing cycle type.
   *
   * @var string
   */
  public $title;

  /**
   * Whether the generated billing cycles will be prepaid or postpaid.
   *
   * @var boolean
   */
  public $prepaid;

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
  public function __construct(array $values, $entityType = NULL) {
    parent::__construct($values, 'cl_billing_cycle_type');
    $this->wrapper = entity_metadata_wrapper($this->entityType, $this);
  }

  /**
   * Implements EntityBundlePluginProvideFieldsInterface::fields().
   */
  static function fields() {
    return array();
  }

  /**
   * Returns the billing cycle.
   */
  public function getBillingCycle(CommerceLicenseBillingCycle $old_billing_cycle = NULL) {

  }

}
