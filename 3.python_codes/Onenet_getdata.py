import urllib.request
import json
import token_ge


# 产品,设备相关信息
pid = '592693'  #产品ID
deviceId = "1069066433" #设备ID
device_name = 'LightSensor2'    #设备名称
access_key = '????????????????????????????' #产品access_key
sign = token_ge.token_generate(pid, device_name, access_key, 2) #签名结果字符串
#sign = "version=2018-10-31&res=products%2F592693&et=1996892239&method=md5&sign=i%2F4UZUepJfRYF1fgx4iQtA%3D%3D"

#通过HTTPS访问OneNet的API详见https://open.iot.10086.cn/doc/v5/develop/detail/257
#和https://open.iot.10086.cn/doc/book/application-develop/api/api-usage.html
 
# 获取函数
def OneNet_Get_Latestdata():

    """ 通过https访问onenet的API,获取最新数据
    :return: json格式的字符串,包含设备创建时间,data及其当前值,等
    """

    url = "http://api.heclouds.com/devices/" + deviceId + "/datastreams"
    request = urllib.request.Request(url)
    request.add_header('authorization', sign)
    request.get_method = lambda: 'GET'
    request = urllib.request.urlopen(request)
    r = json.loads(request.read())
    data = r.pop('data')
    print(r)
    return data
 
 
if __name__ == '__main__':
    load_data = OneNet_Get_Latestdata()
    print('参数' + '\t\t\t\t\t' + '更新时间' + '\t\t\t' + '数值')
    for index, element in enumerate(load_data):
        a = str(element.get('update_at', ''))
        b = str(element.get('current_value', ''))
        if a != "" and b != "":
            print(str(element['id']) + '\t\t\t\t' + a + '\t\t\t' + b)
            