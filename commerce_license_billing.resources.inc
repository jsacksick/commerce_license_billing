<?php

/**
 * @file
 * Callbacks for resources.
 */

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
    return services_error(t('License revoked'), 204);
  }
  catch (Exception $e) {
    return services_error($e->getMessage(), 500);
  }
}
