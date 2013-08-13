<?php

/**
 * A test implementation of the billing cycle engine API.
 *
 * This is a cycle billing cycle engine that just create synchronous
 * billing cycles whose names are numbered starting from 0.
 */
class CommerceUsageProductUsage_Test implements CommerceUsageProductUsageInterface {
  public function getUsage($product,  $account = NULL) {
    return 1;
  }

  public function getNextUsage($product, $account = NULL) {
    return 1;
  }

  public function setUsage($product, $usage, $account = NULL) {
    $line_item = commerce_usage_get_active_product($product, $account);
    $line_item->quantity = $usage;
    $line_item->save();
  }
}
