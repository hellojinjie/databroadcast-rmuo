@echo off
rem ���ݹ㲥ģ��ʵ��

if exist output.tmp del output.tmp
if exist requestDeadlineMissRatio.result.txt (
    del requestDeadlineMissRatio.result.txt
)

echo Running...
set path=.\lib;%path%
Debug\databroadcast.exe > output.tmp

type requestDeadlineMissRatio.result.txt

notepad requestDeadlineMissRatio.result.txt

pause
