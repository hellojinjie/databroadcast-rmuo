@echo off
rem 数据广播模拟实验运行脚本

if exist output.tmp del output.tmp
if exist requestDeadlineMissRatio.result.txt (
    del requestDeadlineMissRatio.result.txt
)

echo Running...

set path=.\lib;%path%
Debug\databroadcast.exe > output.tmp

if exist requestDeadlineMissRatio.result.txt (
	type requestDeadlineMissRatio.result.txt
	notepad requestDeadlineMissRatio.result.txt
) else (
	echo Sorry, 程序出错了，请将 output.tmp 文件用 email 发给我来排错
)

pause
