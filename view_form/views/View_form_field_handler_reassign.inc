<?php
/**
 * @file
 * A Views' field handler for editing a node title.
 */

class View_form_field_handler_reassign  extends views_handler_field
{
    /**
     * Construct. 
     */
    public function construct() 
    {
        parent::construct();
        $this->additional_fields['nid'] = 'nid';
    }
    /**
     * Render querry.
     */
    public function query() 
    {
        $this->ensure_my_table();
        $this->add_additional_fields();
    }
    /**
     * Render a Views form item placeholder.This causes Views to wrap the View in a form.       
     */
    public function render($values) 
    {
        return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
    }
    /**
     * View form.
     */
    public function views_form(&$form, &$form_state) 
    {
        global $base_url;
        $form[$this->options['id']] = array(
        '#type' => 'container',
        '#tree' => true,
        );
        $options = select_company_users();
        foreach ($this->view->result as $row_index => $row) {
            $form[$this->options['id']][$row_index]['select_user'] = array(
            '#type' => 'select',
            '#title' => check_plain($this->options['id'] . '-selectuser-' . $row_index),
            '#title_display' => 'invisible',
            '#prefix' => '<div>',
            '#options' => $options,
            '#default_value' => 0,
            '#ajax' => array(
            'callback' => 'reassignuser_info_callback',
            'wrapper' => 'reassignuser-info-wrapper-' . $row_index,
            'event' => 'change',
            'method' => 'replace',
            ),
            );

            $form[$this->options['id']][$row_index]['info'] = array(
            '#type' => 'item',
            '#prefix' => '<div id="reassignuser-info-wrapper-' . $row_index . '">',
            '#suffix' => '</div>',
            );

            $form[$this->options['id']][$row_index]['re_assign'] = array(
            '#type' => 'submit',
            '#value' => 'Re-assign',
            );

            $form['#action'] = $base_url . '/pm-dashboard/rejected/';

            if (isset($form_state['values'])) {
                if (preg_match('/selectuser/', $form_state['triggering_element']['#title'])) {
                    $element = $form_state['triggering_element']['#title'];
                    $id = explode('-', $element);
                    $optionsId = $id[0];
                    $rowIndex = $id[2];
                    $options = $form_state['triggering_element']['#options'];
                    $value = $form_state['values'][$optionsId][$rowIndex]['select_user'];
                    $username = $options[$value];
                    if ($value != 0) {
                        $row_count = fetch_tasks($value);
                        $markup = $row_count > 0 ? $username . ' current assignment <br/>' . $row_count . ' videos to do' : 'No Tasks assigned';
                        $form[$optionsId][$rowIndex]['info'] = array(
                        '#type' => 'item',
                        '#markup' => $markup,
                        '#prefix' => '<div id="reassignuser-info-wrapper-' . $rowIndex . '">',
                        '#suffix' => '</div>',
                        );
                    }
                }
            }
        }
    }

    /**
     * Form Submit.
     */
    function views_form_submit($form, &$form_state) 
    {
        switch ($form_state['input']['op']) {
        case 'Re-assign':
            foreach ($this->view->result as $row_index => $row) {
                if ($form_state['values']['reassign'][$row_index]['select_user'] != 0) {
                    $subuser_id = $form_state['values']['reassign'][$row_index]['select_user'];
                    $update_node = node_load($row->nid);
                    $update_node->field_company_subuser_refernce[$update_node->language][0]['target_id'] = $subuser_id;
                    $update_node->field_vm_status[$update_node->language][0]['tid'] = 49;
                    $update_node->field_date_of_job_assignment[$update_node->language][0]['value']['date'] = date("Y-m-d h:m:s");
                    node_save($update_node);
                    drupal_set_message('Video (' . $update_node->field_video_id['und'][0]['value'] . ' - ' . $update_node->title . ') has been re-assigned.', 'status');
                }
            }
            break;
        }
    }
}

/**
 * Ajax callback for select user job status.
 */
function reassignuser_info_callback($form, &$form_state) 
{
    $element = $form_state['triggering_element']['#title'];
    $id = explode('-', $element);
    $options_id = $id[0];
    $row_index = $id[2];
    return $form[$options_id][$row_index]['info'];
}
