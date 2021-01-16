# coding=UTF-8

import threading
import logging
import time
import re
import sys
import json
from enum import Enum

try:
    import Queue as queue
except ImportError:
    import queue

    
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# 因为查询的是中文字符，因此使用utf-8编码形式
reload(sys)
sys.setdefaultencoding('utf-8')

class Collection:
    

    def __init__(self,expansion = None,db = None,monitor = None):
        
        # 停止标志位
        self.done = False
        
        # 数据库操作
        self.__db = db
        
        # 扩展版函数
        self.__expansion = expansion
        
        # MQTT发布消息
        self.__monitor = monitor
    
    def startCollection(self,daemon = True):
        '''
        开始采集点的采集
        :daemon:采集线程是否随着主线程停止而停止
        :return:
        '''
        try:
            self.done = False         
            t = threading.Thread(target=self.run)
            # 当主循环结束，此线程就会结束
            t.daemon = daemon
            t.start()
        except Exception as e :
            logger.exception(e)
    
    def run(self):
        '''
        串口通信监听和发送的实体
        :return:
        '''
        while not self.done:
            try:
                self.__run()
            except Exception as e:
                logger.exception(e)

    def __run(self):
        '''
        run方法实现
        :return:
        '''
        while not self.done:
            try:
                if self.__db is not None and self.__expansion is not None:
                    houseID = self.__db.getHouseID()
                    # 发送采集的数据
                    self.__expansion.notifyCollectionOld(houseID)
                    time.sleep(8)  # 等待数据采集完成
                    collection_array = json.loads(self.__db.getAllCollections())
                    for c in collection_array:
                        (result,err_code,value) = self.__expansion.Collection_Return_Old(houseID,c['id'],5)
                        if result and self.__monitor is not None:
                            # 获取采集点成功
                            logger.info("get collection({}) successfully".format(c['id']))
                            # 发布 采集点的MQTT
                            self.__monitor.sensorsValuesConfirmation(houseID,c['id'],json.loads(self.__db.getSensorValues(c['id'])))
                        else:
                            # 获取采集点失败
                            logger.info("get collection({}) failed".format(c['id']))
            except Exception as e:
                logger.exception(e)
    
    def stop(self):
        '''
        线程监听停止
        :return:
        '''
        self.done = True
    
    