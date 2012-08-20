<?php
/**
 * The webform resource definition.
 *
 * @return array
 */

function _webform_resource_definition() {
  return array(
    'webform' => array(
      'retrieve' => array(
        'callback' => 'webform_service_get_resource',
        'args' => array(
          array(
            'name' => 'uuid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'string',
            'description' => 'The uuid of the node to get',
          ),
        ),
        'access callback' => 'webform_service_resource_access',
        'access arguments' => array('view'),
        'access arguments append' => TRUE,
      ),
      'create' => array(
        'file' => array('type' => 'inc', 'module' => 'webform_service', 'name' => 'resources/webform_resource'),
        'callback' => '_webform_resource_create',
        'args' => array(
          array(
            'name' => 'webform',
            'optional' => FALSE,
            'source' => array('data' => 'webform'),
            'description' => 'All the elements that make up the webform entity.',
            'type' => 'array',
            'default value' => array(),
          ),
         ),// args
        'access callback' => 'webform_service_resource_access',
        'access arguments' => array('create'),
        'access arguments append' => TRUE,
      ),// create
      'update' => array(
        'file' => array('type' => 'inc', 'module' => 'webform_service', 'name' => 'resources/webform_resource'),
        'callback' => '_webform_resource_update',
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
        'access callback' => 'webform_service_resource_access',
        'access arguments' => array('update'),
        'access arguments append' => TRUE,
      ),
      'delete' => array(
        'callback' => 'webform_service_resource_delete',
        'args' => array(
          array(
            'name' => 'uuid',
            'optional' => FALSE,
            'source' => array('path' => 0),
            'type' => 'string',
          ),
        ),
        'access callback' => 'webform_service_resource_access',
        'access arguments' => array('delete'),
        'access arguments append' => TRUE,
      ),

      // webform
      'index' => array(
        'file' => array('type' => 'inc', 'module' => 'webform_service', 'name' => 'resources/webform_resource'),
        'help' => t('Return all webforms.'),
        'callback' => '_webform_resource_index',
        'args' => array(
          array(
            'name' => 'page',
            'optional' => TRUE,
            'type' => 'int',
            'description' => 'The zero-based index of the page to get, defaults to 0.',
            'default value' => 0,
            'source' => array('param' => 'page'),
          ),
          array(
            'name' => 'pagesize',
            'optional' => TRUE,
            'type' => 'int',
            'description' => 'Number of records to get per page.',
            'default value' => variable_get('services_node_index_page_size', 20),
            'source' => array('param' => 'pagesize'),
          ),
          array(
            'name' => 'type',
            'optional' => TRUE,
            'type' => 'string',
            'description' => 'Webform Type',
            'default value' => '',
            'source' => array('param' => 'type'),
          ),
        ),
        'access arguments' => array('access content'),
      ),// index

      'relationships' => array(
        'submissions' => array(
          'file' => array('type' => 'inc', 'module' => 'webform_service', 'name' => 'resources/webform_resource'),
          'help' => t('Retrieve the submissions provided a webform.'),
          'callback' => '_webform_resource_submissions',
          'args' => array(
            array(
              'name' => 'uuid',
              'optional' => FALSE,
              'source' => array('path' => 0),
              'type' => 'string',
              'description' => t('The uuid of the webform you want the relationship for.'),
            ),
            array(
              'name' => 'page',
              'optional' => TRUE,
              'type' => 'int',
              'description' => 'The zero-based index of the page to get, defaults to 0.',
              'default value' => 0,
              'source' => array('param' => 'page'),
            ),
            array(
              'name' => 'pagesize',
              'optional' => TRUE,
              'type' => 'int',
              'description' => 'Number of records to get per page.',
              'default value' => variable_get('services_node_index_page_size', 20),
              'source' => array('param' => 'pagesize'),
            ),
          ),
          'access arguments' => array('access content'),
        ),// submissions
      ),// relationships
    ),
  );
}

/**
 * Creates a new webform based on submitted values.
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
function _webform_resource_create($webform) {
  // @todo:  Build this out...
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
function _webform_resource_update($uuid, $node) {
  // @todo: Build this out.
  return NULL;
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
function _webform_resource_index($page, $page_size, $type) {

  // Get the webform types they wish to include.
  $webform_types = $type ? array($type) : webform_variable_get('webform_node_types');

  // Validate the page_size parameter.
  webform_service_validate_parameters($page_size);

  // Get the query.
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', $webform_types, 'IN')
    ->propertyCondition('status', 1)
    ->propertyOrderBy('created', 'DESC')
    ->range($page * $page_size, $page_size)
    ->execute();

  // Pass this along to our resource index parser.
  return webform_service_resource_index('webform', $result);
}

/**
 * Retrieve all submissions for a webform.
 */
function _webform_resource_submissions($uuid, $page, $page_size) {
  // @todo: Build this out.
}
