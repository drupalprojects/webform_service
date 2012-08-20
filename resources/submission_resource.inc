<?php
/**
 * The media resource definition.
 *
 * @return array
 */

define('MEDIA_RESOURCE_INC', 'resources/media_resource');

function _media_resource_definition() {
  $media_resource = array(
    'media' => array(
      'alias' => 'media',
      // api/media/{uuid}
      'retrieve' => array(
        'file' => array('type' => 'inc', 'module' => 'truli_services', 'name' => 'resources/media_resource'),
        'callback' => '_media_resource_retrieve',
        'args' => array(
          array(
            'name' => 'uuid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'string',
            'description' => 'The uuid of the node to get',
          ),
        ),
        'access callback' => 'truli_services_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
      ),
      'create' => array(
        'file' => array('type' => 'inc', 'module' => 'truli_services', 'name' => 'resources/media_resource'),
        'callback' => '_media_resource_create',
        'args' => array(
          array(
            'name' => 'media',
            'optional' => FALSE,
            'source' => array('data' => 'media'),
            'description' => 'All the elements that make up the media entity.',
            'type' => 'array',
            'default value' => array(),
          ),
         ),// args
        'access callback' => 'truli_services_resource_access',
        'access arguments' => array('create'),
        'access arguments append' => TRUE,
      ),// create
      'update' => array(
        'file' => array('type' => 'inc', 'module' => 'truli_services', 'name' => 'resources/media_resource'),
        'callback' => '_media_resource_update',
        'args' => array(
          array(
            'name' => 'uuid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'string',
            'description' => 'The uuid of the node to get',
          ),
          array(
            'name' => 'node',
            'optional' => FALSE,
            'source' => 'data',
            'description' => 'The node data to update',
            'type' => 'array',
          ),
        ),
        'access callback' => 'truli_services_resource_access',
        'access arguments' => array('update'),
        'access arguments append' => TRUE,
      ),
      'delete' => array(
        'file' => array('type' => 'inc', 'module' => 'truli_services', 'name' => 'resources/media_resource'),
        'callback' => '_media_resource_delete',
        'args' => array(
          array(
            'name' => 'uuid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'string',
          ),
        ),
        'access callback' => 'truli_services_resource_access',
        'access arguments' => array('delete'),
        'access arguments append' => TRUE,
      ),

      // api/media
      'index' => truli_services_build_index(
        'resources/media_resource',
        'Returns all media',
        '_media_resource_index',
        'access content'
      ),// index

      'relationships' => array(
        'files' => truli_services_build_relationship(
          MEDIA_RESOURCE_INC,
          'This method returns files associated with a node.',
          '_media_resource_load_node_files',
          'access content',
          $args = array(
            array(
              'name' => 'file_contents',
              'type' => 'int',
              'description'  => t('To return file contents or not.'),
              'source' => array('path' => 2),
              'optional' => TRUE,
              'default value' => TRUE,
            ),
            array(
              'name' => 'image_styles',
              'type' => 'int',
              'description' => t('To return image styles or not.'),
              'source' => array('path' => 3),
              'default value' => FALSE,
            ),
          )
        ),// files
      ),// relationships
    ),
  );
  return $media_resource;
}

/**
 * Returns a single media resource.
 *
 * @param $entity
 *   UUID or the whole object of the media we want to return.
 * @return
 *   Node object or FALSE if not found.
 *
 * @see node_load()
 */
function _media_resource_retrieve($entity) {
  if ($entity = truli_services_get_media($entity)) {

    // Add the mobile media URL to the entity.
    if (limelight_include('media')) {
      $media = new LimelightMedia(array('id' => $entity['media_id']), FALSE);
      $media_info = $media->mobileURL();
      $entity['media_url'] = $media_info->mobile_urls[0]->url;
    }

    // Return the entity.
    return $entity;
  }
  return services_error(t('Media @uuid not found', array('@uuid' => $uuid)), 404);
}

/**
 * Creates a new node based on submitted values.
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
function _media_resource_create($media) {
  // Adds backwards compatability with regression fixed in #1083242
  $node = _services_arg_value($media, 'node');

  if (!isset($node['name'])) {
    // Assign username to the node from $user created at auth step.
    global $user;
    $node['name'] = $user->name;
  }

  // Load the required includes for drupal_execute
  module_load_include('inc', 'node', 'node.pages');

  $node_type = $node['type'];

  // Setup form_state
  $form_state = array();
  $form_state['values'] = $node;
  $form_state['values']['op'] = variable_get('services_node_save_button_' . $node_type . '_resource_create', t('Save'));

  drupal_form_submit($node_type . '_node_form', $form_state, (object)$node);

  if ($errors = form_get_errors()) {
    return services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
  }
  // Fetch $nid out of $form_state
  $nid = $form_state['nid'];
  // Only add the URI for servers that support it.
  $node = array('nid' => $nid);
  if ($uri = services_resource_uri(array('node', $nid))) {
    $node['uri'] = $uri;
  }
  return $node;
}

/**
 * Updates a new node based on submitted values.
 *
 * Note that this function uses drupal_execute() to create new nodes,
 * which may require very specific formatting. The full implications of this
 * are beyond the scope of this comment block. The Googles are your friend.
 *
 * @param $nid
 *   Node ID of the node we're editing.
 * @param $node
 *   Array representing the attributes a node edit form would submit.
 * @return
 *   The node's nid.
 *
 * @see drupal_execute()
 */
