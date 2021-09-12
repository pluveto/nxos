# NXOS 目录结构

- Docs: 文档

- Inc: 公共头文件

    - OS
    - Modules
    - Archs
    - Platforms

- OS:

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

- Archs: 处理器架构的公有部分代码

    - X86
    - Amd64
    - Riscv32
    - Riscv64
    - Arm
        - CortextA
        - CortextM
        - CortextR
    - AArch64
    
- Platforms: 平台私有代码，引导，总线，特有驱动
    
    - PC
        - PC32: 32位系统的引导，地址总线，32位驱动等
            - Boot
            - PlatformInit
            - Drivers
        - PC64: 64位系统的引导，地址总线，64位驱动等
        - PC_Public
            - PCI
            - PCIe
    - QemuRiscv64
        - SBI
        - PlatformInit
        - Drivers    
    - K210
        - SBI
        - PlatformInit
        - Drivers    
    - QemuVexpressA9
    - RaspberryPi
        - Drivers
            - serial
            - usb
            - sd
        - Raspi3-32
        - Raspi3-64
        - Raspi4-32
        - Raspi4-64

- Modules:
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