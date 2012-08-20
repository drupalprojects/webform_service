<?php
/**
 * @file
 * webform_service.services.inc
 */

/**
 * Implements hook_default_services_endpoint().
 */
function webform_service_default_services_endpoint() {
  $export = array();

  $endpoint = new stdClass();
  $endpoint->disabled = TRUE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'rest';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'webform';
  $endpoint->authentication = array(
    'services' => 'services',
  );
  $endpoint->server_settings = array(
    'rest_server' => array(
      'formatters' => array(
        'json' => TRUE,
        'jsonp' => TRUE,
        'rss' => TRUE,
        'xml' => TRUE,
        'bencode' => FALSE,
        'php' => FALSE,
        'yaml' => FALSE,
      ),
      'parsers' => array(
        'application/json' => TRUE,
        'application/vnd.php.serialized' => TRUE,
        'application/x-www-form-urlencoded' => TRUE,
        'application/x-yaml' => TRUE,
        'multipart/form-data' => TRUE,
      ),
    ),
  );
  $endpoint->resources = array(
    'webform' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
      'relationships' => array(
        'submissions' => array(
          'enabled' => 1,
        ),
      ),
    ),// webform

    'submission' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => 1,
        ),
        'create' => array(
          'enabled' => 1,
        ),
        'update' => array(
          'enabled' => 1,
        ),
        'delete' => array(
          'enabled' => 1,
        ),
        'index' => array(
          'enabled' => 1,
        ),
      ),
    ),// submission.
  );
  $endpoint->debug = 0;
  $export['rest'] = $endpoint;

  return $export;
}
