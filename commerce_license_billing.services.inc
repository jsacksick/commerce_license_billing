<?php

/**
 * @file
 * Contains services module hook implementations.
 */

/**
 * Implements hook_services_resources().
 */
function commerce_license_billing_services_resources() {
  $resources["licenses"]['delete'] = array(
    'file' => array('type' => 'inc', 'module' => 'commerce_license_billing', 'name' => 'commerce_license_billing.resources'),
    'callback' => '_commerce_license_billing_resource_delete',
    'args' => array(
      array(
        'name' => 'license_id',
        'optional' => FALSE,
        'source' => array('path' => 0),
        'type' => 'int',
        'description' => 'The license id.',
      ),
    ),
    'access arguments' => array('access content'),
    'access arguments append' => FALSE,
  );
  $resources['licenses']['targeted_actions'] = array(
    'usages' => array(
      'help' => 'Report the usages of a license.',
      'file' => array('type' => 'inc', 'module' => 'commerce_license_billing', 'name' => 'commerce_license_billing.resources'),
      'callback' => '_commerce_license_billing_report_usages_resource',
      'args' => array(
        array(
          'name' => 'license_id',
          'optional' => FALSE,
          'source' => array('path' => 0),
          'type' => 'int',
          'description' => 'The license id.',
        ),
        array(
          'name' => 'usages',
          'type' => 'struct',
          'description' => 'An array of usages.',
          'source' => 'data',
          'optional' => FALSE,
        ),
      ),
      'access arguments' => array('access content'),
      'access arguments append' => FALSE,
    ),
  );

  return $resources;
}