@echo off

setlocal enabledelayedexpansion

echo ...BUILDING GFX...

REM DIRECTORY, WHERE DATA THAT NEED TO BE PACKED, MUST BE COPIED
set DIR_TO_PACK=%1

REM DIRECTORY, WHERE DATA WHICH DON'T NEED TO BE PACKED, MUST BE COPIED
set DIR_NOT_TO_PACKED=%2

REM DIRECTORY, WHERE TO SOURCE FILES MUST BE COPIED
set DIR_SOURCE=%3

rem call make.bat in each subdirectory
	for /D %%i in (*.*) do (
		cd %%i
		if exist make.bat (
			call make.bat %1 %2 %3
		)
		cd ..
	)

rem	--- converting SPRITES to BSPRITES
rem	%TOOLS_PATH%\AuroraGT\AuroraGT.exe "export.sprcmd"
rem --- copy to output
move /y "%DATA_OUT_PATH%"\*.bs  %DATA_SPRITE_PATH%\> NUL

rem	copy /y *.java "%DIR_SOURCE%" > NUL

:end
endlocal