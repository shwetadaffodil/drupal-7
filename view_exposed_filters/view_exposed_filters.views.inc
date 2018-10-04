<?php
/**
 * @file
 * Adds Views3 support.
 */

/**
 * Implements hook_views_plugins().
 */
function view_exposed_filters_views_plugins() 
{
    $path = drupal_get_path('module', 'view_exposed_filters');
    $plugins = array(); 
    $plugins['display_extender']['view_exposed_filters'] = array( 
    'title' => t('Custom view exposed filters'), 
    'help' => t('Check to alter expose form.'), 
    'path' => $path,
    'handler' => 'view_exposed_filters_plugin_display_extender_code',
    ); 
    return $plugins;
}
