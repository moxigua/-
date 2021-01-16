
(function($) {
       
	$(function() {
        var baseID;
        var houseID;
        var collectionID;
        var deviceID;
        
        /*
        // 在网页上动态添加子项目的方法
        function creatASubControler(ControlerTitle,ControlerDescribtion,ControlerState,ControlerID)
        {
            var baseID = currentBaseID;
            var houseID = currentHouseID;
            //具体查看哪种采集点的信息
            if(ControlerState == "1"){
                var src="../../Public/Images/on.png";
            }
            else{
                var src="../../Public/Images/off.png";
            }
            str="<div class='main-controler'>"+
                "<div class='ControlerChangeState'>0</div>"+
                "<ul >"+
                "<li class='controler'><div class='controlerTitle'>"+ControlerTitle+"</div ><div class='controlerState' style='background:url("+'"'+src+'"'+") no-repeat;'></div></li>"+
                "<li class='controlerDescribtion'><div>作用描述： <span>"+ControlerDescribtion+"</span></div></li>"+
                "<div class='controlerID'>"+ControlerID+"</div>"+
                "<div class='controlerStateHidden'>"+ControlerState+"</div>"+
                "</ul></div>";
            $("#main-container").append(str);
        }
        */
        
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

        function refreshCollectionMessages(airTemp,airHum,soilTemp,soilHum,light){
            $("#airTemp").text(airTemp);
            $("#airHum").text(airHum);
            $("#soilTemp").text(soilTemp);
            $("#soilHum").text(soilHum);
            $("#light").text(light);
        }
        
        function getCollectionMessages(){
            $.post('getCollections',null,function(data){
                    try{
                        id = data['id'];
                        if(id==1){
                            airTemp = data['air_temp'];
                            airHum = data['air_hum'];
                            soilTemp = data['soil_temp'];
                            soilHum = data['soil_hum'];
                            light = data['light'];
                            refreshCollectionMessages(airTemp,airHum,soilTemp,soilHum,light);
                        }          
                    }
                    catch(err){
                        //在这里处理错误
                        ;
                    }
                }
                ,"json");
        }
        
        var interval_flag;
        $(document).ready(function(){
            interval_flag = setInterval(getCollectionMessages, 3000);
        });
        
        
        $(document).ready(function(){
           getCollectionMessages(); 
        });
	});

})(jQuery);