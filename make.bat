	if "%1"=="data" (
        set MAKEDATA=1
        set COMPILE=
        set EXECUTE=
        set CLEAN=
		set FAST=

    ) else if "%1"=="debug" (
        set MAKEDATA=
        set COMPILE=debug
        set EXECUTE=
        set CLEAN=

    ) else if "%1"=="release" (
        set MAKEDATA=
        set COMPILE=release
        set EXECUTE=
        set CLEAN=

    ) else if "%1"=="run" (
        set MAKEDATA=
        set COMPILE=
        set EXECUTE=1
        set CLEAN=

    ) else if "%1"=="clean" (
        set MAKEDATA=
        set COMPILE=
        set EXECUTE=
        set CLEAN=1

    ) 

	
call config.bat
rem current path
set PROJ_DIR=%cd%
	
rem directory with the tools
set TOOLS_PATH=%PROJ_DIR%\tools

set WORKDIRECTORY=%PROJ_DIR%\.version

set DATA_PATH=%WORKDIRECTORY%\data
rem directory where data are copied before being exported
set DATA_PATH_RAW=%DATA_PATH%\0-data
rem directory where data to be packed are put
set DATA_TOPACK_PATH=%DATA_PATH%\1-dataToPack
rem directory where data generated source files are put
set DATA_H_PATH=%DATA_PATH%\2-dataSrc
rem directory where data once packed are put
set DATA_OUT_PATH=%DATA_PATH%\2-dataOut

set DATA_SPRITE_PATH=%PROJ_DIR%\projectHunter\Resources\Sprite

set DATA_TILESET_PATH=%PROJ_DIR%\projectHunter\Resources\tileset

SET MASTER_DATA=%PROJ_DIR%\_master_data
rem set the directory which contains your specific sources
SET SPECIFIC_DATA=%PROJ_DIR%\data_%2

set LANG_DEFINE=LANG_%3
set PLATFORM_DEFINE=PLATFORM_%2
set DEFINES=
set DEFINES=%DEFINES% -D %LANG_DEFINE% -D %PLATFORM_DEFINE%

cd %PROJ_DIR%

rem ---- make sure work directory exist
if not exist %WORKDIRECTORY% (
    mkdir "%WORKDIRECTORY%"
)
	
if "%CLEAN%"=="1" (
    echo ###
    echo ### cleaning 
    echo ###

    Echo ...cleaning work directory...
    if exist %WORKDIRECTORY% (
        rd /S/Q %WORKDIRECTORY%
    )
    rem create a clean work directory
    mkdir "%WORKDIRECTORY%"

    goto end
)




:makedata

echo ...clearing...
if exist %DATA_PATH% (
    rd /S /Q %DATA_PATH%
)

md %DATA_PATH%
md %DATA_PATH_RAW%
md %DATA_TOPACK_PATH%
md %DATA_H_PATH%
md %DATA_OUT_PATH%

	echo ...copying master data
    xcopy /S /Y %MASTER_DATA% %DATA_PATH_RAW%\ > NUL
	REM xcopy /S /Y %SPECIFIC_DATA% %DATA_PATH_RAW%\ > NUL
	
	cd %DATA_PATH_RAW%\
	echo %DATA_PATH_RAW%\
    call make.bat %DATA_TOPACK_PATH% %DATA_OUT_PATH% %DATA_H_PATH%
    if not %ERRORLEVEL%==0 (
        goto Error
    )
	
	REM Echo ...packing data...
	REM if not exist %DATA_PATH_RAW%\data*.ini (
        REM echo ...WARNING...
		REM echo ...    file %DATA_PATH_RAW%\data-something.ini are missing...
        REM echo ...    no data to pack...
        REM goto Error
    REM )
	
	
	REM dir /B /ON %DATA_PATH_RAW%\data*.ini > #dir.txt
	REM echo //data.ini file > %DATA_PATH_RAW%\data.ini
	REM for /F %%i in (#dir.txt) do (
		REM copy %DATA_PATH_RAW%\data.ini + %%i /a > NUL
	REM )

	REM copy %DATA_PATH_RAW%\data.ini %DATA_PATH_RAW%\data_beforePP.ini
	REM "%TOOLS_PATH%\cpp\cpp.exe" -C -P %DEFINES% %DATA_PATH_RAW%\data.ini %DATA_PATH_RAW%\data.ini
	
	REM "%JDK_PATH%\bin\java.exe" -Xms256m -Xmx512m -cp "%TOOLS_PATH%\packFile" packFile -i %DATA_TOPACK_PATH%\ -h %DATA_H_PATH%\ -d %DATA_OUT_PATH%\ -c %TOOLS_PATH%\compress\ %DATA_PATH_RAW%\data.ini

	copy %DATA_OUT_PATH%\ %PROJ_DIR%\projectHunter\Resources\ > NUL
	REM if "%PLATFORM_DEFINE%"=="PLATFORM_ANDROID" (
		REM copy %DATA_OUT_PATH%\ %PROJ_DIR%\src\Android\AndroidApk\assets\ > NUL
	REM )
	REM copy %DATA_H_PATH%\DATA.hpp %PROJ_DIR%\src\header\DATA.h
	
	cd	%PROJ_DIR%
	call ConvertJavaToC.bat
	
	
	rem generate define files
	echo //this is the define files generate from make_data.bat > %PROJ_DIR%\src\header\define.h
	echo #define %LANG_DEFINE% >> %PROJ_DIR%\src\header\define.h
	rem echo #define VERSION_NUMBER %VERSION_NUM% >> %PROJ_DIR%\src\header\define.h
	
	
	
	cd %PROJ_DIR%
    if not %ERRORLEVEL%==0 (
        echo ERROR:: error while packing data
        goto error
    )
	
	
:end