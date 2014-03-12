<?php

use Securepass\Securepass;

/**
 * Check if API keys are correctly configured.
 *
 * @param boolean $verbose Print debug message
 */
function _securepass_check_api($verbose = false) {
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
 * @file
 * Administration page callbacks for the fbconnect module.
 */
function securepass_api_keys_settings($form, &$form_state) {
  $status = securepass_api_ping();
  $status_message = '<strong><small style="color: red;">' . t('Not connected') . '</small></strong>';

  if ($status) {
    $status_message = '<strong><small style="color: green;">' . t('Connected') . '</small></strong>';
  }

  $form['securepass_status'] = array(
    '#type' => 'item',
    '#title' => t('Securepass server status'),
    '#markup' => $status_message
  );

  $form['securepass_app_id'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Application ID'),
    '#default_value' => variable_get('securepass_app_id', NULL),
    '#description' => t('Securepass application id.'),
  );

  $form['securepass_app_secret'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Application Secret'),
    '#default_value' => variable_get('securepass_app_secret', NULL),
    '#description' => t('Securepass application secret.'),
  );

  return system_settings_form($form);
}


function securepass_authentication_settings($form, &$form_state) {
  $enabled = _securepass_check_api(true) ?: false;
  $form['securepass_authentication'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Securepass authentication'),
    '#description' => t('Activating this option will override standard Drupal authentication using Securepass API to autenticate the users.'),
    '#default_value' => variable_get('securepass_authentication', FALSE),
    '#disabled' => !$enabled
  );
  return system_settings_form($form);
}
