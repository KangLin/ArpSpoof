功能：
1、欺骗局域网内的所有主机
2、欺骗指定的主机
3、有间隔发送欺骗包
4、捕获数据流
5、提供回调函数处理捕获到的数据

函数：
1、得到本机MAC地址和本机IP
2、得到网内所有主机的IP地址和相应的MAC地址
3、得到指定IP的主机的MAC地址
4、构造ARP请求包
5、构造ARP应答包


编译：
VC++ 6.0 中使用WinPcap 
下载并安装WinPcap，安装之后在目录”C:\WINDOWS\system32“下WinPcap添加了Packet.dll、wpcap.dll。 
增加WinPcap的include和lib路径： 
Tools→Options→Directories，其中include文件的路径增加WinPcap的include路径（其中有pcap.h等头文件），library文件的路径增加WinPcap的lib路径（其中有Packet.lib和wpcap.lib）。 
增加项目的静态链接库： 
Project→Settings→Link→Object/library Modules，在文本框的末尾添加”wpcap.lib packet.lib ws2_32.lib“。 
增加预编译信息： 
Project→Settings→C/C++→Preprocessor definitions，在文本框的末尾添加”WPCAP,HAVE_REMOTE“。 

用windows ip helper api：
ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.chs/iphlp/iphlp/about_ip_helper.htm