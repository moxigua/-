# encoding:utf-8

import sys
import datetime
import logging
import json
import sys
import os
import uuid
import threading
import time
import paho.mqtt.client as mqtt
from imp import reload

try:
    import Queue as queue
except ImportError:
    import queue

# 因为查询的是中文字符，因此使用utf-8编码形式
reload(sys)
try:
    sys.setdefaultencoding('utf-8')
except:
    # 在python3中默认编码就是unicode，因此不需要重新设置编码
    pass

logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger(__name__)

# python 2.7.9 之后版本引入了一个新特性：当你urllib.urlopen一个 https 的时候会验证一次 SSL 证书 ，当目标使用的是自签名的证书时就会爆出该错误消息。
# 在树莓派下添加下面命令可以解决改问题
# ssl._create_default_https_context = ssl._create_unverified_context

# MQTT相关设置
username='small_homekeeper/moxigua_make'
password='ey4l3wOPHU9ClMj1I/jZTQmRcpCkWHt6OKJJ9nCMoNw='
hostname='small_homekeeper.mqtt.iot.gz.baidubce.com'

DIRECT_TOPIC = 'direct'
EVENT_TIPIC = 'event'


class DateEncoder(json.JSONEncoder):  
    '''
    JSON 扩展，默认不支持DateTime的扩展
    '''
    def default(self, obj):  
        if isinstance(obj, datetime.datetime):  
            return obj.strftime('%Y-%m-%d %H:%M:%S')  
        elif isinstance(obj, date):  
            return obj.strftime("%Y-%m-%d")  
        else:  
            return json.JSONEncoder.default(self, obj) 


