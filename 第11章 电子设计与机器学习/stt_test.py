
from aip import AipSpeech
import os

''' APPID AK SK  可在百度云平台上自主申请'''
APP_ID = '22779964'
API_KEY = 'eC76mRT7SWULhFsETQ2LMaCn'
SECRET_KEY = 'LSQGbpcSVedyyEO0ER1i23G2IQPLtWQM'


# 读取文件
def get_file_content(test):   #filePath  待读取文件名
    with open(test, 'rb') as fp:
        return fp.read()

def stt(test):        
    # 语音识别实例
    client = AipSpeech(APP_ID, API_KEY, SECRET_KEY)
    result = client.asr(get_file_content(test),
            'wav',
            8000,#16000
            # dev_pid参数表示识别的语言类型 1536表示普通话
            {'dev_pid': 1537,})
    # 解析返回值，打印语音识别的结果
    print(result)
    if result['err_msg']=='success.':
        word = result['result'][0].encode('utf-8')       # utf-8编码
        if word!='':
            print(word)
        else:
            print ("音频文件不存在或格式错误")
    else:
        print ("错误")

# main函数 识别本地录音文件yahboom.wav
if __name__ == '__main__':
    stt('temp.wav')

