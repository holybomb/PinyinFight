@echo om

set DATA_PATH=..\projectHunter\Resources\db\
set TOOL=..\..\tools\TexturePacker.exe

echo ********************************
echo *******  export plist  *********
echo ********************************

call %TOOL% shengmu --data ..\Resources\plist\shengmu.plist --sheet ..\Resources\plist\shengmu.png --format cocos2d --size-constraints AnySize
call %TOOL% mainmenu --data ..\Resources\plist\mainmenu.plist --sheet ..\Resources\plist\mainmenu.png --format cocos2d --size-constraints AnySize
call %TOOL% gamepad --data ..\Resources\plist\gamepad.plist --sheet ..\Resources\plist\gamepad.png --format cocos2d --size-constraints AnySize
call %TOOL% public --data ..\Resources\plist\public.plist --sheet ..\Resources\plist\public.png --format cocos2d --size-constraints AnySize
xcopy /y /s fntFont ..\Resources\fonts

xcopy /y /s ..\Resources\image ..\..\..\..\projectHunter\Resources\image
xcopy /y /s ..\Resources\plist ..\..\..\..\projectHunter\Resources\plist
if not %ERRORLEVEL%==0 (
    goto Error
)

goto end
	
:Error
echo *** build process stopped, an error occured ***

:end
echo *** Everything is ok ***

pause