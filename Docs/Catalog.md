# NXOS 目录结构

- Docs: 文档

- Scripts: 脚本文件

- Tools: 工具链或者其它工具

- Src: 源码目录
    - Inc: 公共头文件

        - Mods
        - Archs
        - Platforms
        - IPC
        - MM
        - Process
        - Thread
        - Sched
        
    - Init
    - Thread
        - Mutex
        - Smaphore
        - Lock
    - Process
    - Sched
        - SMP

    - MM
        - Heap
        - VmSpace
        - Swap

    - IPC
        - Signal
        - Pipe
        - Channel
        - MsgQueue
        - LocalSocket
        - LPC

    - Platforms: 平台私有代码，引导，总线，特有驱动
        - i386

            - PC32: 32位系统的引导，地址总线，32位驱动等
                - Drivers
        - Amd64

            - PC64: 64位系统的引导，地址总线，64位驱动等

        - Riscv64
            - QemuRiscv64
                - Drivers
            - K210

        - Arm
            - QemuVexpressA9
        
        - Arm64
            - RaspberryPi
                - Drivers
                    - serial
                    - usb
                    - sd
                - Raspi3-32
                - Raspi3-64
                - Raspi4-32
                - Raspi4-64

    - Mods:
        - Drivers:  不同类型的驱动框架  
            - Disk
            - Char
                - Serial
            - Net
            - Sound
            - GPU
            - Power
            - UserDriver: 用户态驱动框架
        
        - Virtual: 
            - Ramdisk
            - Loop
            - Null
            - Zero
            - Virtio: 虚拟IO接口

        - FS
            - Fat32
            - Ext2/3/4
            - Devfs
            - Ramfs
            - Romfs
            - Public

        - Net 
            - TCP/UDP
            - Socket
            - Public

        - Syscall

        - TTY

        - Sound

        - Graphic

        - KVM

        - Security