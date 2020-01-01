#include "windows.h"

struct display_device_info
{
	//原始DISPLAY_DEVICE
	DISPLAY_DEVICE device;
	//获取到的显示器名称，宽字节类型
	WCHAR* monitor_name;
	//显示器默认模式
	DEVMODE default_mode;
	//显示器可用模式
	DEVMODE* available_modes;
	//显示器可用模式数量
	int available_modes_count;
};
