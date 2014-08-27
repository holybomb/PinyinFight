svn update --force
svn revert -R %CD%\projectHunter\Resources
svn revert -R %CD%\projectHunter\Classes
@echo off

echo./*
echo. * Check VC++ environment...
echo. */
echo.

if defined VS110COMNTOOLS (
    set VSTOOLS="%VS110COMNTOOLS%"
    set VC_VER=110
) else if defined VS100COMNTOOLS (
    set VSTOOLS="%VS100COMNTOOLS%"
    set VC_VER=100
)


set VSTOOLS=%VSTOOLS:"=%
set "VSTOOLS=%VSTOOLS:\=/%"

set VSVARS="%VSTOOLS%vsvars32.bat"

if not defined VSVARS (
    echo Can't find VC2010 or VC2012 installed!
    goto ERROR
)

echo./*
echo. * Building cocos2d-x library binary, please wait a while...
echo. */
echo.

call %VSVARS%
if %VC_VER%==100 (
    rem msbuild cocos2d-win32.vc2010.sln /p:Configuration="Debug" /Project HelloCpp
	devenv cocos2d-win32.vc2010.sln /build "Debug|Win32" /Project HelloCpp 
    rem msbuild cocos2d-win32.vc2010.sln /p:Configuration="Release"
) else if %VC_VER%==110 (
    msbuild cocos2d-win32.vc2012.sln /t:Clean
    msbuild cocos2d-win32.vc2012.sln /p:Configuration="Debug" /m
    msbuild cocos2d-win32.vc2012.sln /p:Configuration="Release" /m
) else (
    echo Script error.
    goto ERROR
)

echo./*
echo. * Check the cocos2d-win32 application "TestCpp.exe" ...
echo. */
echo.

pushd ".\Debug.win32\"

set CC_HUNTER_BIN=HelloCpp.exe

set CC_HUNTER_RES=%CD%\..\projectHunter\Resources
set RELEASE_PATH=%CD%\..\策划表单\GameRelease
rem %%E7%%AD%%96%%E5%%88%%92%%E8%%A1%%A8%%E5%%8D%%95



echo./*
echo. * Run cocos2d-win32 tests.exe and view Cocos2d-x Application Wizard for Visual Studio User Guide.
echo. */
echo.

echo %CD%
echo %RELEASE_PATH%
echo %CC_HUNTER_RES%

rem xcopy /S /Y %MASTER_DATA% %DATA_PATH_RAW%\
rem xcopy  /E /Y %CD%\ %RELEASE_PATH%\
rem xcopy  /E /Y %CC_HUNTER_RES%\ %RELEASE_PATH%\
xcopy /S /Y %CD% %RELEASE_PATH%\
xcopy /S /Y %CC_HUNTER_RES% %RELEASE_PATH%\

echo on
popd
goto EOF

:ERROR
exit 1
:EOF
cd /d %RELEASE_PATH%
svn add * --force
svn commit -m "web commit"
exit 0