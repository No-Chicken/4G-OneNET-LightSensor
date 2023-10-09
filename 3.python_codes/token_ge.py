import base64
import hmac
import time
from urllib.parse import quote

def token_generate(pid,device_name,access_key,mode):

    """ 计算出对onenet进行访问的token值,这是用于onenet的安全鉴权
    :pid: 产品ID号
    :device_name: 产品名称,mode=3时才需要
    :access_key: mode=2时, key为产品的access_key. mode=3时, key为设备的key
    :mode: token生成模式选择, 1为MQ_ID访问MQ, 2为通过产品ID通过HTTPS访问onenet的API, 3为设备通过MQTT连云
    :return: 字符串token值
    """

    version = '2018-10-31'

    # 通过MQ_ID访问MQ
    if mode == 1:
        res = 'mqs/%s' % pid 
    
    # 通过产品ID访问产品API
    if mode == 2:
        res = 'products/%s' % pid 

    # 设备通过MQTT访问
    if mode == 3: 
        res = 'products/%s/devices/%s' %(pid,device_name) 

    # 用户自定义token过期时间
    et = '1996892239'

    # 签名方法，支持md5、sha1、sha256
    method = 'md5'

    # 对access_key进行decode
    key = base64.b64decode(access_key)

    # 计算sign
    org = et + '\n' + method + '\n' + res + '\n' + version
    sign_b = hmac.new(key=key, msg=org.encode(), digestmod=method)
    sign = base64.b64encode(sign_b.digest()).decode()

    # value 部分进行url编码，method/res/version值较为简单无需编码
    sign = quote(sign, safe='')
    res = quote(res, safe='')

    # token参数拼接
    token = 'version=%s&res=%s&et=%s&method=%s&sign=%s' % (version, res, et, method, sign)

    return token


if __name__ == '__main__':

    pid = '592693'
    #mode=2,访问onenet的API时,device_name参数不用可以不管.仅在mode=3,MQTT访问时要用这个
    device_name = 'LightSensor2'
    #这个key如果使用MQTT进行设备连接时,用的是设备的key.如果是访问API用的是产品的access_key
    key = '?????????????????????????' 

    print(token_generate(pid,device_name,key,2))