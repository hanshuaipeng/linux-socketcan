socketcan测试，基于am335x

设置can
ip link set can1 type can bitrate 100000  triple-sampling on

启动can
ip link set can1 up

关闭can
ip link set can1 down

can接收
candump can1

参考https://blog.csdn.net/lybhit/article/details/78663347


