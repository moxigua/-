var showToast=function(object) {
    let tipTxt=object.title;
    let time;
    if(!object.duration){
        time=1500;
    }
    else{
        time=object.duration
    }
    let mask;
    if(!object.mask){
        mask=false
    }
    else{
        mask=object.mask
    }
    let icon;
    if(!object.icon){
        icon="none"
    }
    else{
        icon=object.icon;
    }
    let image;
    if(object.image){
        image=object.image;
    }
    let success = object.success;
    let fail = object.fail;
    let complete = object.complete;
    var result;
    try{
        var viewportID=document.getElementById("viewport");
        if(!viewportID){
            var oMeta = document.createElement('meta');
            oMeta.content = 'width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0;';
            oMeta.name='viewport';
            oMeta.id='viewport';
            document.getElementsByTagName('head')[0].appendChild(oMeta);
        }
        $(".xsw_toast").remove();
        var popToastHtml="";
        popToastHtml += `<div class="xsw_showToast"> `;
        if(mask==true){
            popToastHtml += `<div class="xsw_bei" style="position: fixed;width: 100%;height: 100%;top: 0;left: 0;background-color: #262626;opacity: 0.4;z-index: 999999999999999999999"></div>`
        }
        popToastHtml += `<div class="pop-toast" style="position: fixed;  width: 140px;  height: 140px;  text-align: center;background-color: #555;border-radius: 10px;box-shadow: 0 2px 8px #555 ;right: 50%;top: 50%;margin: -70px -70px 0 0;z-index: 9999999999999999999999">`;
        if(image){
            popToastHtml += `<img src="${image}" style="width: 50px;margin-top: 12px;">`
            popToastHtml += ` <div class="toast-tip" style=" font-size: 16px;  color: #fff;  height: 45px;overflow: hidden;width:130px; word-wrap: break-word; text-align: center;padding: 0 5px ;margin-bottom: 10px;">${tipTxt}</div>
                              </div></div>`;
        }
        else{
            if(icon=="none"){
                popToastHtml += ` <img  style="width: 50px;margin-top: 12px;display: none">`
                popToastHtml += ` <div class="toast-tip" style=" font-size: 16px;  color: #fff;  height: 45px;overflow: hidden;width:130px; word-wrap: break-word; text-align: center;padding: 0 5px ;margin-top: 45px; ">${tipTxt}</div>
                              </div></div>`;
            }
            else {
                if(icon=="success"){
                    popToastHtml += `<canvas id="x_canvas" width="140px" height="50px"></canvas>`;
                }
                else if(icon=="loading"){
                    popToastHtml += `<div id="xsw_canvas" style="140px;height: 60px;"></div>`;
                }
                popToastHtml += ` <div class="toast-tip" style=" font-size: 16px;  color: #fff;  height: 45px;overflow: hidden;width:130px; word-wrap: break-word; text-align: center;padding: 0 5px ;margin-bottom: 10px;margin-top: 20px;">${tipTxt}</div>
                              </div></div>`;
            }

        }
        $("body").append(popToastHtml);
        $("body").css({"position":"relative"});
        if(icon=="success"){
            /////////////////////////////////////////////////画勾
            var cvs = document.getElementById('x_canvas'); //画布
            var ctx = cvs.getContext('2d'); // 画笔
            ctx.lineWidth  = 4;//线宽
            ctx.strokeStyle="#fff";//线的颜色
            ctx.moveTo( 55, 35 );
            ctx.lineTo( 65, 45 );
            ctx.lineTo( 90, 15 );
            ctx.stroke();
        }
        else if(icon=="loading"){
            /////////////////////////////////////////////////
            loading();
        }
        /////////////////////////////////////////////////
        if(time!="" || time!=0){
            setTimeout(function () {
                $("#viewport").remove();
                $(".xsw_showToast").remove();
            }, time);
        }
        result = {showToast:"ok"};
        if(success){
            success(result);
        }
        if(complete){
            complete(result);
        }

    }catch(e) {
        result = {errMsg:e.message};
        if(fail){
            fail(result);
        }
        if(complete){
            complete(result);
        }
    }
};

