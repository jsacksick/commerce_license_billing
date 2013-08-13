<?php
/**
 * @file
 * Rules integration for the Commerce Usage module.
 */

/**
 * Implements hook_rules_condition_info().
 */
function commerce_usage_rules_condition_info() {
  $conditions['commerce_usage_order_elligible'] = array(
    'label' => t('This order is Usage based elligible'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Commerce Usage'),
    'callbacks' => array(
      'execute' => 'commerce_usage_order_elligible_check',
    ),
  );
  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_usage_rules_action_info() {
  $actions['commerce_usage_create_recurring_order'] = array(
    'label' => t('Create a recurring order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Commerce Usage'),
    'callbacks' => array(
      'execute' => 'commerce_usage_rules_create_recurring_order',
    ),
  );
  return $actions;
}

/**
 * Rules condition callback, check if an order is usaged based elligible.
 */
function commerce_usage_order_elligible_check($order) {
  $usage_order_types = commerce_usage_get_order_types();
  $product_types = commerce_usage_get_product_types();
  if (isset($usage_order_types[$order->type]) && !empty($product_types)) {
    // Check if at least one of the product is usaged based enabled.
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
      if (isset($product_types[$line_item_wrapper->commerce_product->type->value()])) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * Rules action callback, create a recurring order with usaged based enabled products.
 */
function commerce_usage_rules_create_recurring_order($order) {
  commerce_usage_create_recurring_order($order);
}
