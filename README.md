# crispy-octo-umbrella
This project decodes the GK2A satellite signal into VCDU for further processing.

Huge thanks to **Lucas Teske's** [blog series](https://lucasteske.dev/satcom-projects/satellite-projects) and [code](https://github.com/racerxdl/open-satellite-project/blob/225a36d4144c0fe0704eb50a8fbc428914f654c0/GOES/network/decoder_tcp.c). 

以及**善麟**提供的前期数据处理`data/rec_128`输入！

This repo is almost a fork of Lucas' work with little changes. Together with a detailed procedure, I will guide you through the necessary steps to run this project on a Windows 10 computer.



## 关于本repo

本repo从`data/rec_128`中读入GK2A卫星的 8-bit soft decision 比特流，进行帧对齐、Viterbi解码、Reed-Solomon纠正后输出 896-byte VCDU。输出文件存放在`vcdu`目录下，命名方式为`virtualChannelID_virtualChannelFrameCount`。详见[AOS Space Data Link Protocol](https://public.ccsds.org/Pubs/732x0b3e1.pdf) 4.1.2节。

目前还没有接入TCP进行实时解码。因此数据处理循环是在`decode.c`第190行定义的。

另外，`makefile_libfec`是供libfec库使用的。详见 [安装libfec库](#安装libfec库)。



## Clone repo到本地

*<u>**请确保本地目录中没有中文字符和空格。**</u>*



## 环境配置

本repo使用到libfec，因此编译和运行需要进行以下配置：

1. [安装Cygwin及编译依赖库](#安装Cygwin及编译依赖库)
2. [安装libfec](#安装libfec)
3. [使用本程序](#使用本程序)



### 安装Cygwin及编译依赖库

首先在Cygwin官网上下载安装包。过程中不需要改变默认选项。

Choose A Download Site时选择一个离您较近的网站。

接着会弹出Select Packages的窗口，请选择Full，输入要查询的package名称，并点击对应的package行的三角形选择要安装的版本，最后单击Next。我们本次需要安装`gcc-core, make, chere`这三个packages。

安装完成后不要删除安装包，未来可能还需要用到它来增减package。

结束之前，记得在最后一页里勾选把Cygwin添加到开始菜单。至此就完成了Cygwin相关的安装。

为了方便调试，我们打开Cygwin命令行后输入

```shell
chere -i -t mintty -s bash
```

这样我们就可以在工作的目录右键，并用Cygwin打开当前目录了！



### 安装libfec

本repo中，我们使用了移植到x86_64上的[libfec](https://github.com/quiet/libfec)——一个非常经典的FEC（Forward Error Correction）库。具体安装过程如下：

首先将上述库下载到本地（*<u>注意：目录中**不要出现中文字符和空格**</u>*），再用本repo的`makefile_libfec`替换它原本的文件，并**重命名为`makefile`**。最后用Cygwin打开当前目录，执行以下指令：

```shell
./configure
make
make test （可以不执行，若执行应该不会报错）
make install
```

完成安装。



### 使用本程序

首先在`makefile`中配置一下libfec的目录。用文本编辑器打开`makefile`，修改`LIB_DIR_WIN`的值为libfec的目录：

```makefile
# 假设libfec在您电脑中的安装目录为 E:\libfec
LIB_DIR_WIN := e/libfec
```

<u>***注意：请勿在磁盘名前加斜杠，且斜杠为 / 而非 \\！***</u>

用Cygwin打开当前目录，使用以下指令：

​	`make`：生成decode.exe。

​	`make clean`：删除decode.exe。



## 有问题？

欢迎提交issue :+1:！



