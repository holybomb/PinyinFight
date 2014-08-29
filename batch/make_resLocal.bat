@echo on
set PYTHON_PATH=..\tools\Python25\python
rem set PYTHON_PATH=C:\Python27\python
set PASSWODR=tools\make_res.py
set DATA_PATH=..\projectHunter\ResourcesMD5\

echo ********************************
echo ***** export MD5 *******
echo ********************************
set HOME=%cd%
cd /d %DATA_PATH%
rem svn update
cd %HOME%
call %PYTHON_PATH% %PASSWODR%

if not %ERRORLEVEL%==0 (
    goto Error
)

cd /d %DATA_PATH%
rem svn add * --force
rem svn commit -m "** MD5 res auto commit"

goto end
	
:Error
echo *** build process stopped, an error occured ***

:end
echo *** Everything is ok ***

pause