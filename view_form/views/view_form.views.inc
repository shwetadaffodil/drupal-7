<?php
/**
 * @file
 * Implements hook_views_data_alter().
 *
 * @param $data
 * Information about Views' tables and fields.
 */
function view_form_views_data_alter(&$data) 
{
    $data['node']['dispatch'] = array(
    'field' => array(
      'title' => t('Dispatch'),
      'help' => t('Dispatch videos to sub users.'),
      'handler' => 'View_form_field_handler_dispatch',
    ),
    );

    $data['node']['reassign'] = array(
    'field' => array(
      'title' => t('Re-assign'),
      'help' => t('Re-assign videos to sub users.'),
      'handler' => 'View_form_field_handler_reassign',
    ),
    );

    $data['node']['filterbyuser_select'] = array(
    'group' => t('Content'),
    'title' => t('Filter by User (select list)'),
    'help' => t('Custom Filter by subuser, choosing from dropdown list.'),
    'filter' => array('handler' => 'view_form_handler_filter_filterbyuser_select'),
    'real field' => 'filterbyuser_select',
    );

    $data['node']['filterby_reportinguser'] = array(
    'group' => t('Content'),
    'title' => t('Filter by Reporting User (to Manager)'),
    'help' => t('Custom Filter by subuser for reporting tab of manager, choosing from dropdown list.'),
    'filter' => array('handler' => 'view_form_handler_filter_filterby_reportinguser'),
    'real field' => 'filterby_reportinguser',
    );
}