var showLoading=function (object) {
    let tipTxt=object.title;
    let mask;
    if(!object.mask){
        mask=false
    }
    else{
        mask=object.mask
    }
    let success = object.success;
    let fail = object.fail;
    let complete = object.complete;
    var result;
    try{
        var viewportID=document.getElementById("viewport");
        if(!viewportID){
            var oMeta = document.createElement('meta');
            oMeta.content = 'width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0;';
            oMeta.name='viewport';
            oMeta.id='viewport';
            document.getElementsByTagName('head')[0].appendChild(oMeta);
        }
        $(".xsw_toast").remove();
        var popToastHtml="";
        popToastHtml += `<div class="xsw_showLoading"> `;
        if(mask==true){
            popToastHtml += `<div class="xsw_bei" style="position: fixed;width: 100%;height: 100%;top: 0;left: 0;background-color: #262626;opacity: 0.4;z-index: 999999999999999999999"></div>`
        }
        popToastHtml += `<div class="pop-toast" style="position: fixed;  width: 140px;  height: 140px;  text-align: center;background-color: #555;border-radius: 10px;box-shadow: 0 2px 8px #555 ;right: 50%;top: 50%;margin: -70px -70px 0 0;z-index: 9999999999999999999999">`;
        popToastHtml += `<div id="xsw_canvas" style="140px;height: 60px;"></div>`;
        popToastHtml += ` <div class="toast-tip" style=" font-size: 16px;  color: #fff;  height: 45px;overflow: hidden;width:130px; word-wrap: break-word; text-align: center;padding: 0 5px ;margin-bottom: 10px;margin-top: 20px;">${tipTxt}</div>
                              </div></div>`;
        $("body").append(popToastHtml);
        $("body").css({"position":"relative"});
        loading();
        result = {showLoading:"ok"};
        if(success){
            success(result);
        }
        if(complete){
            complete(result);
        }

    }catch(e) {
        result = {errMsg:e.message};
        if(fail){
            fail(result);
        }
        if(complete){
            complete(result);
        }
    }
};

var hideToast=function () {
    $("#viewport").remove();
    $(".xsw_showToast").remove();
}

var hideLoading=function () {
    $("#viewport").remove();
    $(".xsw_showLoading").remove();
}

var showModal=function (object) {
    let title=object.title;
    let content=object.content;
    let showCancel=object.showCancel;
    let cancelColor=object.cancelColor;
    let confirmColor=object.confirmColor;
    let cancelText;
    if(!object.cancelText){
        cancelText="取消";
    }
    else{
        cancelText= object.cancelText;
    }
    let confirmText;
    if(!object.confirmText){
        confirmText="确定";
    }
    else{
        confirmText=object.confirmText;
    }
    let success = object.success;
    let fail = object.fail;
    let complete = object.complete;
    var result;
    try{
        var viewportID=document.getElementById("viewport");
        if(!viewportID){
            var oMeta = document.createElement('meta');
            oMeta.content = 'width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0;';
            oMeta.name='viewport';
            oMeta.id='viewport';
            document.getElementsByTagName('head')[0].appendChild(oMeta);
        }
        $(".xsw_showModa").remove();
        var showModalHtml="";
        showModalHtml+= `<div class="xsw_showModa" >`
        showModalHtml += `<div class="xsw_showModaBei" style="position: fixed;width: 100%;height: 100%;top: 0;left: 0; background-color: #333;opacity: 0.4;z-index: 999999999999999999999"></div>`
        showModalHtml+=`<div class="xsw_modal-data" style="width:220px;  border-radius: 10px;  top: 50%;  left: 50%;  box-shadow: 0 2px 8px #555 ;  background-color: #fff;  border:1px solid #000;  padding:15px 0;  text-align:center;  z-index: 9999999999999999999999;  position: fixed;">
                                    <div style="text-align: center;font-size: 18px;font-weight: 400">${title}</div><br>
                                    <div style="text-align: center;font-size: 14px;margin-bottom: 30px;">${content} </div><br>`;
        if(showCancel==false){
            showModalHtml+=         `<div style="position: absolute;bottom: 5px;"><div class="yesDian" style="width: 110px;display: inline-block;margin-left: 55px;background-color: gold;cursor: pointer;border-radius: 10px;padding: 8px 0;">${confirmText}</div></div>`
        }
        else{
            showModalHtml+=         ` <div style="position: absolute;bottom: 0px;"><div class="noDian" style="width: 110px;display: inline-block;margin-bottom: 0;background-color: brown;cursor: pointer;border-radius: 0 0 0 10px;padding: 8px 0;color: #000000">${cancelText}</div><div class="yesDian" style="width: 110px;display: inline-block;background-color: gold;cursor: pointer;border-radius: 0 0 10px 0;padding: 8px 0;">${confirmText}</div></div>`
        }
        `></div></div>`;
        $("body").append(showModalHtml);
        if(cancelColor){
            $('.noDian').css({'color':cancelColor})
        }
        if(confirmColor){
            $('.yesDian').css({'color':confirmColor})
        }
        var outerWidth=-$('.xsw_modal-data').outerHeight()/2;
        var modal =document.querySelector('.xsw_modal-data');
        modal.style.margin=outerWidth+'px 0 0 -110px';
        /* $('.xsw_modal-data').css({'margin-left:','"+outerWidth+"'})*/
        $("body").css({"position":"relative"});
        $('.noDian').click(function () {
            $("#viewport").remove();
            $(".xsw_showModa").remove();
            result={
                cancel:true,
                showModal:"ok"
            };
            if(success){
                success(result);
            }
            if(complete){
                complete(result);
            }
        });
        $('.yesDian').click(function () {
            $("#viewport").remove();
            $(".xsw_showModa").remove();
            result={
                confirm:true,
                showModal:"ok"
            };
            if(success){
                success(result);
            }
            if(complete){
                complete(result);
            }
        });
    }
    catch(e) {
        result = {errMsg:e.message};
        if(fail){
            fail(result);
        }
        if(complete){
            complete(result);
        }
    }
};

