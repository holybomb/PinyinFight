@echo on

set DATA_PATH=..\projectHunter\Resources\db\
set PASSWODR=1234
echo ********************************
echo ***** export password db *******
echo ********************************

call "tools\wxsqlite3PWD.exe" %DATA_PATH% %PASSWODR%

pause

if not %ERRORLEVEL%==0 (
    goto Error
)

goto end
	
:Error
echo *** build process stopped, an error occured ***

:end
echo *** Everything is ok ***

rem pause