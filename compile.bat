@echo ��ʼ���ɰ�װ����
@echo off

set CUR_PATH=%~dp0
set PATH=C:\Qt\Qt5.6.3\5.6.3\msvc2015\bin;%PATH%

cd .\bin
windeployqt --no-opengl-sw --no-translations TimeHelper.exe 
del /f /s /q *.pdb


