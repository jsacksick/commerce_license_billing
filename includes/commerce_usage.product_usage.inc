<?php

/**
 * @file
 * The abstract class for product-usage.
 */
abstract class CommerceUsageProductUsageAbstract extends Entity implements CommerceUsageProductUsageInterface {

}

interface CommerceUsageProductUsageInterface {
  public function getUsage($product,  $account = NULL);
}
