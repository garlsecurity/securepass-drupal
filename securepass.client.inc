<?php

use Securepass\Securepass;
use Securepass\Exception\Exception as SecurepassException;


class SecurepassClient {
  private $client;

  public function __construct() {
    if ($config = this->getConfigurations(true)) {
      try {
        $this->client = new Securepass($config);
        return $securepass;
      }
      catch (\Exception $e) {
        $error_message = $e->getMessage();
        drupal_set_message('Securepass: ' . $error_message, 'error');
        watchdog('securepass', 'Securepass error: @error', array('@error' => $error_message), WATCHDOG_ERROR);
      }
    }

  /**
   * Check if API keys are correctly configured.
   *
   * @param boolean $verbose Print debug message
   */
  public function getConfigurations($verbose = false) {
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
   * @return
   */
  public function getSecurepassClient() {
    return $this->client;
  }

  /**
   * Ping Securepass API
   */
  public function checkStatus($verbose = false) {
    return $this->execute('ping');
  }

  /**
   * Execute
   */
  public function execute($type, $params = array()) {
    try {
      $res = $this->client($type, $params);
      return $res;
    }
    catch (SecurepassException $e) {
      $error_message = $e->getMessage();
      drupal_set_message('Securepass: ' . $error_message, 'error');
      watchdog('securepass', 'Securepass error: @error', array('@error' => $error_message), WATCHDOG_ERROR);
    }
  }
}
