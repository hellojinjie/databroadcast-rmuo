@echo off
rem 数据广播模拟实验运行脚本
title %cd%

if exist output.tmp del output.tmp
if exist requestDeadlineMissRatio.result.txt (
    del requestDeadlineMissRatio.result.txt
)
if exist utilizationAndDeadlineMissRatio.result.txt (
    del utilizationAndDeadlineMissRatio.result.txt
)

echo Running...

set path=.\lib;%path%
echo 开始时间 %time%
Debug\databroadcast.exe > output.tmp

rem 如果不正常结束就退出
if not %errorlevel% == 0 goto end

echo 结束时间 %time%

rem 花这么长时间计算的结果一定要备份
if not exist result mkdir result
set dirname=result\%date:~0,4%%date:~5,2%%date:~8,2%%time:~0,2%%time:~3,2%%time:~6,2%
mkdir %dirname%
copy simulation.conf.js %dirname%
copy output.tmp %dirname%
copy *.result.txt %dirname% 

if exist requestDeadlineMissRatio.result.txt (
	type requestDeadlineMissRatio.result.txt
	notepad requestDeadlineMissRatio.result.txt
)

if exist utilizationAndDeadlineMissRatio.result.txt (
	type utilizationAndDeadlineMissRatio.result.txt
	notepad utilizationAndDeadlineMissRatio.result.txt
)

:end
pause
