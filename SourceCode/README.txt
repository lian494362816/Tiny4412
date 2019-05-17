reg_editor
    查看寄存器的小工具
    
    使用方法：
        1) 加载驱动 reg_editor_drv.ko
        2) 执行测试程序  ./reg_editor/drv_test/drv_test
            Use:./drv_test <r8|r16|r32> <reg_address> [num] 
            Use:./drv_test <w8|w16|w32> <reg_address> <val>

platform_resource
    平台的资源

001_led
    001_simple_led  
        最简单的led 驱动，insmod 点亮led1, rmmod 熄灭led1

    002_plat_led
        使用平台总写的led 驱动

        测试方法:
            1) 加载资源 insmod platform_resource/platform_res.ko
            2) 加载驱动 indmod led_platform_drv.ko
            3) 执行测试程序 ./002_plat_led/drv_test/drv_test ,可以看到led1~led4依次闪烁一遍

    003_led_class  
        使用led子系统来实现 led 的控制
        
        测试方法:
            1) 加载驱动 insmod insmod led_class_drv.ko
            2) cd sys/class/leds/s3c4412_led/ 
            3) echo 1 > brightness ; 点亮led 
            4) echo 0 > brightness ; 熄灭led 

    004_plat_led_class
        使用led子系统和platform-device-driver 的方法实现对led的控制

        测试方法
            1) 加载资源 insmod platform_resource/platform_res.ko
            2) 加载驱动 insmod led_plat_class_drv.ko
            3) cd sys/class/leds/ledX ; X 表示具体的数字, 可以为0~3
            4) echo 1 > brightness ; 点亮led 
            5) echo 0 > brightness ; 熄灭led 

002_i2c
    001_24AA025E48
        eeprom-s4aa025e48 芯片的驱动程序
    
    测试方法
        1) 加载资源 insmod platform_resource/platform_res.ko
        2) 加载驱动 insmod i2c_eeprom_drv.ko
        3) 执行测试程序 ./drv_test 

        命令如下
            Use ./drv_test <w> <addr> <value> 
            Use ./drv_test <r> <addr> [count] 
        举例
            [root@4412 ~]#./drv_test w 5 10
            [INF]:addr:0x5 , value:0x10 

            [root@4412 ~]#./drv_test r 5
            [0x5 ] 0x10

            [root@4412 ~]#./drv_test r 0 10
            [0x0 ] 0x0   0x1   0x2   0x3   0x4   0x10  0x0   0x0
            [0x8 ] 0x0   0x0



