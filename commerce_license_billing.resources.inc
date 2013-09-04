<?php

/**
 * @file
 * Callbacks for resources.
 */

/**
 * Update the product_id of a license.
 */
function _commerce_license_billing_resource_update($license_id, $data) {
  if (!$license = entity_load_single('commerce_license', $license_id)) {
    return services_error(t('License not found.'), 404);
  }
  if (!isset($data['product_id'])) {
    return services_error(t('Missing parameter @param', array('@param' => 'product_id')), 406);
  }
  if (!$product = commerce_product_load($data['product_id'])) {
    return services_error(t('Product id not found.'), 404);
  }
  try {
    $license->product_id = $data['product_id'];
    $license->save();
  }
  catch (Exception $e) {
    return services_error($e->getMessage(), 500);
  }
}

/**
 * Revoke a license.
 */
function _commerce_license_billing_resource_delete($license_id) {
  if (!$license = entity_load_single('commerce_license', $license_id)) {
    return services_error(t('License not found.'), 404);
  }
  try {
    $license->status = COMMERCE_LICENSE_REVOKED;
    $license->save();
  }
  catch (Exception $e) {
    return services_error($e->getMessage(), 500);
  }
}

/**
 * Report the usages of a license.
 */
function _commerce_license_billing_report_usages_resource($license_id, $usages) {
  if (!$license = entity_load_single('commerce_license', $license_id)) {
    return services_error(t('License not found.'), 404);
  }
  if (!$license instanceof CommerceLicenseBillingUsageInterface) {
    return services_error(t('The license requested is not usage based.'), 406);
  }
  // Loop over the usages reported.
  $keys = array('group', 'quantity', 'start', 'end');
  foreach ($usages as $usage) {
    $usage_array = array();
    foreach ($usage as $key => $value) {
      // Skip unwanted values.
      if (!in_array($key, $keys)) {
        continue;
      }
      $usage_array[$key] = $value;
    }
    if (!isset($usage_array['group']) || !isset($usage_array['quantity'])) {
      return services_error(t("The reported usages don't match the expected format."), 406);
    }
    // Add default values.
    $usage_array += array(
      'start' => NULL,
      'end' => NULL,
    );
    try {
      // This will import the variables into the current symbol table from the
      // usage array.
      extract($usage_array);
      if (!empty($start)) {
        $start = strtotime($start);
      }
      if (!empty($end)) {
        $end = strtotime($end);
      }
      commerce_license_billing_usage_add($license, $group, $quantity, $start, $end);
    }
    catch (Exception $e) {
      return services_error($e->getMessage(), 500);
    }
  }
  drupal_add_http_header('Status', '201 Created');
}
