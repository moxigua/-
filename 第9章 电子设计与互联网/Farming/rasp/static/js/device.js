
(function($) {
       
	$(function() {
        var baseID;
        var houseID;
        var collectionID;
        var deviceID;
        
        function isRealNum(val){
            if(val === "" || val ==null){
                return false;
            }
            if(!isNaN(val)){
                return true;
            }else{
                return false;
            }
        }      

        function addDevice(id,description,dev_status){
            str="<li class='device'>"+
                "<div>"+
                "<button class='device_open'>开</button>"+
                "<div  class='device_name'><b>"+description+"</b></div>"+
                "<b class='device_id'>"+ id +"</b>"+
                "<button  class='device_close'>关</button>"+
                "</div>"+
                "</li>";
            
            $("#devices_list").append(str);

        }
        
        $(document).ready(function(){
            id = 1;
            dev_status = false;
            description = '加热设备';
            addDevice(id,description,dev_status);  

            id = 2;
            dev_status = false;
            description = '照明设备';
            addDevice(id,description,dev_status);  

            // 开启操作
            $('.device_open').click(
                function(){
                    var e=this;
                    var name = $(e).parents("div").children(".device_name").children('b').text();
                    var id = $(e).parents("div").children(".device_id").text();
                    if(isRealNum(id)){
                        $.post('changeStatus',{"id":id,"status":"open"},function(data){
                                if(data['error_code'] == 0){
                                    showToast({
                                       title:"打开电器成功",
                                       icon:'success',
                                       duration:2000,
                                       mask:true
                                    });
                                }
                                else{
                                    showToast({
                                       title:"打开电器失败",
                                       duration:2000,
                                       mask:true
                                    });
                                    
                                }
                            }
                        ,"json");
                    }
            });
            // 关闭操作
            $('.device_close').click(
                function(){
                    var e=this;
                    var name = $(e).parents("div").children(".device_name").children('b').text();
                    var id = $(e).parents("div").children(".device_id").text();
                    $.post('changeStatus',{"id":id,"status":"close"},function(data){
                        if(data['error_code'] == 0){
                            showToast({
                               title:"关闭电器成功",
                               icon:'success',
                               duration:2000,
                               mask:true
                            });
                        }
                        else{
                            showToast({
                               title:"关闭电器失败",
                               duration:2000,
                               mask:true
                            });
                            
                        }
                    }
                ,"json");
            });

        });
        
	});

})(jQuery);