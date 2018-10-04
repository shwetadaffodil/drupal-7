<?php
/** 
 * The plugin that added additional setting to views edit form.
 * 
 * @ingroup views_display_plugins 
 */
class view_exposed_filters_plugin_display_extender_code extends views_plugin_display_extender
{
    /** 
     * Provide a form to edit options for this plugin. 
     */
    function options_definition_alter(&$options) 
    { 
        $options['view_exposed_filters'] = array('default' => 'FALSE');
    } 
 
    /** 
     * Provide the form to set new option. 
     */
    function options_form(&$form, &$form_state) 
    { 
        switch ($form_state['section']) { 
        case 'view_exposed_filters': 
            $form['#title'] .= t('Custom controller for views exposed filters'); 
            $form['view_exposed_filters'] = array( 
            '#type' => 'checkbox',
            '#title' => 'Controller Filter',
            '#description' => t('Check to add Controller filter in exposed form'),
            '#default_value' => $this->display->get_option('view_exposed_filters'), 
            ); 
            break; 
        } 
    } 
    
    /** 
     * Inserts the code into the view display. 
     */
    function options_submit(&$form, &$form_state) 
    { 
        $new_option  = $form_state['values']['view_exposed_filters']; 
        switch ($form_state['section']) { 
        case 'view_exposed_filters': 
            $this->display->set_option('view_exposed_filters', $new_option); 
            $empty = trim($new_option); 
            $empty = empty($empty); 
            break; 
        } 
    } 
 
    /** 
     * Summarizes new option. 
     * 
     * Lists the fields as either 'Yes' if there is text or 'None' otherwise and
     * categorizes the fields under the 'Other' category. 
     */
    function options_summary(&$categories, &$options) 
    {
        $new_option = check_plain(trim($this->display->get_option('view_exposed_filters'))); 
        if ($new_option) { 
            $new_option = t('Yes'); 
        } 
        else { 
            $new_option = t('None'); 
        } 
        $options['view_exposed_filters'] = array( 
        'category' => 'other', 
        'title'    => t('Custom controller for views exposed filters'), 
        'value'    => $new_option, 
        'desc'     => t('Check to add Controller filter'), 
        ); 
    } 
}