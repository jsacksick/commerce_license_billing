<?php

/**
 * Billing test license type.
 */
class CommerceLicenseBillingTest extends CommerceLicenseBase implements CommerceLicenseBillingUsageInterface  {

  /**
   * Implements CommerceLicenseBillingUsageInterface::usageGroups().
   */
  public function usageGroups() {
    return array(
      'environments' => array(
        'title' => t('Development environments'),
        'type' => 'average',
        'product' => 'BILLING-TEST-ENV',
        'immediate' => TRUE,
        'free_quantity' => 0,
      ),
      // Track MBs of storage used.
      'storage' => array(
        'title' => t('Storage'),
        'type' => 'total',
        'product' => 'BILLING-TEST-STORAGE',
        'immediate' => TRUE,
        'free_quantity' => 1024,
      ),
    );
  }

  /**
   * Implements CommerceLicenseBillingUsageInterface::usageDetails().
   */
  public function usageDetails() {
    $env_usage = commerce_license_billing_current_usage($this, 'environments');
    $storage_usage = commerce_license_billing_current_usage($this, 'storage');

    $details = t('Environments: @environments', array('@environments' => $env_usage));
    $details .= '<br />';
    $details .= t('Storage: @storage MB', array('@storage' => $storage_usage));
    return $details;
  }

  /**
   * Implements CommerceLicenseInterface::checkoutCompletionMessage().
   */
  public function checkoutCompletionMessage() {
    $text = 'Thank you for purchasing the billing test.';
    return $text;
  }
}
