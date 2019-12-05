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

    002_mma7660
        mma7660 三轴加速度计 使用了DEVICE_ATTR 来调试

    测试方法
        1) 加载资源 insmod platform_resource/platform_res.ko
        2) 加载驱动 insmod mma7660_drv.ko
        3) 执行测试程序 ./drv_test 
        会输出X Y Z 三个轴上的数据[-31, 31]
            [root@4412 ~]#./drv_test
            x:  2,y:-30,z: 22
            x:  2,y:-30,z: 21
            x:  1,y:-30,z: 22
            x:  1,y:-30,z: 22
            x:  0,y:-31,z: 21
            x:  2,y:-31,z: 22
        
        4) cd /sys/bus/i2c/drivers/mma7660/3-004c, 此目录下会有reg 和 xyz_axis 2个文件
            xyz_axis: cat 这文件就可以读取XYZ三轴上的数据

                [root@4412 /sys/devices/platform/s3c2440-i2c.3/i2c-3/3-004c]#cat xyz_axis 
                1, -30,  22

            reg: cat 这个文件就可以读取mma7660所有寄存器的数据

                [root@4412 /sys/devices/platform/s3c2440-i2c.3/i2c-3/3-004c]#cat reg
                REG: [0x00] = 0x02 ...... [ 0000 0010 ]
                REG: [0x01] = 0x3f ...... [ 0011 1111 ]
                REG: [0x02] = 0x16 ...... [ 0001 0110 ]
                REG: [0x03] = 0x01 ...... [ 0000 0001 ]
                REG: [0x04] = 0x01 ...... [ 0000 0001 ]
                REG: [0x05] = 0xa0 ...... [ 1010 0000 ]
                REG: [0x06] = 0xe7 ...... [ 1110 0111 ]
                REG: [0x07] = 0x01 ...... [ 0000 0001 ]
                REG: [0x08] = 0x49 ...... [ 0100 1001 ]
                REG: [0x09] = 0x04 ...... [ 0000 0100 ]
                REG: [0x0a] = 0x0f ...... [ 0000 1111 ] 

            reg: echo <reg> <value> > reg 就可以修改寄存器的数据
            某些寄存器的数值可能无法改动，因此会提示Error并此寄存器的数据

                [root@4412 /sys/devices/platform/s3c2440-i2c.3/i2c-3/3-004c]#echo 7 0 > reg 
                write [0x7] = 0x0 OK

                [root@4412 /sys/devices/platform/s3c2440-i2c.3/i2c-3/3-004c]#echo 0 10 > reg
                write [0x0] = 0x10 Error
                ret   [0x0] = 0x0

003_spi
    使用spi 来驱动oled_0.96 模块
    
    测试方法
    
        1)加载ko insmod oled.ko
        2)执行测试程序 ./drv_test
            此时oled 会显示"嵌入式"


05_mmap_test
    测试mmap的功能

