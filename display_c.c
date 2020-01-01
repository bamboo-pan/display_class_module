// display_c.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//stdio 就是指 “standard input & output"（标准输入输出）
//所以，源代码中如用到标准输入输出函数时，就要包含这个头文件！
//int getchar()//从标准输入设备写入一个字符
//int putchar()//向标准输出设备读出一个字符
//int scanf(char* format[, argument…])//从标准输入设备读入格式化后的数据
//int printf(char* format[, argument…])//向标准输出设备输出格式化字符串
//char* gets(char* string)//从标准输入设备读入一个字符串
//int puts(char* string)//向标准输出设备输出一个字符串
//int sprintf(char* string, char* format[, …])//把格式化的数据写入某个字符串缓冲区
#include <stdio.h>
//包括如下
//#include <windef.h>
//#include <winbase.h>
//#include <wingdi.h>
//#include <winuser.h>
#include "windows.h"
#include <time.h>
//stdlib 头文件即standard library标准库头文件。
//stdlib.h里面定义了五种类型、一些宏和通用工具函数。 类型例如size_t、wchar_t、div_t、ldiv_t和lldiv_t；
//宏例如EXIT_FAILURE、EXIT_SUCCESS、RAND_MAX和MB_CUR_MAX等等； 常用的函数如malloc()、calloc()、realloc()、free()、system()、atoi()、atol()、rand()、srand()、exit()等等。
//具体的内容可以打开编译器的include目录里面的stdlib.h头文件查看。
#include <stdlib.h>
//自定义结构体
//
#include "structs.h"



//随机整数生成
//输入：起始值
//输入：最大值
//输出：随机整数
int random(int start, int end)
{
	srand(time(NULL));
	return rand() % (end + 1) + start;

}

//随机小数生成，0-1
//输入：无
//输出：double小数
double random1()
{
	srand(time(NULL));
	return ((double)rand() / (RAND_MAX));
}

//对display_device第二次查询EnumDisplayDevices，会得到monitor的信息
//输入：display_device
//输出：display_device
DISPLAY_DEVICE get_monitor_info(DISPLAY_DEVICE d)
{
	DISPLAY_DEVICE dd;
	dd.cb = sizeof(DISPLAY_DEVICE);
	if (EnumDisplayDevices(d.DeviceName, 0, &dd, 0))
	{
	}
	return dd;
}

//获取display_device的当前模式
//输入：display_device
//输出：DEVMODE
DEVMODE get_display_devices_current_mode(DISPLAY_DEVICE d)
{

	DEVMODE mode;
	mode.dmSize = sizeof(DEVMODE);
	mode.dmDriverExtra = 0;

	if (EnumDisplaySettingsEx(d.DeviceName, ENUM_REGISTRY_SETTINGS, &mode, EDS_RAWMODE | EDS_ROTATEDMODE))
	{
		return mode;
	}
}

/*
//获取display_device的当前模式
//输入：display_device name
//输出：DEVMODE
*/
DEVMODE get_display_devices_current_mode_devicename(WCHAR* device_name)
{

	DEVMODE mode;
	mode.dmSize = sizeof(DEVMODE);
	mode.dmDriverExtra = 0;

	if (EnumDisplaySettingsEx(device_name, ENUM_REGISTRY_SETTINGS, &mode, EDS_RAWMODE | EDS_ROTATEDMODE))
	{
		return mode;
	}
}

//获取display_device所有可用模式
//输入：display_device
//输入：mode_index，初始值为0,函数执行完，其值为所有可用模式的数量
//输出：DEVMODE类型的指针，指向一块连续区域，存放着所有可用模式，数量为mode_index的返回值
DEVMODE* get_available_display_mode(DISPLAY_DEVICE temp_device, int* mode_index)
{
	//初始化DEVMODE类型数组长度为10
	//
	int mode_list_size;
	mode_list_size = 10;

	//为数组分配内存，类型为DEVMODE，长度为mode_list_size
	//DEVMODE类型的指针指向数组mode_list第一个DEVMODE类型的数据
	//数组内其他元素可通过下标访问
	DEVMODE* mode_list = (DEVMODE*)malloc(sizeof(DEVMODE) * mode_list_size);

	//DEVMODE类型的变量，用于临时接收display_device的DEVMODE
	//
	DEVMODE mode;
	mode.dmSize = sizeof(DEVMODE);
	mode.dmDriverExtra = 0;
	//
	//从mode_index=0开始尝试获取可用devmode，返回成功则从mode_index+1继续调用，知道返回失败，表示取完所有devmode
	while (EnumDisplaySettingsEx(temp_device.DeviceName, *mode_index, &mode, EDS_ROTATEDMODE))
	{
		//如果mode_list数组容量不足以存放所有devomde则重新分配数组，大小*2
		if (*mode_index >= mode_list_size)
		{
			DEVMODE* mode_list_temp = (DEVMODE*)realloc(mode_list, sizeof(DEVMODE) * mode_list_size * 2);
			if (mode_list_temp != NULL)
			{
				mode_list = mode_list_temp;
				mode_list_size = mode_list_size * 2;
			}

		}
		//将取出的devmode放入mode_list数组中
		mode_list[*mode_index] = mode;
		//mode_index + 1继续调用
		*mode_index = *mode_index + 1;
	}
	//返回mode_list数组的第一个元素的地址，其他元素通过下表以及mode_index来访问
	return mode_list;
}