function _media_resource_update($uuid, $node) {
  $old_node = truli_services_resource_load($uuid);
  node_object_prepare($old_node);

  if ($old_node->nid) {
    // Node types cannot be changed once they are created.
    if (isset($node['type']) && $node['type'] != $old_node->type) {
      return services_error(t('Node type cannot be changed'), 406);
    }

    // Load the required includes for drupal_execute
    module_load_include('inc', 'node', 'node.pages');

    $node_type = $node['type'];

    // Setup form_state.
    $form_state = array();
    $form_state['values'] = $node;
    $form_state['values']['op'] = variable_get('services_node_save_button_' . $node_type . '_resource_update', t('Save'));
    $form_state['node'] = $old_node;
    drupal_form_submit($node_type . '_node_form', $form_state, $old_node);

    if ($errors = form_get_errors()) {
      return services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
    }
  }
  else {
    return services_error(t('Node @nid not found', array('@nid' => $old_node->nid)), 404);
  }

  $node = array('nid' => $nid);
  if ($uri = services_resource_uri(array('node', $nid))) {
    $node['uri'] = $uri;
  }
  return $node;
}

/**
 * Delete a node given its nid.
 *
 * @param $nid
 *   Node ID of the node we're deleting.
 * @return
 *   The node's nid.
 */
function _media_resource_delete($uuid) {
  if ($node = truli_services_resource_load($uuid)) {
    node_delete($node->nid);
  }
  return TRUE;
}

/**
 * Return an array of optionally paged nids baed on a set of criteria.
 *
 * An example request might look like
 *
 * http://domain/endpoint/node?fields=nid,vid&parameters[nid]=7&parameters[uid]=1
 *
 * This would return an array of objects with only nid and vid defined, where
 * nid = 7 and uid = 1.
 *
 * @param $page
 *   Page number of results to return (in pages of 20).
 * @param $fields
 *   The fields you want returned.
 * @param $search
 *   An array containing fields and values used to build a sql WHERE clause
 *   indicating items to retrieve.
 * @param $page_size
 *   Integer number of items to be returned.
 * @return
 *   An array of node objects.
 *
 * @todo
 *   Evaluate the functionality here in general. Particularly around
 *     - Do we need fields at all? Should this just return full nodes?
 *     - Is there an easier syntax we can define which can make the urls
 *       for index requests more straightforward?
 */
function _media_resource_index($page, $page_size, $search) {

  // Setting the limit for media objects returned.
  truli_services_validate_parameters($page_size);

  $function = 'truli_services_get_media';

  $query = new EntityFieldQuery();

  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'media')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_limelight_state', 'value', 'Published')
    ->propertyOrderBy('created', 'DESC')
    ->range($page * $page_size, $page_size);

  return truli_services_resource_index('media', $search, $function, $query);
}

/**
 * Generates an array of base64 encoded files attached to a node
 *
 * @param $nid
 *   Number. Node ID
 * @param $include_file_contents
 *   Bool Whether or not to include the base64_encoded version of the file.
  * @param $get_image_style
 *   Bool Whether or not to provide image style paths.
 * @return
 *   Array. A list of all files from the given node
 */
function _media_resource_load_node_files($uuid, $include_file_contents, $get_image_style) {
  module_load_include('inc', 'services', 'resources/file_resource');
  $node = truli_services_resource_load($uuid);

  // Hopefully theres another way to get a nodes fields that are a file, but this was the only way I could do it.
  $fields = field_info_fields();
  $files = array();

  // Loop through all of the fields on the site
  foreach ($fields as $key => $field) {
    //if we are a field type of file
    if ($field['type'] == 'image' || $field['type'] == 'file') {
      // If this field exists on our current node..
      if (isset($node->{$field['field_name']})) {
        // If there are items in the field...
        if (isset($node->{$field['field_name']}[LANGUAGE_NONE])) {
          // Grab the items given and attach them to the array.
          $node_file_field_items = $node->{$field['field_name']}[LANGUAGE_NONE];
          foreach ($node_file_field_items as $file) {
            $files[] = _file_resource_retrieve($file['fid'], $include_file_contents, $get_image_style);
          }
        }
      }
    }
  }

  return $files;
}
