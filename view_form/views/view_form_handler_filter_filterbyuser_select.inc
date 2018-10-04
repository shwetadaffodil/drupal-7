<?php
/**
 * My custom filter handler
 */
class view_form_handler_filter_filterbyuser_select extends views_handler_filter_in_operator
{

    /**
     * Override parent get_value_options() function.
     *
     * @return
     *   Return the stored values in $this->value_options if someone expects it.
     */
    function get_value_options() 
    {
        global $user;
        if(isset($user->roles[4]) && $user->roles[4] == "DM Admin") {
            $users = select_sub_users(5);
            $companies = array();
            if(!empty($users)) {
                foreach ($users as $key => $val) {        
                    $query = db_select('field_data_field_company_name', 'fdc');
                    $query->fields('fdc', array('field_company_name_value'));
                    $query->condition('fdc.entity_id', $key, '=');
                    $result = $query->execute()->fetch();
                    $companies[$key] = $result->field_company_name_value;
                }
            }      
            $this->value_options = $companies;
            return $companies;
        }
        else {
            $users = select_company_users();
            unset($users[0]);
            $this->value_options = $users;
            return $users;
        }
    }
    function query() 
    {
    
        global $user;
        $this->ensure_my_table();
        $real_field_name = $this->real_field;
        $value = $this->view->exposed_data[$real_field_name];
        if ($value != 0) {
            if (isset($user->roles[4]) && $user->roles[4] == "DM Admin") {
                if (arg(1) == "submitted-jobs") {
                    $this->query->add_where_expression(
                        $this->options['group'],
                        "field_data_field_job_assigned_company.field_job_assigned_company_target_id = :uid",
                        array(':uid' => $value)
                    );
                }
                else {
                    $this->query->add_where_expression(
                        $this->options['group'],
                        "node_field_data_field_parent_node_reference__field_data_field_job_assigned_company.field_job_assigned_company_target_id = :uid",
                        array(':uid' => $value)
                    );
                }
            }
            else {
                $this->query->add_where_expression(
                    $this->options['group'],
                    "field_data_field_company_subuser_refernce.field_company_subuser_refernce_target_id = :uid",
                    array(':uid' => $value)
                );  
            }
        }
    }
}

function select_sub_users($role) 
{   
    global $user;
    $result = array();
    $query  = db_select('users', 'u');
    $query->fields('u', array('uid', 'name'));
    $query->join('field_data_endpoints', 'fde', 'u.uid = fde.endpoints_entity_id');
    $query->where('fde.bundle = :subuser', array(':subuser' => 'subuser'));
    $query->where('fde.endpoints_entity_type = :user', array(':user' => 'user'));
    $query->where('fde.endpoints_r_index = :index', array(':index' => 0));
    $query->join('field_data_endpoints', 'fde2', 'fde.entity_id = fde2.entity_id');
    $query->condition('fde2.endpoints_entity_type', 'user', '=');
    $query->where("fde2.endpoints_r_index != fde.endpoints_r_index");
    $query->join('users', 'users_users', 'fde2.endpoints_entity_id = users_users.uid');
    $query->condition('fde2.endpoints_entity_type', 'user', '=');
    $query->join('users_roles', 'ur', 'u.uid = ur.uid');
    $query->condition('ur.rid', $role, '=');
    $query->condition('users_users.status', 0, '<>');
    $query->orderBy('users_users.name', 'ASC');
    $lstig = $query->execute()->fetchAll();
    if (!empty($lstig)) {
        foreach ($lstig as $value) {
            $result[$value->uid] = strtoupper($value->name);
        }
        return $result;
    }
      return $result;
}