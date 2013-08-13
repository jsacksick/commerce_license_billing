<?php
/**
 * @file
 * Rules integration for the Commerce Usage module.
 */

/**
 * Implements hook_rules_condition_info().
 */
function commerce_license_billing_condition_info() {
  $conditions['commerce_license_billing_order_elligible'] = array(
    'label' => t('This order is elligible'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Commerce License Billing'),
    'callbacks' => array(
      'execute' => 'commerce_license_billing_order_elligible_check',
    ),
  );
  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_license_billing_rules_action_info() {
  $actions['commerce_license_billing_create_recurring_order'] = array(
    'label' => t('Create a recurring order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Commerce License Billing'),
    'callbacks' => array(
      'execute' => 'commerce_license_billing_rules_create_recurring_order',
    ),
  );
  return $actions;
}

/**
 * Rules condition callback, check if an order is usaged based elligible.
 */
function commerce_license_billing_order_elligible_check($order) {
  // Check if at least one of license is usage based.
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
    // TODO: Add the instanceof check on the license.
    if (isset($line_item_wrapper->commerce_license)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Rules action callback, create a recurring order with usaged based enabled products.
 */
function commerce_license_billing_rules_create_recurring_order($order) {
  commerce_license_billing_create_recurring_order($order);
}
