#ifndef __OS_CONFIG__
#define __OS_CONFIG__
#define CONFIG_CPU_64BITS 1
#define CONFIG_DEBUG 1
#define CONFIG_LOG_LEVEL 3
#define CONFIG_DEBUG_COLOR 1
#define CONFIG_DEBUG_TIMELINE 1
#define CONFIG_PLATFROM_NAME "riscv64-qemu"
#define CONFIG_NR_MULTI_CORES 4
#define CONFIG_IRQ_NAME_LEN 48
#define CONFIG_NR_IRQS 80
#define CONFIG_KVADDR_OFFSET 0x00000000
#define CONFIG_PAGE_SHIFT 12
#define CONFIG_MAX_THREAD_NR 256
#define CONFIG_THREAD_NAME_LEN 32
#define CONFIG_THREAD_STACK_SIZE 8192
#define CONFIG_ENABLE_SCHED 1
#define CONFIG_PLATFROM_RISCV64_QEMU 1
#define CONFIG_UART0_FROM_SBI 1
#define CONFIG_PRINT_BUF_LEN 256
#define CONFIG_TICKS_PER_SECOND 100
#define CONFIG_DEMO_HAL_CONTEXT 1
#endif
