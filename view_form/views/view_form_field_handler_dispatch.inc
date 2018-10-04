<?php
/**
 * @file
 * A Views' field handler for editing a node title.
 */

class View_form_field_handler_dispatch  extends views_handler_field
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
        if ($values->views_php_4 > 0) {
            return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
        }
        else {
            return format_string('No videos to dispatch');
        }
    }
    /**
     * View form.
     */
    public function views_form(&$form, &$form_state) 
    {
        $form[$this->options['id']] = array(
        '#type' => 'container',
        '#tree' => true,
        );

        foreach ($this->view->result as $row_index => $row) {
            $form[$this->options['id']][$row_index]['dispatch'] = array(
            '#type' => 'select',
            '#options' => array('yes' => 'Yes', 'no' => 'No'),
            '#default_value' => 'no',
            '#disabled' => ($row->views_php_4 > 1) ? false : true,
            '#prefix' => '<div class="dispatch-col-wrapper">',
            );
            $options = select_company_users();
            $form[$this->options['id']][$row_index]['select_user'] = array(
            '#type' => 'select',
            '#title' => check_plain($this->options['id'] . '-selectuser-' . $row_index),
            '#title_display' => 'invisible',
            '#prefix' => '<div class="select-user-col">',
            '#options' => $options,
            '#default_value' => 0,
            '#states' => array(
              'visible' => array(
              'select[id="edit-dispatch-' . $row_index . '-dispatch"]' => array('value' => 'no'),
            ),
            ),
            '#ajax' => array(
            'callback' => 'user_info_callback',
            'wrapper' => 'user-info-wrapper-' . $row_index,
            'event' => 'change',
            'method' => 'replace',
            ),
            );

            $form[$this->options['id']][$row_index]['info'] = array(
            '#type' => 'item',
            '#prefix' => '<div id="user-info-wrapper-' . $row_index . '">',
            '#suffix' => '</div>',
            );

            $val = variable_get('user_no_dropdoen', '');
            $totaluser_options = getno_of_totalusers($val);

            $form[$this->options['id']][$row_index]['total_users'] = array(
            '#type' => 'select',
            '#title' => check_plain($this->options['id'] . '-totalusers-' . $row_index),
            '#title_display' => 'invisible',
            '#options' => $totaluser_options,
            '#default_value' => 0,
            '#states' => array(
              'visible' => array(
              'select[id="edit-dispatch-' . $row_index . '-dispatch"]' => array('value' => 'yes'),
            ),
            ),
            '#ajax' => array(
            'callback' => 'dispatch_form_callback',
            'wrapper' => 'subuser-fieldset-wrapper-' . $row_index,
            'event' => 'change',
            'method' => 'replace',
            'effect' => 'fade',
            ),

            );

            $form[$this->options['id']][$row_index]['select_subuser'] = array(
            '#type' => 'fieldset',
            '#title' => check_plain('Company Select Users'),
            '#prefix' => '<div id="subuser-fieldset-wrapper-' . $row_index . '">',
            '#suffix' => '</div></div></div>',
            '#states' => array(
              'visible' => array(
              'select[id="edit-dispatch-' . $row_index . '-dispatch"]' => array('value' => 'yes'),
            ),
            ),
            );
        }
        if (isset($form_state['values'])) {
            if (preg_match('/totalusers/', $form_state['triggering_element']['#title'])) {
                $element = $form_state['triggering_element']['#title'];
                $id = explode('-', $element);
                $optionsId = $id[0];
                $rowIndex = $id[2];

                $value = $form_state['values'][$optionsId][$rowIndex]['total_users'];
                $options = select_company_users();

                for ($i = 1; $i <= $value; $i++) {

                    $form[$optionsId][$rowIndex]['select_subuser']['select_subuser_' . $i] = array(
                    '#type' => 'select',
                    '#title' => check_plain($optionsId . '-subusers-' . $rowIndex . '-' . $i),
                    '#title_display' => 'invisible',
                    '#options' => $options,
                    '#default_value' => 0,
                    '#states' => array(
                    'visible' => array(
                    'select[id="edit-dispatch-' . $rowIndex . '-dispatch"]' => array('value' => 'yes'),
                    ),
                    ),
                    '#ajax' => array(
                    'callback' => 'subuser_info_callback',
                    'wrapper' => 'subuser-info-wrapper-' . $rowIndex . '-' . $i,
                    'event' => 'change',
                    'method' => 'replace',
                    ),
                    );          

                    $form[$optionsId][$rowIndex]['select_subuser']['videosfor_user_' . $i] = array(
                    '#type' => 'textfield',
                    //'#title' => check_plain('Videos'),
                    //'#attributes' =>array('placeholder' => t('Videos')),
                    '#size' => 10,
                    '#states' => array(
                    'visible' => array(
                      'select[id="edit-dispatch-' . $rowIndex . '-dispatch"]' => array('value' => 'yes'),
                    ),
                    ),
                    );
          
                    $form[$optionsId][$rowIndex]['select_subuser']['subuser_info_' . $i] = array(
                    '#type' => 'item',
                    '#prefix' => '<div id="subuser-info-wrapper-' . $rowIndex . '-' . $i . '">',
                    '#suffix' => '</div>',
                    );
                }
            }
            elseif (preg_match('/subusers/', $form_state['triggering_element']['#title'])) {
                $element = $form_state['triggering_element']['#title'];
                $id = explode('-', $element);
                $optionsId = $id[0];
                $rowIndex = $id[2];

                $totalusers = $form_state['values'][$optionsId][$rowIndex]['total_users'];
                $users = select_company_users();

                for ($i = 1; $i <= $totalusers; $i++) {

                    $form[$optionsId][$rowIndex]['select_subuser']['select_subuser_' . $i] = array(
                    '#type' => 'select',
                    '#title' => check_plain($optionsId . '-subusers-' . $rowIndex . '-' . $i),
                    '#title_display' => 'invisible',
                    '#options' => $users,
                    '#default_value' => 0,
                    '#states' => array(
                    'visible' => array(
                    'select[id="edit-dispatch-' . $rowIndex . '-dispatch"]' => array('value' => 'yes'),
                    ),
                    ),
                    '#ajax' => array(
                    'callback' => 'subuser_info_callback',
                    'wrapper' => 'subuser-info-wrapper-' . $rowIndex . '-' . $i,
                    'event' => 'change',
                    'method' => 'replace',
                    'progress' => array('type' => 'throbber'),
                    ),
                    );

                    $form[$optionsId][$rowIndex]['select_subuser']['videosfor_user_' . $i] = array(
                    '#type' => 'textfield',
                    '#title' => check_plain('Videos'),
                    '#size' => 10,
                    '#states' => array(
                      'visible' => array(
                      'select[id="edit-dispatch-' . $rowIndex . '-dispatch"]' => array('value' => 'yes'),
                    ),
                    ),
                    );
                    $form[$optionsId][$rowIndex]['select_subuser']['subuser_info_' . $i] = array(
                    '#type' => 'item',
                    '#prefix' => '<div id="subuser-info-wrapper-' . $rowIndex . '-' . $i . '">',
                    '#suffix' => '</div>',
                    );
                }

                $i = $id[3];
                $options = $form_state['triggering_element']['#options'];
                $value = $form_state['values'][$optionsId][$rowIndex]['select_subuser']['select_subuser_' . $i];
                $username = $options[$value];
                if ($value != 0) {
                    $row_count = fetch_tasks($value);
                    $markup = $row_count > 0 ? $username . ' current assignment <br/>' . $row_count . ' videos to do' : 'No Tasks assigned';
                    $form[$optionsId][$rowIndex]['select_subuser']['subuser_info_' . $i] = array(
                    '#type' => 'item',
                    '#markup' => $markup,
                    '#prefix' => '<div id="subuser-info-wrapper-' . $rowIndex . '-' . $i . '">',
                    '#suffix' => '</div>',
                    '#states' => array(
                    'visible' => array(
                    'select[id="edit-dispatch-' . $rowIndex . '-dispatch"]' => array('value' => 'yes'),
                    ),
                    ),
                    );
                }
            }
            elseif (preg_match('/selectuser/', $form_state['triggering_element']['#title'])) {
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
                    '#prefix' => '<div id="user-info-wrapper-' . $rowIndex . '">',
                    '#suffix' => '</div>',
                    '#states' => array(
                    'visible' => array(
                    'select[id="edit-dispatch-' . $rowIndex . '-dispatch"]' => array('value' => 'no'),
                    ),
                    ),
                    );
                }
            }
        }
    }

    /**
     * FORM Validate.
     */
    function views_form_validate($form, &$form_state) 
    {
        if (isset($form_state['input']['op']) && (strcasecmp($form_state['input']['op'], 'Save') == 0)) {
            foreach ($this->view->result as $row_index => $row) {
                if (($form_state['values']['dispatch'][$row_index]['dispatch'] == 'yes') && ($form_state['values']['dispatch'][$row_index]['total_users'] > 0)) {
                    $videosfor_user = 0;
                    $total_users = $form_state['values']['dispatch'][$row_index]['total_users'];
                    for ($j = 1; $j <= $total_users; $j++) {            
                        $subuser_id = isset($form_state['values']['dispatch'][$row_index]['select_subuser']['select_subuser_' . $j]) ? $form_state['values']['dispatch'][$row_index]['select_subuser']['select_subuser_' . $j] : 0;
                        if ($subuser_id != 0) {
                            if (!empty($form_state['values']['dispatch'][$row_index]['select_subuser']['videosfor_user_' . $j])) {
                                $videosfor_user = $videosfor_user + $form_state['values']['dispatch'][$row_index]['select_subuser']['videosfor_user_' . $j];
                            }
                            else {
                                form_set_error('', t('Enter number of videos to be assigned to each selected User.'));
                            }
                        }
                        else {
                            form_set_error('', t('Select user first.'));
                        }
                    }

                    if ($row->views_php_4 < $videosfor_user) {
                        form_set_error('', t('Numbers of videos dispatched did not match with the assigned number of videos.'));
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
        if (!empty($form_state['input']['op']) && (strcasecmp($form_state['input']['op'], 'Save') == 0)) {
            $saved = false;
            foreach ($this->view->result as $row_index => $row) {
                $entity_id = db_query("SELECT p.entity_id FROM {field_data_field_parent_node_reference} p where p.`field_parent_node_reference_target_id` = :nid and p.entity_id NOT IN ( Select `entity_id` from {field_data_field_company_subuser_refernce} )", array(':nid' => $row->nid))->fetchAll();
                if ($form_state['values']['dispatch'][$row_index]['dispatch'] == 'no' && $form_state['values']['dispatch'][$row_index]['select_user'] != 0) {
                    $subuser_id = $form_state['values']['dispatch'][$row_index]['select_user'];
                    $user_fields = user_load($subuser_id);
                    $usermail = $user_fields->mail;
                    $mail_body = "<br/>Hi " . $user_fields->field_first_name[LANGUAGE_NONE]['0']['value'] . "<br/><br/>Following videos has been assigned to you by your manager : <br/><br/>";
                    if (!empty($entity_id)) {
                        foreach ($entity_id as $obj) {
                            $update_node = node_load($obj->entity_id);
                            $update_node->field_company_subuser_refernce[$update_node->language][0]['target_id'] = $subuser_id;
                            $update_node->field_vm_status[$update_node->language][0]['tid'] = 49;
                            $update_node->field_date_of_job_assignment[$update_node->language][0]['value']['date'] = date("Y-m-d h:m:s");
                            node_save($update_node);
                            $mail_body .= " - " . $update_node->title . "<br/>";
                            $saved = true;
                        }
                    }
                    if (sendjobmail($usermail, $mail_body)) {
                        drupal_set_message(t("Mail has been sent to subuser"));
                    }
                    else {
                        drupal_set_message(t("There is some error while sending mail"), 'warning');
                    }
                }
                elseif ($form_state['values']['dispatch'][$row_index]['dispatch'] == 'yes') {
                    $total_users = $form_state['values']['dispatch'][$row_index]['total_users'];
                    if ($total_users > 0) {
                        for ($j = 1; $j <= $total_users; $j++) {
                            $subuser_id = $form_state['values']['dispatch'][$row_index]['select_subuser']['select_subuser_' . $j];
                            if ($subuser_id != 0) {
                                $user_fields = user_load($subuser_id);
                                $usermail = $user_fields->mail;
                                $mail_body = "<br/>Hi " . $user_fields->field_first_name[LANGUAGE_NONE]['0']['value'] . "<br/><br/>Following videos has been assigned to you by your manager : <br/><br/>";
                                $videosfor_user = $form_state['values']['dispatch'][$row_index]['select_subuser']['videosfor_user_' . $j];

                                for ($v = 0; $v < count($entity_id), $v < $videosfor_user; $v++) {
                                    $obj = each($entity_id);
                                    if (!empty($obj)) {
                                        $id = $obj['value']->entity_id;
                                        $update_node = node_load($id);
                                        $update_node->field_company_subuser_refernce[$update_node->language][0]['target_id'] = $subuser_id;
                                        // Video status changed to assigned.
                                        $update_node->field_vm_status[$update_node->language][0]['tid'] = 49;
                                        $update_node->field_date_of_job_assignment[$update_node->language][0]['value']['date'] = date("Y-m-d h:m:s");
                                        node_save($update_node);
                                        $mail_body .= " - " . $update_node->title;
                                        $saved = true;
                                    }
                                }
                                if (sendjobmail($usermail, $mail_body)) {
                                    drupal_set_message(t("Mail has been sent to sub user."));
                                }
                                else {
                                    drupal_set_message(t("There is some error while sending mail."), 'warning');
                                }
                            }
                        }
                    }
                }
            }
            if ($saved === true) {
                drupal_set_message(t("Videos assigned to company's sub user."));
            }
            else {
                drupal_set_message(t("Select user first"), 'warning');
            }
        }
    }
}

/**
 * Send job assigned mail.
 */
function sendjobmail($to, $body) 
{
    $parms['subject'] = 'Job Assignment';
    $parms['body'] = $body . "<br/><br/>Please start processing and check further details in your account.<br/><br/> Regards<br/>Daily Motion";
    if (drupal_mail('forgot_username', 'send_mail', $to, language_default(), $parms, variable_get('site_name', ''))) {
        return true;
    }
    else {
        return false;
    }
}

/**
 * Ajax Dispatch callback.
 */
function dispatch_form_callback($form, &$form_state) 
{
    $element = $form_state['triggering_element']['#title'];
    $id = explode('-', $element);
    $options_id = $id[0];
    $row_index = $id[2];
    return $form[$options_id][$row_index]['select_subuser'];
}

/**
 * Ajax callback for select user job status.
 */
function user_info_callback($form, &$form_state) 
{
    $element = $form_state['triggering_element']['#title'];
    $id = explode('-', $element);
    $options_id = $id[0];
    $row_index = $id[2];
    return $form[$options_id][$row_index]['info'];
}

/**
 * Ajax callback for select user job status in case of YES to dispach.
 */
function subuser_info_callback($form, &$form_state) 
{
    $element = $form_state['triggering_element']['#title'];
    $id = explode('-', $element);
    $options_id = $id[0];
    $row_index = $id[2];
    $i = $id[3];
    return $form[$options_id][$row_index]['select_subuser']['subuser_info_' . $i];
}
/**
 * Populate dropdown options.
 */
function getno_of_totalusers($val) 
{
    $res = array();
    $res[0] = 'Total Users';
    for ($i = 1; $i <= $val; $i++) {
        $res[$i] = $i;
    }
    return $res;
}
