<?php
/**
 * @file
 * Admin UI for Billing cycle types.
 */

/**
 * UI controller.
 */
class CommerceUsageBillingCycleTypeUIController extends EntityDefaultUIController {

}

/**
 * Commerce Usage billing cycle type form.
 */
function commerce_usage_cycle_type_form($form, &$form_state, $billing_cycle_type, $op = 'edit', $entity_type = 'commerce_usage_cycle_type') {
  global $user;
  $is_new = !empty($billing_cycle_type->is_new) ? $billing_cycle_type->is_new : FALSE;

  // Add the base properties.
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => !empty($billing_cycle_type->title) ? $billing_cycle_type->title : '',
    '#maxlength' => 32,
    '#required' => TRUE,
    '#weight' => -10,
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#default_value' => !empty($billing_cycle_type->name) ? $billing_cycle_type->name : '',
    '#maxlength' => 255,
    '#machine_name' => array(
      'exists' => 'commerce_usage_name_exists',
      'source' => array('title'),
    ),
    '#disabled' => $is_new && $user->uid != 1,
    '#weight' => -9,
  );

  // Add the field related form elements.
  $form['#entity_type'] = $entity_type;
  $form_state[$entity_type] = $billing_cycle_type;

  field_attach_form($entity_type, $billing_cycle_type, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $is_new ? t('Create') : t('Save'),
    '#submit' => (!empty($form['#submit']) ? $form['#submit'] : array()) + array('commerce_usage_cycle_type_form_submit'),
  );

  return $form;
}

/**
 * Submit callback for commerce_usage_cycle_type_form().
 */
function commerce_usage_cycle_type_form_submit($form, &$form_state) {
  $entity_type = $form['#entity_type'];
  $entity = $form_state[$entity_type];

  // Notify field widgets.
  entity_form_submit_build_entity($entity_type, $entity, $form, $form_state);
  entity_save($entity_type, $entity);
  drupal_set_message(t('Billing cycle type %name saved.', array('%name' => $entity->title)));
  drupal_goto('admin/commerce/config/usage');
}
