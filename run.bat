@echo off
rem ���ݹ㲥ģ��ʵ�����нű�

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
	echo Sorry, ��������ˣ��뽫 output.tmp �ļ��� email ���������Ŵ�
)

pause
