# -*- coding: utf-8 -*-
from flask import Flask
from flask import render_template
from flask import request
import logging
import time
import json

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

app = Flask(__name__)

# 定义环境参数
air_temp = 0
air_hum = 0
soil_temp = 0
soil_hum = 0
light = 0


@app.route('/')
def hello_world():
    return render_template('index.html')  # 默认页面为采集节点的信息
    
@app.route('/collections')
def collections():
    return render_template('index.html')  # 显示采集节点的信息
    
@app.route('/devices')
def devices():
    return render_template('devices.html') # 显示设备控制的界面
    
@app.route('/changeStatus',methods=['POST','GET']) # 改变设备的状态
def changeStatus():
    try:
        id = request.form.get('id')
        status = request.form.get('status')
        print("id is {},status is {}".format(id,status))
        if id.isdigit():
            #  执行 开启 或 关闭 操作
            error_code = 0x0
            ret = {'error_code':error_code}
        else:
            error_code = 0x81
            ret = {'error_code':error_code}
        return  json.dumps(ret)
    except Exception as e:
        error_code = 0x82
        ret = {'error_code':error_code}
        return False

@app.route('/getCollections',methods=['POST','GET']) # 获取采集节点的信息
def getCollections():
    global air_temp
    global air_hum 
    global soil_temp 
    global soil_hum 
    global light 
    global co2 
    data = {
            "id":1,
            "air_temp":air_temp,
            "air_hum":air_hum,
            "soil_temp":soil_temp,
            "soil_hum":soil_hum,
            "light":light,
            "refresh_time":time.time()
        }
    return json.dumps(data)
 
@app.route('/setCollections',methods=['POST','GET']) # 获取采集节点的信息
def setCollections():
    global air_temp
    global air_hum 
    global soil_temp 
    global soil_hum 
    global light 
    global co2 
    try:
		rec_data = json.loads(request.data)
		air_temp = rec_data['air_temp']
		air_hum = rec_data['air_hum']
		soil_temp = rec_data['soil_temp']
		soil_hum = rec_data['soil_hum']
		light = rec_data['light']
		print("air_temp is {},air_hum is {}".format(air_temp,air_hum))
		error_code = 0x0
		ret = {'error_code':error_code}
    except Exception as e:
        error_code = 0x82
        ret = {'error_code':error_code}
    return json.dumps(ret)

if __name__ == '__main__':

    # 运行flask
    app.run(host='0.0.0.0')
    


    
    