//改变display_device的显示模式
//输入：display_device_info结构体
//输入：宽
//输入:高
//输出：整形结果
//#define DISP_CHANGE_SUCCESSFUL       0
//#define DISP_CHANGE_RESTART          1
//#define DISP_CHANGE_FAILED          -1
//#define DISP_CHANGE_BADMODE         -2
//#define DISP_CHANGE_NOTUPDATED      -3
//#define DISP_CHANGE_BADFLAGS        -4
//#define DISP_CHANGE_BADPARAM        -5
int change_resolution(struct display_device_info ddi, int width, int height)
{
	//临时DEVMODE，用于设置模式
	DEVMODE temp_mode;
	temp_mode.dmSize = sizeof(DEVMODE);
	temp_mode.dmDriverExtra = 0;
	//在默认模式的基础上修改分辨率
	temp_mode = ddi.default_mode;
	temp_mode.dmPelsWidth = width;
	temp_mode.dmPelsHeight = height;

	int res = ChangeDisplaySettingsEx(ddi.device.DeviceName, &temp_mode, NULL, 0, NULL);
	return res;
}


 int change_resolution_device_name(WCHAR* device_name, DWORD width, DWORD height, DWORD frequency, LONG x, LONG y, DWORD Orientation)
 {
	 DEVMODE temp_mode;
	 temp_mode.dmSize = sizeof(DEVMODE);
	 temp_mode.dmDriverExtra = 0;
	 temp_mode = get_display_devices_current_mode_devicename(device_name);
	 temp_mode.dmPelsWidth = width;
	 temp_mode.dmPelsHeight = height;
	 temp_mode.dmDisplayFrequency = frequency;
	 temp_mode.dmPosition.x = x;
	 temp_mode.dmPosition.y = y;
	 temp_mode.dmDisplayOrientation = Orientation;
	 DMDO_DEFAULT;

	 //default_mode.dmPelsWidth = width;
	 //default_mode.dmPelsHeight = height;

	 int res = ChangeDisplaySettingsEx(device_name, &temp_mode, NULL, 0, NULL);
	 DISP_CHANGE_SUCCESSFUL;
	 DISP_CHANGE_BADMODE;
	 return res;
 }

//随机改变display_device的显示模式，从所有可用模式中随机选择一个
//输入：display_device_info结构体
//输出：整形结果
//#define DISP_CHANGE_SUCCESSFUL       0
//#define DISP_CHANGE_RESTART          1
//#define DISP_CHANGE_FAILED          -1
//#define DISP_CHANGE_BADMODE         -2
//#define DISP_CHANGE_NOTUPDATED      -3
//#define DISP_CHANGE_BADFLAGS        -4
//#define DISP_CHANGE_BADPARAM        -5
int change_random_resolution(struct display_device_info ddi)
{

	//从所有可用mode中随机选取一个
	int random_int = random(0, ddi.available_modes_count - 1);
	int res = ChangeDisplaySettingsEx(ddi.device.DeviceName, &ddi.available_modes[random_int], NULL, 0, NULL);
	return res;
}

