@echo off
rem ���ݹ㲥ģ��ʵ�����нű�

if exist output.tmp del output.tmp
if exist requestDeadlineMissRatio.result.txt (
    del requestDeadlineMissRatio.result.txt
)
if exist utilizationAndDeadlineMissRatio.result.txt (
    del utilizationAndDeadlineMissRatio.result.txt
)

echo Running...

set path=.\lib;%path%
Debug\databroadcast.exe > output.tmp

if exist requestDeadlineMissRatio.result.txt (
	type requestDeadlineMissRatio.result.txt
	notepad requestDeadlineMissRatio.result.txt
)

if exist utilizationAndDeadlineMissRatio.result.txt (
	type utilizationAndDeadlineMissRatio.result.txt
	notepad utilizationAndDeadlineMissRatio.result.txt
)

pause
