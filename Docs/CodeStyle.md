# NXOS 编程风格

本文档是关于**NXOS**的代码编程风格的描述。开发者必须遵守本文档规范进行开发。NXOS系统内核使用C语言作为主要开发语言，因此以下规则仅仅适用于C语言代码。

## 1.目录名称

目录名称采用首字母大写的形式，子目录应该是属于父目录的内容，需要有相关性。例如：Drivers/Disk/

## 2.文件名称

文件名称也是使用首字母大写，如果有缩写，可以全部都大写。文件命名尽量不要使用通用化、使用频率高的名称。例如：MutexLock.c

## 3.头文件定义

C语言头文件为了避免多次重复包含，需要定义一个符号。这个符号的定义形式请采用如下的风格：

```c
#ifndef __FILE__
#define __FILE__
/* header file content */
#endif /* __FILE__ */
```

即定义的符号两侧采用 "_" 以避免重名，另外也可以根据文件名中是否包含多个词语而采用 "" 连接起来。例如：

```
__DRIVER_KEYBOARD__
```

## 4.头文件注释

在每个源文件文件头上，应该包括相应的版权信息，文件描述，Change Log 记录：

```c
/**
 * Copyright (c) 2018-2021, BookOS Development Team
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Contains: This file is a code style example
 * 
 * Change Logs:
 * Date           Author            Notes
 * 2021-9-12      JasonHu           Init
 * 2021-10-1      JasonHu           Add xxx
 */
```

例如采用如上的形式。

## 5.变量命名

变量采用小驼峰命名风格，即首字母小写，后续单词首字母大写，例如：

```c
myThread, tmpFileBuffer
```

## 6.函数命名

函数采用大驼峰命名风格，即每个单词的首字母都大写，如果单词是用的缩写，那么单词之间需要用"_"隔开，例如：

```c
ThreadFindByName
VFS_CreateFile
```

如果函数参数为空，必须使用void作为入口函数，例如：

```c
Cpu *CpuGetCurrent(void)
```

## 7.结构体定义

结构体命名使用大驼峰风格，结构体成员使用小驼峰命名，如果成员是函数指针，则使用大驼峰。

```c
struct SoundDevice
{
    char name[SOUND_DEVICE_NAME_LEN];
    int reference;
    void (*Write)(void *buffer, NX_Size len);
};
```

其中"{"和"}"需要**单独使用一行**。

结构体的类型定义是为了消除struct关键字，可以和结构体名一致。

```c
typedef struct SoundDevice SoundDevice;
```

如果需要在类型定义中使用指针，则需要使用Ptr后缀或者其他可以表达意思的单词指明。

```c
typedef struct Device *DevicePtr;
```

## 8.枚举定义

枚举结构名也是用大驼峰，枚举成员使用全大写，单词间用"_"隔开。

```c
enum SoundType
{
    SOUND_AC97,
    SOUND_PC,
    SOUND_HD,
};
typedef enum SoundType SoundType;
```

## 9.宏定义

使用大写作为名字，单词间使用"_"隔开，例如：

```c
#define DEVICE_NAME_LEN	32
```

当宏需要使用多行的时候，则使用空格+"\\"的形式进行换行，例如：

```c
#define ConsolePrintColor(color, str) \
	ConsoleSetColor(color); \
	ConsolePrint(str); \
	ConsoleRestoreColor()
```

## 10.注释编写

请使用英文做为注释，使用中文注释将意味着在编写代码时需要来回不停的切换中英文输入法从而打断编写代码的思路。并且使用英文注释也能够比较好的与中国以外的技术者进行交流。除非必须引用原来的文本的时候，就可以写中文。

源代码的注释不应该过多，更多的说明应该是代码做了什么，仅当个别关键点才需要一些相应提示性的注释以解释一段复杂的算法它是如何工作的。对语句的注释只能写在它的上方或右方，其他位置都是非法的。

注意，"//"注释只能放到右边。

```
/* Single line comment  */
/**
 * Multi-line comments 
 * Line1
 * Line2
 */

bufSize = 16;	// set buffer size
```

## 11.缩进以及分行

缩进请采用 4 个空格的方式。如果没有什么特殊意义，请在 "{" 后进行分行，并在下一行都采用缩进的方式，例如：

```c
    if (condition)
    {
        /* xxx */
    }
```

唯一的例外是 switch 语句，switch-case 语句采用 case 语句与 switch 对齐的方式， 例如：

```c
    switch (data)
    {
    case data1:
        break;
    case data2:
        break;
    default:
        break;
    }
```

## 12.大括号与空格

从代码阅读角度，建议每个大括号单独占用一行，而不是跟在语句的后面，例如：

```c
	if (condition)
    {
        /* xxx */
    }
```

匹配的大括号单独占用一行，代码阅读起来就会有相应的层次而不会容易出现混淆的情况。空格建议在非函数方式的括号调用前留一个空格以和前面的进行区分，例如：

```c
	if (x <= y)
    {
        /* xxx */
    }

    for (i = 0; i < MAX_CPU_NR; i++)
    {
        /* xxx */
    }
```

建议在括号前留出一个空格(涉及的包括 if、for、while、switch 语句)，而运算表达式中，运算符与字符串间留一个空格。另外，不要在括号的表达式两侧留空格，例如：

```c
	/* X: Bad!!! */
	if ( x <= y )
    {
        /* xxx */
    }
	
	/* V: Good! */
	if (x <= y)
    {
        /* xxx */
    }
```

有一个例外，就是如果遇到循环函数，里面不会执行代码的情况，括号就直接写在右边。

```c
    while (x < y) {}
    for (;;) {}
```

## 13.函数指针类型定义

在使用函数指针类型定义的时候，需要使用Handler后缀，来表示是一个处理函数。

```c
typedef void (*SoundCallbackHandler)(void *buffer, NX_Size len);

SoundCallbackHandler sndHandler;
```

## 14.函数参数太多

当函数参数过多的时候，一行写不下，那么就需要把每个参数写成一行。

```c
NX_Thread NX_CreateThread(char *threadName,
    ThreadHandler threadHandler,
    void *param,
    void *stackBase,
    NX_Size stackSize,
    int priority)
{
    ...   
}
```

## 15.宏条件定义

当使用宏条件#if, #else, #endif这种时，需要在开头和结尾后面写上注释来帮助开发这快速找到对应的宏。

```c
#ifdef NX_HAS_SMP
xxx
#else
#ifdef NX_HAS_ONECORE
yyy
#endif	/* NX_HAS_ONECORE */
#endif	/* NX_HAS_SMP */
```

## 总结

相信看到最后，您已经对我们的编程规范有了一定的了解，那么就开始您的**NXOS**开发之旅吧！

