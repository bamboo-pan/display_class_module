#include "Python.h"
#include "display_c.h"
#include "structmember.h"

//创建属性存放结构体
typedef struct {
	PyObject_HEAD
	/* Type-specific fields go here. */
	PyObject* display_device_info_list;
	PyObject* default_mode;

} DISPLAY_object;


static int
DISPLAY_clear(DISPLAY_object* self)
{
	Py_CLEAR(self->default_mode);
	Py_CLEAR(self->display_device_info_list);
	return 0;
}

//释放对象
static void
DISPLAY_dealloc(DISPLAY_object* self)
{
	PyObject_GC_UnTrack(self);
	DISPLAY_clear(self);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

//__new__
static PyObject*
DISPLAY_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
	DISPLAY_object* self;
	self = (DISPLAY_object*)type->tp_alloc(type, 0);
	if (self != NULL) {

	}
	return (PyObject*)self;
}


//__init__
static int
DISPLAY_init(DISPLAY_object* self, PyObject* args, PyObject* kwds)
{

	return 0;
}


//自定义方法


static PyObject* display_get_device_display_info(DISPLAY_object* self, PyObject* args) {

	int device_num = 0;
	DISPLAY_DEVICE* display_devices;
	get_display_devices(&display_devices, &device_num);
	struct display_device_info* display_device_info_list;
	get_device_display_info(display_devices, device_num, &display_device_info_list);

	PyObject* list = PyList_New(device_num);
	for (int i = 0; i < device_num; i++)
	{
		PyObject* list_temp = PyList_New(3);
		WCHAR* DeviceName = display_devices[i].DeviceName;
		PyObject* temp_device_name = PyUnicode_FromWideChar(DeviceName, -1);
		PyList_SetItem(list_temp, 0, temp_device_name);
		WCHAR* DeviceString = display_devices[i].DeviceString;
		PyObject* temp_device_string = PyUnicode_FromWideChar(DeviceString, -1);
		PyList_SetItem(list_temp, 1, temp_device_string);
		WCHAR* monitor_name = display_device_info_list[i].monitor_name;
		PyObject* temp_monitor_name = PyUnicode_FromWideChar(monitor_name, -1);
		PyList_SetItem(list_temp, 2, temp_monitor_name);
		PyList_SetItem(list, i, list_temp);
	}
	self->display_device_info_list = list;
	Py_IncRef(self->display_device_info_list);
	return list;

}

static PyObject* display_get_current_mode(DISPLAY_object* self, PyObject* args) {
	WCHAR* device_name;

	if (!PyArg_ParseTuple(args, "u", &device_name))
		return Py_BuildValue("i", 134);
	PyObject* temp_mode;
	DEVMODE default_mode;
	default_mode.dmSize = sizeof(DEVMODE);
	default_mode.dmDriverExtra = 0;
	default_mode = get_display_devices_current_mode_devicename(device_name);

	temp_mode= Py_BuildValue("[ukkklli]", device_name, default_mode.dmPelsWidth, default_mode.dmPelsHeight,
		default_mode.dmDisplayFrequency, default_mode.dmPosition.x, default_mode.dmPosition.y, default_mode.dmDisplayOrientation);

	self->default_mode = temp_mode;
	Py_INCREF(self->default_mode);
	return self->default_mode;

}

static PyObject* display_change_resolution(DISPLAY_object* self, PyObject* args) {
	WCHAR* device_name;
	DWORD width;
	DWORD height;
	DWORD frequency;
	LONG x;
	LONG y;
	DWORD Orientation;
	if (!PyArg_ParseTuple(args, "ukkkllk", &device_name, &width, &height, &frequency, &x, &y, &Orientation))
		return Py_BuildValue("i", 444);
	//DEVMODE default_mode;
	//default_mode.dmSize = sizeof(DEVMODE);
	//default_mode.dmDriverExtra = 0;
	//default_mode = get_display_devices_current_mode_devicename(device_name);
	int result;
	result = change_resolution_device_name(device_name, width, height, frequency, x, y, Orientation);
	return Py_BuildValue("i", result);

}



//遍历实例，使GC能够管控所有实例
static int
DISPLAY_traverse(DISPLAY_object* self, visitproc visit, void* arg)
{

	return 0;
}

//对外暴露类属性
static PyMemberDef DISPLAY_members[] = {
	{"display_device_info_list", T_OBJECT_EX, offsetof(DISPLAY_object, display_device_info_list), 0,"device list"},
	{"default_mode", T_OBJECT_EX, offsetof(DISPLAY_object, default_mode), 0,"default_mode"},
	{NULL}  /* Sentinel */
};

//对外暴露类方法
static PyMethodDef DISPLAY_methods[] = {
	{"get_device_display_info",(PyCFunction)display_get_device_display_info,METH_VARARGS},
	{"get_current_mode",(PyCFunction)display_get_current_mode,METH_VARARGS},
	{"change_resolution",(PyCFunction)display_change_resolution,METH_VARARGS},
	{NULL}  /* Sentinel */
};



//组装类
static PyTypeObject DISPLAY_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"display.DISPLAY",                          /*tp_name*/
	sizeof(DISPLAY_object),                     /*tp_basicsize*/
	0,                                          /*tp_itemsize*/
	(destructor)DISPLAY_dealloc,				/*tp_dealloc*/
	0,                                          /*tp_print*/
	0,                                          /*tp_getattr*/
	0,                                          /*tp_setattr*/
	0,                                          /*tp_compare*/
	0,                                          /*tp_repr*/
	0,                                          /*tp_as_number*/
	0,                                          /*tp_as_sequence*/
	0,                                          /*tp_as_mapping*/
	0,                                          /*tp_hash */
	0,                                          /*tp_call*/
	0,                                          /*tp_str*/
	0,                                          /*tp_getattro*/
	0,                                          /*tp_setattro*/
	0,                                          /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC, /*tp_flags*/
	"class test",                               /*tp_doc*/
	(traverseproc)DISPLAY_traverse,				/*tp_traverse*/
	(inquiry)DISPLAY_clear,						/*tp_clear*/
	0,                                          /*tp_richcompare*/
	0,                                          /*tp_weaklistoffset*/
	0,                                          /*tp_iter*/
	0,                                          /*tp_iternext*/
	DISPLAY_methods,                            /*tp_methods*/
	DISPLAY_members,							/*tp_members*/
	0,                         /*tp_getsets*/
	0,                                          /*tp_base*/
	0,                                          /*tp_dict*/
	0,                                          /*tp_descr_get*/
	0,                                          /*tp_descr_set*/
	0,                                          /*tp_dictoffset*/
	(initproc)DISPLAY_init,						/*tp_init*/
	0,                                          /*tp_alloc*/
	DISPLAY_new,								/*tp_new*/
};


// 对外暴露模块名称
static struct PyModuleDef display_class_Module = {
	PyModuleDef_HEAD_INIT,
	"display",//模块名称
	NULL,
	-1,
	0
};

//实例化类，返回实例对象
//displayclass是import的名称
//必须与setup.py中的extension名称相同
PyMODINIT_FUNC
PyInit_displayclass(void)
{
	PyObject* m;
	if (PyType_Ready(&DISPLAY_type) < 0)
		return NULL;

	m = PyModule_Create(&display_class_Module);
	if (m == NULL)
		return NULL;

	Py_INCREF(&DISPLAY_type);
	if (PyModule_AddObject(m, "DISPLAY", (PyObject*)& DISPLAY_type) < 0) {
		Py_DECREF(&DISPLAY_type);
		Py_DECREF(m);
		return NULL;
	}

	return m;
}