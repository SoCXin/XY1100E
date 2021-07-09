## [Embedded](https://github.com/SoCXin/XY1100/releases/download/1.0.0/Embedded.7z)

Embedded压缩包文件用于构建编译环境，通过修改SDK文件中的build.bat脚本指向Embedded解压缩文件，可以快速构建起window下的开发环境。

编译后文件使用的注意事项：

* 通过boot按键(GPIO_10)断电再上电进入下载模式
* 如果一直是 Uart Receive Timeout，可能是你的串口工具问题


## [Logview_mini](https://github.com/SoCXin/XY1100/releases/download/1.0.0/Logview_mini.zip)

Logview用于调试信息答应和固件下载，相关使用说明在压缩包中。

QS-100模组相关串口使用注意事项：

* 下载使用的AT主串口RXD/TXD，答应日志信息是DTXD/DRXD
* 下载固件和打印日志使用的波特率921600，注意USB转串口的质量
* AT固件默认波特率9600

### [探索芯世界 www.SoC.xin](http://www.SoC.Xin)
