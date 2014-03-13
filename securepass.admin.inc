<?php

use Securepass\Securepass;

/**
 * @file
 * Administration page callbacks for the fbconnect module.
 */
function securepass_api_keys_settings($form, &$form_state) {
  $status = securepass_api_check_status();

  if ($status !== true) {
    $status_message = '<strong><small style="color: red;">' . $status . '</small></strong>';
  }
  else {
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

/**
 * Authentication settings
 */
function securepass_authentication_settings($form, &$form_state) {
  $active = securepass_api_ping() ?: false;

  if (!$active) {
    drupal_set_message(t('Securepass is not responding or you are not authorized, authentication temporary disabled.'), 'error');
  }

  $form['securepass_authentication'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Securepass authentication'),
    '#description' => t('Activating this option will override standard Drupal authentication using Securepass API to autenticate the users.'),
    '#default_value' => variable_get('securepass_authentication', FALSE),
    '#disabled' => !$active
  );
  return system_settings_form($form);
}
