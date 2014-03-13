<?php

use Securepass\Securepass;
use Securepass\Exception\Exception as SecurepassException;

/**
 * Check if API keys are correctly configured.
 *
 * @param boolean $verbose Print debug message
 */
function securepass_get_configurations($verbose = false) {
  $app_id = variable_get('securepass_app_id', FALSE);
  $app_secret = variable_get('securepass_app_secret', FALSE);
  if ($app_id && $app_secret) {
    $config = array('app_id'     => $app_id,
                    'app_secret' => $app_secret);
    return $config;
  }
  if ($verbose) {
    drupal_set_message(t('Securepass API Keys are not configured, you can <a href="@url"> configure api keys by clicking here</a>',
                       array('@url' => url('admin/config/system/securepass/api-keys'))), 'warning');
  }
}

/**
 * Get Securepass client
 */
function securepass_client() {
  if ($config = securepass_get_configurations()) {
    try {
      $securepass = new Securepass($config);
      return $securepass;
    }
    catch (\Exception $e) {
      securepass_api_log_error($e);
    }
  }
}

/**
 * Ping Securepass API
 */
function securepass_api_check_status() {
  try {
    securepass_api_execute('ping');
    return true;
  }
  catch (SecurepassDrupalException $e) {
    return $e->getMessage();
  }
}

function securepass_api_execute($method, $type = false, $params = array()) {
  $debug = variable_get('securepass_debug', false);
  try {
    $client = securepass_client();
    if (method_exists($client, $method)) {
      // try to understand which method has been called, some methods does't have parameters (ex. ->ping())
      $reflection = new ReflectionMethod($client, $method);
      $method_params = $reflection->getParameters();
      if (!count($method_params)) {
        $res = $client->{$method}();
      }
      else {
        $res = $client->{$method}($type, $params);
      }
      return $res;
    }
    else {
      throw new InvalidArgumentException(t('This method not exists, please refer the documentation.'));
    }
  }
  catch (Exception $e) {
    securepass_api_log_error($e, $debug);
    throw new SecurepassDrupalException($e->getMessage());
  }
}

function securepass_api_log_error($exception, $verbose = false) {
  $exception_class = get_class($exception);
  $error_message = $exception->getMessage();
  watchdog('securepass', 'Securepass API error: "@exception_class" - "@error"',
            array('@exception_class' => $exception_class,
                  '@error' => $error_message), WATCHDOG_ERROR);
  if ($verbose) {
    drupal_set_message('Securepass API error: ' . $error_message, 'error');
  }
}