class RemoteMonitor(object):
    def __init__(self,custom_skill = None,db = None):
        '''
        Monitor 初始化
        #return
        '''
        self.namespace = 'Farming.Control'
        self.token = ''
        
        # 自定义技能
        self.skills = custom_skill
        
        # 数据库操作
        self.db = db
        
        # 停止标志位
        self.done = False
        # 创建发送消息的消息队列
        self.send_queue = queue.Queue()

        self.client = mqtt.Client()
        self.client.on_connect = self.__on_connect
        self.client.on_message = self.__on_message
        self.client.username_pw_set(username, password)
    
    def __on_connect(self,client, userdata, flags, rc):
        '''
        设备连接上MQTT服务器的回掉函数
        '''
        logger.info("Connected with result code "+str(rc))
        # 订阅DIRECT_TOPIC,接受客户端上传的指令
        client.subscribe(DIRECT_TOPIC,1)
        # 发布消息
        if self.db is not None:
            self.isAliveConfirmation(self.db.getHouseID())
   
    def __on_message(self,client, userdata, msg):
        '''
        收到消息的回掉函数
        '''
        try:
            logger.info("{} : {}".format(msg.topic,msg.payload))
            self.handleMessage(msg)
        except Exception as e:
            logging.exception(e)
    
    def start(self,daemon = True):
        '''
        开启远程监控的线程
        :param daemon: 远程监控的线程是否随着主线程停止而停止
        :return
        '''
        try:
            self.done = False
            logger.info("start remote monitor.")
            # 设置遗嘱消息
            if self.db is not None:
                payload = self.getOfflineConfirmation(self.db.getHouseID())
                self.client.will_set(EVENT_TIPIC, payload=payload, qos=1, retain=False)
            # 异步连接连接MQTT服务器
            self.client.connect_async(hostname,port = 1883, keepalive = 60)
            # MQTT自动重连
            self.client.loop_start()
            # 接受命令线程
            t = threading.Thread(target=self.monitor_loop)
            # 当主循环结束，此线程就会结束
            t.daemon = daemon
            t.start()
        except Exception as e :
            logger.exception(e)
    
    def monitor_loop(self):
        '''
        远程监控线程实体
        :return:
        '''
        while not self.done:
            try:
                self.__monitor_loop()        
            except Exception as e:
                logger.exception(e)

    def __monitor_loop(self):
        '''
        monitor_loop方法实现
        :return:
        '''
        self.send_queue.queue.clear()
        
        while not self.done:
            # 读取视频播放指令
            try:
                topic,payload,qos = self.send_queue.get(timeout=0.35)
            except queue.Empty:
                topic = None   
            if topic is not None:
               self.client.publish(topic,payload,qos)

    def stop(self):
        '''
        停止所有线程
        '''
        self.done = True
        self.client.loop_stop(force=False)
        self.client.disconnect()
    
    def uploadEvent(self,data,qos):
        '''
        使用MQTT上报数据
        '''
        self.send_queue.put((EVENT_TIPIC,data,qos))
        
    def turnOnConfirmation(self,status,houseID,deviceID,deviceStatusArray):
        '''
        返回请求打开设备命令
        :param status: 打开设备是否成功
        :param houseID:打开设备所在的房间位置信息
        :param deviceID:打开设备的名称
        :param deviceStatusArray: 附加电器设备的状态值
        '''
        event = {
            "header": {
                "namespace": self.namespace,
                "name": "TurnOnConfirmation",
                "messageId": uuid.uuid4().hex,
                "payloadVersion": "1"
            },
            "payload": {
                "appliance": {
                    "houseID": houseID,
                    "deviceID": deviceID,
                    "status":status,
                    "time":datetime.datetime.now()
                },
                "attributes": deviceStatusArray
            }
        }
        str = json.dumps(event,cls=DateEncoder)
        self.uploadEvent(str,1)
        
    def turnOffConfirmation(self,status,houseID,deviceID,deviceStatusArray):
        '''
        返回请求关闭设备命令
        :param status: 打开设备是否成功
        :param houseID:打开设备所在的房间位置信息
        :param deviceID:打开设备的名称
        :param deviceStatusArray: 附加电器设备的状态值
        '''
        event = {
            "header": {
                "namespace": self.namespace,
                "name": "TurnOffConfirmation",
                "messageId": uuid.uuid4().hex,
                "payloadVersion": "1"
            },
            "payload": {
                "appliance": {
                    "houseID": houseID,
                    "deviceID": deviceID,
                    "status":status,
                    "time":datetime.datetime.now()
                },
                "attributes": deviceStatusArray
            }
        }
        str = json.dumps(event,cls=DateEncoder)
        self.uploadEvent(str,1)
        
    def devciesStatusConfirmation(self,houseID,res,deviceStatusArray):
        '''
        返回多个设备的状态
        :param houseID: 房间的ID号
        :param res: 通过串口获取设备电器设备的状态
        :param deviceStatusArray:设备的列表，格式为:  [{'homelocation':'主卧',"deviceName": "灯","status": "on"},{'homelocation':'厨房',"deviceName": "灯","status": "off"}]
        '''
        event = {
            "header": {
                "namespace": self.namespace,
                "name": "DevciesStatusConfirmation",
                "messageId": uuid.uuid4().hex,
                "payloadVersion": "1"
            },
            "payload": {
                "appliance": {
                    "houseID": houseID,
                    "status": res,
                    "time":datetime.datetime.now()
                },
                "attributes": deviceStatusArray
            }
        }
        str = json.dumps(event,cls=DateEncoder)
        self.uploadEvent(str,1)
    
    def sensorsValuesConfirmation(self,houseID,collectionID,sensorsValuesArray):
        '''
        上传传感器的值
        :houseID:温室大棚房间的ID号
        :collectionID:采集点的ID号
        :param sensorsValuesArray:设备的列表，格式为:  [{'type':'airTemp','value':16.5},{'type':'airHum','value':33}]
        '''
        event = {
            "header": {
                "namespace": self.namespace,
                "name": "SensorsValueConfirmation",
                "messageId": uuid.uuid4().hex,
                "payloadVersion": "1"
            },
            "payload": {
                "appliance": {
                    "houseID": houseID,
                    "collectionID":collectionID
                },
                "attributes": sensorsValuesArray
            }
        }
        str = json.dumps(event)
        self.uploadEvent(str,1)
        
    def isAliveConfirmation(self,houseID):
        '''
        本机在先确认消息
        :param houseID: 本机的房间ID
        '''
        event = {
            "header": {
                "namespace": self.namespace,
                "name": "IsAliveConfirmation",
                "messageId": uuid.uuid4().hex,
                "payloadVersion": "1"
            },
            "payload": {
                "appliance": {
                    "houseID": houseID
                }
            }
        }
        str = json.dumps(event)
        self.uploadEvent(str,1)
    
    def offlineConfirmation(self,houseID):
        '''
        离线通知
        :param houseID: 本机的房间ID
        '''
        event = {
            "header": {
                "namespace": self.namespace,
                "name": "OfflineConfirmation",
                "messageId": uuid.uuid4().hex,
                "payloadVersion": "1"
            },
            "payload": {
                "appliance": {
                    "houseID": houseID
                }
            }
        }
        str = json.dumps(event)
        self.uploadEvent(str,1)
    
    def getOfflineConfirmation(self,houseID):
        '''
        返回离线通知的字符串
        :param houseID: 本机的房间ID
        '''
        event = {
            "header": {
                "namespace": self.namespace,
                "name": "OfflineConfirmation",
                "messageId": uuid.uuid4().hex,
                "payloadVersion": "1"
            },
            "payload": {
                "appliance": {
                    "houseID": houseID
                }
            }
        }
        str = json.dumps(event)
        return str
    
    def NotifyDevicesStatusToRemoteForButtonsPressed(self):
        '''
        因为操作按钮被按下了，因此需要本地主动推送推送继电器的状态
        :return :
        '''
        logger.info("Notify Devices Status To Remote For Buttons Pressed.")
        self.devciesStatusConfirmation(self.db.getHouseID(),True,json.loads(self.db.getAllDevices()))
    
    def handleMessage(self,msg):
        '''
        处理接受到的json信息
        :param msg:接受的json字符串
        :return
        '''
        try:
            if msg.topic == 'direct':
                utf8_payload = "{}".format(msg.payload)
                utf8_payload = utf8_payload.decode("gb2312").encode()
                utf8_payload = utf8_payload.decode('utf-8')
                jsonObj = json.loads(utf8_payload)
                if jsonObj is not None:
                    if 'header' in jsonObj:
                        header = jsonObj['header']
                        namespace = header['namespace']
                        name = header['name']
                        payloadVersion = header['payloadVersion']
                        messageId = header['messageId']
                        if payloadVersion == '1':
                            if namespace == self.namespace:
                                if 'payload' in jsonObj:
                                    if 'appliance' in jsonObj['payload']:                                       
                                        if name == 'TurnOnRequest':
                                            # 打开设备
                                            logger.info("received TurnOnRequest.")
                                            if self.skills is not None and self.db is not None and hasattr(self.skills,'openSingleKA'):
                                                if 'houseID' in jsonObj['payload']['appliance'] and 'deviceID' in jsonObj['payload']['appliance']:
                                                    # 判断下houseID
                                                    houseID = jsonObj['payload']['appliance']['houseID']
                                                    if houseID == self.db.getHouseID():
                                                        deviceID = jsonObj['payload']['appliance']['deviceID']
                                                        slave_addr = deviceID//16+1
                                                        device_id = deviceID%16
                                                        (res,error_code,values) = self.skills.openSingleKA(slave_addr,device_id)
                                                        self.turnOnConfirmation(res,houseID,deviceID,json.loads(self.db.getAllDevices()))
                                        elif name == 'TurnOffRequest':
                                            # 关闭设备
                                            logger.info("received TurnOffRequest.")
                                            if self.skills is not None and self.db is not None and hasattr(self.skills,'closeSingleKA'):
                                                if 'houseID' in jsonObj['payload']['appliance'] and 'deviceID' in jsonObj['payload']['appliance']:
                                                    # 判断下houseID
                                                    houseID = jsonObj['payload']['appliance']['houseID']
                                                    if houseID == self.db.getHouseID():
                                                        deviceID = jsonObj['payload']['appliance']['deviceID']
                                                        slave_addr = deviceID//16+1
                                                        device_id = deviceID%16
                                                        (res,error_code,values) = self.skills.closeSingleKA(slave_addr,device_id)
                                                        self.turnOffConfirmation(res,houseID,deviceID,json.loads(self.db.getAllDevices()))
                                                    else:
                                                        logger.info("houseID is wrong")
                                                else:
                                                    logger.info("houseID or deviceID is None")
                                            else:
                                                logger.info("skills or db is None.")
                                        elif name == 'DevicesStatusRequest':
                                            # 请求设备状态
                                            logger.info("received DevicesStatusRequest.")
                                            if self.skills is not None and self.db is not None:
                                                if 'houseID' in jsonObj['payload']['appliance']:
                                                    # 判断下houseID
                                                    houseID = jsonObj['payload']['appliance']['houseID']
                                                    if houseID == self.db.getHouseID():
                                                        if self.skills is not None and hasattr(self.skills,'readOutputValues'):
                                                            (res,error_code,values) = self.skills.readOutputValues()
                                                            self.devciesStatusConfirmation(houseID,res,json.loads(self.db.getAllDevices()))
                                                        else:
                                                            self.devciesStatusConfirmation(houseID,False,json.loads(self.db.getAllDevices()))
                                                    else:
                                                        logger.info("houseID is wrong")
                                                else:
                                                    logger.info("houseID  is None")
                                            else:
                                                logger.info("skills or db is None.")
                                                
                                        elif name == 'SensorsValuesRequest':
                                            # 请求传感器的值
                                            logger.info("received SensorsValuesRequest.")
                                            if self.skills is not None and self.db is not None:
                                                if 'houseID' in jsonObj['payload']['appliance'] and 'collectionID' in jsonObj['payload']['appliance']:
                                                    # 判断下houseID
                                                    houseID = jsonObj['payload']['appliance']['houseID']
                                                    if houseID == self.db.getHouseID():
                                                        collectionID = jsonObj['payload']['appliance']['collectionID']
                                                        self.sensorsValuesConfirmation(houseID,collectionID,json.loads(self.db.getSensorValues(collectionID)))
                                                    else:
                                                        logger.info("houseID is wrong")
                                                else:
                                                    logger.info("houseID or collectionID is None")
                                            else:
                                                logger.info("skills or db is None.")
                                        elif name == 'IsAliveRequest':
                                            # 请求是否在先
                                            logger.info("received SensorsValuesRequest.")
                                            if self.skills is not None and self.db is not None:
                                                if 'houseID' in jsonObj['payload']['appliance']:
                                                    # 判断下houseID
                                                    houseID = jsonObj['payload']['appliance']['houseID']
                                                    if houseID == self.db.getHouseID():
                                                        self.isAliveConfirmation(houseID)
                                                    else:
                                                        logger.info("houseID is wrong")
                                                else:
                                                    logger.info("houseID is None")
                                            else:
                                                logger.info("skills or db is None.")
                                        else:
                                            logger.info("mqtt received unkown json data.")
                                    else:
                                        logger.info("json string must contain \"appliance\".")
                                else:
                                    logger.info("json string must contain \"payload\".")
                        else:
                            logger.info("unsupport the payload version: {}".format(payloadVersion))
                    else:
                        logger.info("json must combine header.")
                else:
                    logger.info("data must be json type.")
        except Exception as e:
            logging.exception(e)
            
            
            

            
# arrayJsonString = [{'type':'airTemp','value':16.5},{'type':'airHum','value':33}]
# deviceStatusArray = [{'homelocation':'主卧',"deviceName": "灯","status": "on"},{'homelocation':'厨房',"deviceName": "灯","status": "off"}]
# monitor = Monitor()
# monitor.start(daemon=False)
# time.sleep(5)
# monitor.sensorsValuesConfirmation(arrayJsonString)
# monitor.devciesStatusConfirmation(deviceStatusArray)
# time.sleep(5)
# monitor.stop()



    
    
    

