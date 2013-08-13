<?php
/**
 * @file
 * Contains admin menu callbacks for the Commerce Usage module.
 */

/**
 * Order settings form callback.
 */
function commerce_usage_order_settings_form($form, &$form_state) {
  form_load_include($form_state, 'inc', 'commerce_usage', 'commerce_usage.admin');
  $form['help'] = array(
    '#markup' => t('This form allows you to enable the Usage Base Billing for your order types.'),
  );
  $form['commerce_usage_order_types'] = array(
    '#title' => t('Order types'),
    '#type' => 'checkboxes',
    '#default_value' => variable_get('commerce_usage_order_types', array()),
    '#options' => commerce_order_type_options_list(),
  );
  $form = system_settings_form($form);
  $form['#submit'][] = 'commerce_usage_order_settings_form_submit';
  return $form;
}

/**
 * Submit callback of the order settings form.
 */
function commerce_usage_order_settings_form_submit($form, &$form_state) {
  commerce_usage_order_bundle_rebuild();
  commerce_usage_line_item_bundle_rebuild();
}
