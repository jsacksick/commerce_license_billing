<?php
/**
 * @file
 * Rules integration for the Commerce License Billing module.
 */

/**
 * Implements hook_rules_condition_info().
 */
function commerce_license_billing_rules_condition_info() {
  $conditions['commerce_license_billing_product_is_postpaid'] = array(
    'label' => t('Product is postpaid'),
    'parameter' => array(
      'commerce_product' => array(
        'type' => 'commerce_product',
        'label' => t('Product'),
      ),
    ),
    'group' => t('Commerce License Billing'),
    'callbacks' => array(
      'execute' => 'commerce_license_billing_product_postpaid_condition',
    ),
  );
  $conditions['commerce_license_billing_order_elligible'] = array(
    'label' => t('Order is elligible for recurring'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Commerce License Billing'),
    'callbacks' => array(
      'execute' => 'commerce_license_billing_order_elligible_condition',
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
 * Rules condition callback: check if the product is postpaid.
 */
function commerce_license_billing_product_postpaid_condition($product) {
  if (!empty($product->cl_billing_cycle_type)) {
    $product_wrapper = entity_metadata_wrapper('commerce_product', $product);
    if ($product_wrapper->cl_billing_cycle_type->prepaid->value() == 0) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Rules condition callback: check if an order is elligible for recurring.
 */
function commerce_license_billing_order_elligible_condition($order) {
  // Prevent recursion.
  if ($order->type == 'recurring') {
    return FALSE;
  }

  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
    if (isset($line_item_wrapper->commerce_license)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Rules action callback: create a recurring order.
 */
function commerce_license_billing_rules_create_recurring_order($order) {
  commerce_license_billing_create_recurring_order($order);
}