//还原display_device的显示模式
//输入：display_device_info结构体
//输出：整形结果
//#define DISP_CHANGE_SUCCESSFUL       0
//#define DISP_CHANGE_RESTART          1
//#define DISP_CHANGE_FAILED          -1
//#define DISP_CHANGE_BADMODE         -2
//#define DISP_CHANGE_NOTUPDATED      -3
//#define DISP_CHANGE_BADFLAGS        -4
//#define DISP_CHANGE_BADPARAM        -5
int restore_resolution(struct display_device_info ddi)
{
	//还原默认模式
	int res = ChangeDisplaySettingsEx(ddi.device.DeviceName, &(ddi.default_mode), NULL, 0, NULL);
	return res;
}

//获取所有display_device
//输入：DISPLAY_DEVICE类型指针，指向一段DISPLAY_DEVICE类型的连续地址，每个地址内存放了指针，这一段连续指针指向一段连续的DISPLAY_DEVICE
//输入：device_num，初始值为0，返回时值为DISPLAY_DEVICE的数量
//输出：无
void get_display_devices(DISPLAY_DEVICE** display_devices, int* device_num)
{
	//DISPLAY_DEVICE数组初始长度为0
	//如果获取设备成功了，再分配内存
	int temp_device_count = *device_num;

	//
	//临时DISPLAY_DEVICE，用于存放取出的DISPLAY_DEVICE
	DISPLAY_DEVICE d;
	d.cb = sizeof(DISPLAY_DEVICE);
	//尝试获取DISPLAY_DEVICE
	//如果成功则数组长度加1并分配内存
	//将获取到的DISPLAY_DEVICE存放到数组里
	while (EnumDisplayDevices(NULL, *device_num, &d, 0))
	{


		if (*device_num >= temp_device_count)
		{
			DISPLAY_DEVICE* temp;
			if (temp_device_count == 0)
			{
				temp = (DISPLAY_DEVICE*)malloc((temp_device_count + 1) * sizeof(DISPLAY_DEVICE));
			}
			else
			{
				temp = (DISPLAY_DEVICE*)realloc(*display_devices, (temp_device_count + 1) * sizeof(DISPLAY_DEVICE));
			}

			if (temp != NULL)
			{
				(*display_devices) = temp;
				temp_device_count = temp_device_count + 1;
			}
		}
		//
		//将取到的DISPLAY_DEVICE按索引放入数组中
		(*display_devices)[(*device_num)] = d;
		//device_num+1
		//用于下一次尝试获取设备
		(*device_num)++;
	}
}

//获取所有display_device的信息，放入display_device_info结构体
//输入：DISPLAY_DEVICE类型指针，指向一段连续的DISPLAY_DEVICE类型数据，长度为lenth
//输入：DISPLAY_DEVICE数量lenth
//输入：display_device_info 类型的指针，指向一段连续的display_device_info内存地址，每个地址内存放了指针，这一段连续指针指向一段连续的display_device_info的结构体数据
//		数量为lenth
void get_device_display_info(DISPLAY_DEVICE* display_devices, int lenth, struct display_device_info** display_device_info_list)
{
	//初始化display_device_info类型数组，长度为display_devices的长度
	*display_device_info_list = (struct display_device_info*)malloc(sizeof(struct display_device_info) * lenth);
	for (int i = 0; i < lenth; i++)
	{
		//获取DISPLAY_DEVICE的monitor信息
		DISPLAY_DEVICE temp_display_device = get_monitor_info(display_devices[i]);
		printf("%s", "");
		//临时display_device_info结构体，用于存放取出的数据
		struct display_device_info temp;
		//放入DISPLAY_DEVICE
		temp.device = display_devices[i];
		//获取monitor信息字符串长度
		int len = lstrlen(temp_display_device.DeviceString) + 1;
		//拷贝monitor信息字符串到新创建的字符串里，由于DISPLAY_DEVICE
		//里monitor信息是一个指针指向数组，故无法直接赋值
		WCHAR* subject = (WCHAR*)malloc(len); // allocate for string and ending \0
		wcscpy_s(subject, len, temp_display_device.DeviceString);
		//放入monitor信息
		temp.monitor_name = subject;
		//如果monitor信息不为空，则代表连接着显示器
		if (temp.monitor_name != NULL)
		{
			//放入默认模式
			temp.default_mode = get_display_devices_current_mode(display_devices[i]);
			//获取可选模式
			int mode_count = 0;
			temp.available_modes = get_available_display_mode(display_devices[i], &mode_count);
			//放入可选模式数组
			temp.available_modes_count = mode_count;
		}
		//将临时display_device_info结构体放入display_device_info结构体数组
		(*display_device_info_list)[i] = temp;
		printf("%s", "");
	}

}
