
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
       
	});

})(jQuery);