<?php
/**
 * The submission resource definition.
 *
 * @return array
 */
function _submission_resource_definition() {
  return array(
    'submission' => array(
      'retrieve' => array(
        'file' => array('type' => 'inc', 'module' => 'webform_service', 'name' => 'resources/submission_resource'),
        'callback' => 'webform_service_get_submission',
        'args' => array(
          array(
            'name' => 'uuid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'string',
            'description' => 'The uuid of the webform to get',
          ),
          array(
            'name' => 'sid',
            'optional' => FALSE,
            'source' => array('path' => 1),
            'type' => 'int',
            'description' => 'The submission ID within this webform',
          ),
        ),
        'access callback' => 'webform_service_submission_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
      ),
      'create' => array(
        'file' => array('type' => 'inc', 'module' => 'webform_service', 'name' => 'resources/submission_resource'),
        'callback' => 'webform_service_submission_create',
        'args' => array(
          array(
            'name' => 'uuid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'string',
            'description' => 'The uuid of the webform.',
          ),
          array(
            'name' => 'submission',
            'optional' => FALSE,
            'source' => array('data' => 'submission'),
            'description' => 'The submission data for this webform.',
            'type' => 'array',
            'default value' => array(),
          ),
         ),// args
        'access callback' => 'webform_service_submission_access',
        'access arguments' => array('create'),
        'access arguments append' => TRUE,
      ),// create
      'update' => array(
        'file' => array('type' => 'inc', 'module' => 'webform_service', 'name' => 'resources/submission_resource'),
        'callback' => 'webform_service_submission_update',
        'args' => array(
          array(
            'name' => 'uuid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'string',
            'description' => 'The uuid of the node to get',
          ),
          array(
            'name' => 'sid',
            'optional' => FALSE,
            'source' => array('path' => 1),
            'type' => 'int',
            'description' => 'The submission ID within this webform',
          ),
          array(
            'name' => 'submission',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The submission data to update',
            'type' => 'array',
          ),
        ),
        'access callback' => 'webform_service_submission_access',
        'access arguments' => array('update'),
        'access arguments append' => TRUE,
      ),
      'delete' => array(
        'file' => array('type' => 'inc', 'module' => 'webform_service', 'name' => 'resources/submission_resource'),
        'callback' => 'webform_service_submission_delete',
        'args' => array(
          array(
            'name' => 'uuid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'string',
            'description' => 'The uuid of the node to get',
          ),
          array(
            'name' => 'sid',
            'optional' => FALSE,
            'source' => array('path' => 1),
            'type' => 'int',
            'description' => 'The submission ID within this webform',
          ),
          array(
            'name' => 'submission',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The submission data to delete.',
            'type' => 'array',
          ),
        ),
        'access callback' => 'webform_service_resource_access',
        'access arguments' => array('delete'),
        'access arguments append' => TRUE,
      ),
    ),
  );
}

/**
 * Gets the submission for the provided resource and submission ID.
 */
function webform_service_get_submission($webform, $sid) {
  $submission = array();

  // Get the webform entity.
  if (gettype($webform) === 'string') {
    $webform = webform_service_resource_load($webform);
  }

  // If the entity exists.
  if ($webform) {

    // Get the submission.
    module_load_include('inc', 'webform', 'includes/webform.submissions');
    $submission = webform_get_submission($webform->nid, $sid);
  }

  // Return the submission.
  return $submission;
}

/**
 * Creates a new submission within a webform.
 *
 * Note that this function uses drupal_execute() to create new nodes,
 * which may require very specific formatting. The full implications of this
 * are beyond the scope of this comment block. The Googles are your friend.
 *
 * @param $media
 *   Array representing the attributes a media edit form would submit.
 * @return
 *   An associative array contained the new node's nid and, if applicable,
 *   the fully qualified URI to this resource.
 *
 * @see drupal_execute()
 */
function webform_service_submission_create($webform, $submission) {
  // Get the webform entity.
  if (gettype($webform) === 'string') {
    $webform = webform_service_resource_load($webform);
  }

  // If the entity exists.
  if ($webform) {

    module_load_include('inc', 'webform', 'includes/webform.submissions');
    webform_submission_insert($webform, $submission);
  }

  return TRUE;
}

/**
 * Updates a webform submission based on submitted values.
 *
 * Note that this function uses drupal_execute() to create new nodes,
 * which may require very specific formatting. The full implications of this
 * are beyond the scope of this comment block. The Googles are your friend.
 *
 * @param $uuid
 *   UUID of the webform we're editing.
 * @param $sid
 *   Submission ID of the submission we are editing.
 * @param $submission
 *   Array representing the submission.
 * @return
 *   The submission object.
 *
 * @see drupal_execute()
 */
function webform_service_submission_update($webform, $sid, $submission) {
  // Get the webform entity.
  if (gettype($webform) === 'string') {
    $webform = webform_service_resource_load($webform);
  }

  // If the entity exists.
  if ($webform) {
    $submission->sid = $sid;
    module_load_include('inc', 'webform', 'includes/webform.submissions');
    webform_submission_update($webform, $submission);
  }
}

/**
 * Delete a submission within a webform.
 *
 * @param $nid
 *   Node ID of the node we're deleting.
 * @return
 *   The node's nid.
 */
function webform_service_submission_delete($webform, $sid, $submission) {
  // Get the webform entity.
  if (gettype($webform) === 'string') {
    $webform = webform_service_resource_load($webform);
  }

  // If the entity exists.
  if ($webform) {
    $submission->sid = $sid;
    module_load_include('inc', 'webform', 'includes/webform.submissions');
    webform_submission_delete($webform, $submission);
  }
}
