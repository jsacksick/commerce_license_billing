<?php
/**
 * @file
 * Hooks definition.
 */

/**
 * Define product usage handlers.
 */
function hook_commerce_product_type_info() {
  return array(
    'usage' => array(
      'plugin' => '<<< Name of the product usage plugin >>>',
    )
  );
}
