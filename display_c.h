//
//这个文件定义了所有display_c.c中的方法
#include"structs.h"

//对display_device第二次查询EnumDisplayDevices，会得到monitor的信息
//输入：display_device
//输出：display_device
DISPLAY_DEVICE get_monitor_info(DISPLAY_DEVICE d);

//获取display_device的当前模式
//输入：display_device
//输出：DEVMODE
DEVMODE get_display_devices_current_mode(DISPLAY_DEVICE temp_device);

/*
//获取display_device的当前模式
//输入：display_device name
//输出：DEVMODE
*/
DEVMODE get_display_devices_current_mode_devicename(WCHAR* device_name);

//获取display_device所有可用模式
//输入：display_device
//输入：mode_index，初始值为0,函数执行完，其值为所有可用模式的数量
//输出：DEVMODE类型的指针，指向一块连续区域，存放着所有可用模式，数量为mode_index的返回值
DEVMODE* get_available_display_mode(DISPLAY_DEVICE temp_device, int* mode_index);

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
int change_resolution(struct display_device_info ddi, int width, int height);

 int change_resolution_device_name(WCHAR* device_name, DWORD width, DWORD height, DWORD frequency, LONG x, LONG y, DWORD Orientation);
 
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
int restore_resolution(struct display_device_info ddi);

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
int change_random_resolution(struct display_device_info ddi);

//随机整数生成
//输入：起始值
//输入：最大值
//输出：随机整数
int random(int start, int end);

//随机小数生成，0-1
//输入：无
//输出：double小数
double random1();

//获取所有display_device
//输入：DISPLAY_DEVICE类型指针，指向一段DISPLAY_DEVICE类型的连续地址，每个地址内存放了指针，这一段连续指针指向一段连续的DISPLAY_DEVICE
//输入：device_num，初始值为0，返回时值为DISPLAY_DEVICE的数量
//输出：无
void get_display_devices(DISPLAY_DEVICE** display_devices,  int* device_num);

//获取所有display_device的信息，放入display_device_info结构体
//输入：DISPLAY_DEVICE类型指针，指向一段连续的DISPLAY_DEVICE类型数据，长度为lenth
//输入：DISPLAY_DEVICE数量lenth
//输入：display_device_info 类型的指针，指向一段连续的display_device_info内存地址，每个地址内存放了指针，这一段连续指针指向一段连续的display_device_info的结构体数据
//		数量为lenth

void get_device_display_info(DISPLAY_DEVICE* display_devices, int lenth, struct display_device_info** display_device_info_list);

