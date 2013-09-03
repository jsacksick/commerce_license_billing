<?php

/**
 * Ensures basic required behavior for a billing cycle engine.
 *
 * EntityBundlePluginProvideFieldsInterface also mandates a fields() method.
 */
interface CommerceLicenseBillingCycleEngineInterface extends EntityBundlePluginProvideFieldsInterface {

  /**
   * Returns a billing cycle entity with provided start time.
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
  public function getBillingCycle($start = REQUEST_TIME);

  /**
   * Returns a label for the billing cycle with the provided start and end.
   *
   * @param $start
   *   The unix timestmap when the billing cycle starts.
   * @param $end
   *   The unix timestamp when the billing cycle ends.
   *
   * @return
   *   The billing cycle label.
   */
  public function getBillingCycleLabel($start, $end);

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
}
