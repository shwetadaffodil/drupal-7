<?php
/**
 * My custom filter handler
 */
class view_form_handler_filter_filterby_reportinguser extends views_handler_filter_in_operator
{

    /**
     * Override parent get_value_options() function.
     *
     * @return
     *   Return the stored values in $this->value_options if someone expects it.
     */
    function get_value_options() 
    {

        $users = select_reporting_users(6);
        $this->value_options = $users;

        return $users;
    }
  
    function query() 
    {
        $this->ensure_my_table();
        $real_field_name = $this->real_field;
        $value = $this->view->exposed_data[$real_field_name];
    
        if ($value != 0) {
            $this->query->add_where_expression(
                $this->options['group'],
                "field_parent_node_reference_node__field_data_field_company_subuser_refernce.field_company_subuser_refernce_target_id = :uid",
                array(':uid' => $value)
            );
        }
    }
}

function select_reporting_users($role_id = null) 
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
    $query->condition('users_users.uid', $user->uid, '=');
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