<?php

/**
 * Allows a license to track its usage for billing purposes.
 */
interface CommerceLicenseBillingUsageInterface {

  /**
   * Returns the usage groups for the current license type.
   *
   * A license type can have one or multiple usage groups, used to track
   * the corresponding types of usage.
   *
   * Each group must have the following keys:
   * - title: The name of the usage group, usually used by usageDetails().
   * - type: The type of the usage group (total or average). Determines
   *   how the usage will be calculated, and subsequently charged for.
   * - product: The sku of the product representing the usage group. Linked
   *   from the line item that charges for the usage.
   * - immediate: Whether usage changes are guaranteed to be reported right
   *   away. If FALSE, the billing cycle won't be closed until the complete
   *   usage for the previous period has been reported.
   * Optional keys:
   * - free_quantity: The quantity provided for free with the license. Only
   *   usage exceeding this quantity will be charged for.
   *
   * <code>
   * return array(
   *  'bandwith' => array(
   *     'title' => t('Bandwith'),
   *     'type' => 'total',
   *     'product' => 'SKU-BANDWITH',
   *     'immediate' => FALSE,
   *     'free_quantity' => 1,
   *   ),
   *   'time' => array(
   *     'title' => t('Time'),
   *     'type' => 'total',
   *     'product' => 'SKU-TIME',
   *     'immediate' => TRUE,
   *     'free_quantity' => 0,
   *   ),
   * );
   * </code>
   */
  public function usageGroups();

  /**
   * Returns the usage details for the current license type.
   *
   * Usually implemented by iterating over usageGroups(), calculating usage
   * for each group, and outputting it to the user in a human-readable way.
   *
   * @return
   *   An html string with the usage details.
   */
  public function usageDetails();

}
