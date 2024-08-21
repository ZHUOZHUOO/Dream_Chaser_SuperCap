# 超级电容仓库介绍
本仓库包含了超级电容的PCB文件以及代码文件

##代码部分只上传稳定版本，不要随意更新##
## PCB
    包含超级电容组、超级电容BOOST、BUCK板的各个版本的PCB文件

    保留了早期开发版本的文件，如BOOST V2等

## CODE
    包含各个版本的代码，其中各版本代码只能适配于其对应的或修改过的PCB，烧录代码前请检查该版本对应的PCB

    Super_Cap_BUCK_V3  古老的代码，主要撰写人为罗益善，适用于STM32F4系列主控
    对应“SuperCapBuck_STM32F405”

    Super_Cap_BUCK_V3  2023赛季的代码，主要撰写人为 王晋达、罗益善，适用于STM32G431主控
    对应“SuperCapBuck_STM32G431_V1”
    #代码上车稳定性不够，不建议上车使用#

    Super_Cap_BUCK_V4  2024赛季的代码，主要撰写人为赖卓群、王晋达，适用于STM32G431主控
    对应“SuperCapBuck_STM32G431_V1”
    #只保留了代码效果稳定的V4.3及其衍生版本#
    #但V4.3版本目前优化了ADC采样需要短接DG、GND#

    Super_Cap_BUCK_V5  主要撰写人为赖卓群，V4版本的升级版
    对应“SuperCapBuck_STM32G431_V2”
    #需要对每一套电容的代码进行调参

    Super_Cap_Boost_V1.1 魏洲航
    #基于Super_Cap_Boost的修改版，优化了代码书写格式，统一采用Linux/Unix风格
