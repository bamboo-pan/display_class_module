import displayclass as display
import copy
test=display.DISPLAY()
print(test.get_device_display_info())
print(test.get_current_mode('\\\\.\\DISPLAY4'))
print(test.default_mode)
temp_mode=copy.deepcopy(test.default_mode)
temp_mode[1]=1024
temp_mode[2]=768
test.change_resolution(*temp_mode)
input()
test.change_resolution(*test.default_mode)

