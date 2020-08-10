# 360MEMZ
作者为360MEMZ自制的无害特效程序
## 构成部分
> 1.DLL-对应*360MEMZ.dll*,由主程序命令*rundll32.exe*执行这个DLL中的特效。  
> 2.Entry-对应*360MEMZ_Entry.exe*，即主程序。  
> 3.multimedia-程序中用到的多媒体文件，请大家按照readme自行下载。  
> 4.taskbar-任务栏小剧场（未开源）  
> 5.StopWorking-“停止工作”程序，已开源在*https://github.com/CnAoKip/StopWorking*  
> 6.NyanConsole-cmd高仿MEMZ（未开源）  
## 系统要求
由于调用了werapi和dwm系列函数，此程序需要在Windows 7及以上运行，当然，你可以改写并完善360MEMZ的系统兼容性。
## 构建要求
> 如果使用gcc编译，需要在编译时加上 ** -fexec-charset=GBK -finput-charset=GBK -lwinmm -lgdi32 ** 
> 需要在dev-cpp中建立一个工程
> gcc 4.9.2 编译通过，过高的gcc版本将会导致错误产生
