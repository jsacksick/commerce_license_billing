<?php

/**
 * Billing test 2 license type.
 */
class CommerceLicenseBillingTest2 extends CommerceLicenseBase implements CommerceLicenseBillingUsageInterface  {

  /**
   * Implements CommerceLicenseBillingUsageInterface::usageGroups().
   */
  public function usageGroups() {
    // Provide a non-immediate usage group.
    return array(
     'bandwith' => array(
        'title' => t('Bandwith'),
        'type' => 'total',
        'product' => 'BILLING-TEST-BANDWITH',
        'immediate' => FALSE,
        'free_quantity' => 0,
      ),
    );
  }

  /**
   * Implements CommerceLicenseBillingUsageInterface::usageDetails().
   */
  public function usageDetails() {
    $usage = commerce_license_billing_current_usage($this, 'bandwith');
    return t('Bandwith: @bandwith MB', array('@bandwith' => $usage));
  }

  /**
   * Implements CommerceLicenseInterface::checkoutCompletionMessage().
   */
  public function checkoutCompletionMessage() {
    $text = 'Thank you for purchasing the billing test 2.';
    return $text;
  }
}
