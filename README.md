# 4G-OneNET-LightSensor
## 简介

使用STM32F103RCT6和合宙的Air724UG 4G模块做的一个户外光照传感器，并将秒级间隔的光照数据传输至OneNet云端

## 1.目的

​       项目来源是之前需要做一个户外光照采集器，收集一个区域各个采集器点的光照强度。然后预测该区域未来的光强，以及可发电量。这里开源这个光照传感器上云的方案，旨在分享如何使用Air724 4G AT模块，分享如何将数据通过MQTT传输至云端（OneNet），之前看网上的资料确实太少太杂了，搞了好久才做出。同时，也可以使用相关API进行云端的数据获取。后面会给出python写好的代码，使用OneNet的https的API获取设备数据。

## 2.总体框架

​       主控使用STM32F103RCT6；供电部分使用的EG1192，一个Buck电路，同时也加了一个type-C口，直接用5v供电也可以；4G模块使用[合宙Air724 4G模块](https://item.taobao.com/item.htm?spm=a1z09.2.0.0.19a12e8dLvxWYN&id=642798218191&_u=83378prq7b96)；光照传感器使用MAX44009模块，这里就直接买的[JT-I2C1208模块](https://item.taobao.com/item.htm?spm=a21n57.1.0.0.11c3523cVVbKqT&id=679213637980&ns=1&abbucket=11#detail)，它配有防水透明外壳和连接线；实时操作系统使用的FreeRTOS；LCD仅在调试时使用即可；外部EEPROM用来记录手动调参的参数，即有可能MAX44009会有偏移，需要手动调参，这里只使用了y = ky’+b，y为调节后的值，y'为原始值，可以调节k和b来改善偏移。

<div align=center>
<img src=".\5.images\总体框图.png" alt="总体框图" style="zoom:60%;" />
</div>

<div align = "center">图1 总体框图</div>

<div align=center>
<img src=".\5.images\传输框图.png" alt="传输框图" style="zoom:70%;" />
</div>
<div align = "center">图2 传输框图</div>
