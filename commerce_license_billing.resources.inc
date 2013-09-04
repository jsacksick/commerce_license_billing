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
