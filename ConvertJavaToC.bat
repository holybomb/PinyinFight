set PROJ_DIR=%CD%
echo %PROJ_DIR%
echo 	... Convert java to c
rem no need to convert java
rem del "%PROJ_DIR%\src\header\TEXT.java"

"%PROJ_DIR%\Tools\python25\python.exe" "%PROJ_DIR%\Tools\JavaToC\convertj2c.py" "%DATA_H_PATH%"
copy "%DATA_H_PATH%"\*.h %PROJ_DIR%\projectHunter\Classes\Header\
echo    ... Delete the java files
dir	/B /ON "%DATA_H_PATH%"\*.h > #list.h
echo //this is header files > %PROJ_DIR%\projectHunter\Classes\Header\header.h
for /F %%i in (#list.h) do (
		rem copy %DATA_PATH_RAW%\data.ini + %%i /a > NUL
		if not "%%i"=="header.h" (
			echo #include "%%i" >> %PROJ_DIR%\projectHunter\Classes\Header\header.h
		)
	)
del #list.h
del "%PROJ_DIR%\projectHunter\Classes\Header\*.java" /Q
REM pause