var showActionSheet = function (object) {
    let device_width=window.screen.height;
    let itemList=object.itemList;
    let itemColor=object.itemColor;
    let success = object.success;
    let fail = object.fail;
    let complete = object.complete;
    var index;
    var result;
    try{
        var viewportID=document.getElementById("viewport");
        if(!viewportID){
            var oMeta = document.createElement('meta');
            oMeta.content = 'width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0;';
            oMeta.name='viewport';
            oMeta.id='viewport';
            document.getElementsByTagName('head')[0].appendChild(oMeta);
        }
        $(".xsw_showActionSheet").remove();
        var showActionSheetHtml="";
        showActionSheetHtml+=`<div class="xsw_showActionSheet" >
                                    <div class="xsw_showActionSheetBei" style="position: fixed;width: 100%;height: 100%;top: 0;left: 0; background-color: #333;opacity: 0.4;z-index: 999999999999999999999"></div>
                                    <div class="xsw_showActionSheet-data" style="width:220px;  border-radius: 5px;  top: 50%;  left: 50%;  box-shadow: 0 2px 8px #555 ;  background-color: #fff;  border:1px solid #000; text-align:center;  z-index: 9999999999999999999999;  position: fixed;color: #000000">
                                        <div style="text-align: left">`;
        var xsw_length;
        if(itemList.length>6){
            xsw_length=6;
        }
        else{
            xsw_length=itemList.length;
        }
        for(var xsw_i=0;xsw_i<xsw_length;xsw_i++){
            showActionSheetHtml+=`<div class="xsw_list" style="border-bottom: 1px solid #cccccc;padding: 10px 0 10px 10px;">${itemList[xsw_i]}</div>`
        }
        showActionSheetHtml+=   `</div></div></div>`;
        $("body").append(showActionSheetHtml);
        var outerWidth=-$('.xsw_showActionSheet-data').outerHeight()/2;
        var modal =document.querySelector('.xsw_showActionSheet-data');
        modal.style.margin=outerWidth+'px 0 0 -110px';
        $('.xsw_list:last').css({'border':"none"})
        if(itemColor){
            $('.xsw_showActionSheet-data').css({'color':itemColor})
        }
        $('.xsw_list').click(function () {
            var thisHtml=$(this).html();
            for(var x in itemList ){
                if(thisHtml==itemList[x]){
                    index=x;
                }
            }
            result={
                showActionSheet:"ok",
                tapIndex:index
            };
            if(success){
                success(result);
            }
            if(complete){
                complete(result);
            }
        });
        $(".xsw_showActionSheet").click(function () {
            $("#viewport").remove();
            $(".xsw_showActionSheet").remove();
        });
    }
    catch(e) {
        result = {errMsg:e.message};
        if(fail){
            fail(result);
        }
        if(complete){
            complete(result);
        }
    }
}

var zhuan;
function loading() {
    clearInterval(zhuan);
    $('#xsw_canvas').html('');
   $('#xsw_canvas').append("<canvas id='xsw_myCanvas' style='margin-left: -80px;margin-top: -30px;'></canvas>");
    var c=document.getElementById("xsw_myCanvas");
    var ctx=c.getContext("2d");
    var lg = ctx.createLinearGradient(180,100,100,20); //可以尝试改变这里的参数，你可以获取更多
    lg.addColorStop(0, '#666');
    lg.addColorStop(1, '#fff');
    ctx.lineWidth=5;
    ctx.strokeStyle = lg;
    ctx.beginPath();
    ctx.arc(150,75,25,0,1.5*Math.PI);
    ctx.stroke();
    var zzz=0;
    zhuan=setInterval(function () {
        zzz=zzz+5;
        $('#xsw_myCanvas').css({'transform':'rotate('+zzz+'deg)'});
    },20);
}

