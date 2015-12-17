echo cd
@echo off
pushd %~dp0
pushd build
if not defined VisualStudioVersion (
    echo "Calling vcvarsall..."
    call k:\visualStudio2013\VC\vcvarsall.bat amd64
    if %ERRORLEVEL% GEQ 1 GOTO fail
)

del *lib.pdb

set CL=/D_USING_V110_SDK71_;%CL%

rem set optimlevel=-O2sy
set optimlevel=-Od -DENABLE_DEBUG_MODE

set commonCompilerFlags= -I ..\src -D_CRT_SECURE_NO_WARNINGS -MT -nologo -fp:fast -GS- -Gm- -EHa %optimlevel% -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -wd4702 -FC -Z7
set commonLinkerFlags= -opt:ref -subsystem:windows,5.01 -incremental:no 

echo "Compiling main program"
cl %commonCompilerFlags% "..\src\main.cpp" /link %commonLinkerFlags% Shlwapi.lib gdi32.lib user32.lib
if %ERRORLEVEL% GEQ 1 GOTO fail

:end
popd
popd
EXIT /B 0
:fail
popd
popd
EXIT /B 1
