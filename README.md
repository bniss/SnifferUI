# SnifferUI
基于MFC和Winpcap库开发的网络抓包软件

## 功能：
* 支持对Ethernet、ARP、IPv4、ICMP、UDP、TCP、DNS、HTTP、DHCP的解析。
* 其中ICMP只支持解析类型为3、4、5、11、8、0的报文，DNS只支持解析类型A、NS、CNAME、PTR的报文，DHCP只能解析选项0、1、3、6、12、50、51、53、54、60、61、255。 

## ToDoList：
* v2.0中DHCP,DNS,HTTP解析的实现
* 根据协议名过滤数据包
* 数据包保存为.pcap格式文件

## Bug：
* 过滤后原来位置的底色保持不变
* DNS协议无法正确解析回答、授权回答、附加信息区域
* 数据包16进制字节流格式不对齐

## v2.0更新说明：
* 代码重构
* 解决内存泄露问题
* 界面更换为WIN7风格

## 运行截图：
### v2.0
<img src="v2.0/img/v2.0_interface_20190131.png" width = 75% height = 75%  />  

### v1.0
<img src="v1.0/img/decode_dns.PNG" width = 75% height = 75%  />  

<img src="v1.0/img/decode_dhcp.png" width = 75% height = 75%  />  

<img src="v1.0/img/decode_http.png" width = 75% height = 75%  />

## 参考资料：
* [一步一步开发sniffer（Winpcap+MFC）（一）工欲善其事，必先配环境——配置winpcap开发环境](https://blog.csdn.net/litingli/article/details/5950962)
* [一步一步开发sniffer（Winpcap+MFC）（二）掀起你的盖头来，让我来画你的脸——用MFC开发GUI](https://blog.csdn.net/litingli/article/details/6098654)
* [一步一步开发sniffer（Winpcap+MFC）（三）安得广厦千万间，先画蓝图再砌砖——搭建winpcap抓包框架](https://blog.csdn.net/litingli/article/details/7315699)
* [一步一步开发sniffer（Winpcap+MFC）（四）要想从此过，留下协议头——各层网络协议头的实现](https://blog.csdn.net/litingli/article/details/7315789)
* [一步一步开发sniffer（Winpcap+MFC）（五）莫道无人能识君，其实我懂你的心——解析数据包](https://blog.csdn.net/litingli/article/details/7315914)
* [一步一步开发sniffer（Winpcap+MFC）（六）千呼万唤始出来，不抱琵琶也露面——将解析数据写到GUI上](https://blog.csdn.net/litingli/article/details/7316173)
* [RFC792 ICMP](https://www.rfc-editor.org/rfc/rfc792.txt)
* [RFC1257 ICMP Router Discovery Messages](https://www.rfc-editor.org/rfc/rfc1256.txt)
* [RFC1035  DOMAIN NAMES - IMPLEMENTATION AND SPECIFICATION](https://www.rfc-editor.org/rfc/rfc1035.txt)
* [RFC2132 DHCP Options and BOOTP Vendor Extensions](https://www.rfc-editor.org/rfc/rfc2132.txt)
