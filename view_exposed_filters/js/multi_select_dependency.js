(function ($) {
    Drupal.behaviors.views_exposed_form = {
        attach: function (context, settings) {
            var selected_ids = new Array();
            var hidden_filters = new Array();
            $('#edit-controller-filter').chosen().each(function() {
              var values = $("#edit-controller-filter").chosen().val();
                if(values) {
                    $.each(values, function(key, value) {
                        var str = value.split('_');
                        selected_ids[key] = 'edit-'+str.join('-')+'-wrapper';
                        $('#edit-'+str.join('-')+'-wrapper').css('display', 'block');
                    });    
                }
                
            });
            console.log(selected_ids);
            $('.views-exposed-widget:not(.views-submit-button, .views-widget-select_fields, .views-widget-filter-controller_filter)').each( function (index) {
                if($.inArray($(this).attr('id'), selected_ids) == -1 && $(this).attr('id') != 'edit-controller-filter-wrapper' && $(this).attr('id') != 'undefined') {
                   hidden_filters[index] = $(this).attr('id');
                }
            });
            
            $.each(hidden_filters, function(key, value) {
                $('#'+value).css('display', 'none');
                $('#'+value+ ' select option:selected').removeAttr('selected');
                $('#'+value+ ' inpput[type=text]').val('');
            });

            $('#edit-controller-filter').chosen().change( function (e, params) {
                var values = $("#edit-controller-filter").chosen().val();
                if(values) {
                    for(var i = 0; i< values.length; i++) {
                      var str = values[i].split('_');
                      $('#edit-'+str.join('-')+'-wrapper').css('display', 'block');
                    }
                }
                var removed = params.deselected;
                
                if(removed) {                    
                    var rstr = removed.split('_');
                    $('#edit-' + rstr.join('-') + '-wrapper').css('display', 'none');
                    $('#edit-' + rstr.join('-') + '-wrapper li.search-choice a.search-choice-close').each( function () {
                        $(this).trigger('click');
                    });
                    $('#edit-'+ rstr.join('-') + '-wrapper select option:selected').removeAttr('selected');
                    if($('#edit-'+ rstr.join('-')).is('input:text')) {
                        $('#edit-'+ rstr.join('-')).val('');
                    }
                }
            });
        }
    };
})(jQuery);