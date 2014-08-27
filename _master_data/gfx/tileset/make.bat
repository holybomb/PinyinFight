
@echo off
setlocal enabledelayedexpansion

echo ...BUILDING TILESET...

REM DIRECTORY, WHERE DATA THAT NEED TO BE PACKED, MUST BE COPIED
set DIR_TO_PACK=%1

REM DIRECTORY, WHERE DATA WHICH DON'T NEED TO BE PACKED, MUST BE COPIED
set DIR_NOT_TO_PACKED=%2

REM DIRECTORY, WHERE TO SOURCE FILES MUST BE COPIED
set DIR_SOURCE=%3


"%TOOLS_PATH%\AuroraGT\AuroraGT.exe" "export.sprcmd"

move /y *.bs "%DATA_TILESET_PATH%" > NUL

rem --- .Game stuff
cd level
set orgPath=%cd%
setlocal enabledelayedexpansion
for /R %%s in (.) do ( 
	cd %%s
	for %%i in (*.gamecmd) do (
	echo %%i
	%TOOLS_PATH%\AuroraGT\AuroraGT.exe %%i

	
	set prefix=%%i
	rem set	prefix=!prefix:00_=!
	rem set prefix=!prefix:_mockup=_level!
	set prefix=!prefix:.gamecmd=!
	echo !prefix!
	for %%j in (*.bin, *.layers, *.cinematics) do (
		rem echo !prefix!_%%j.dat
		copy %%j ..\!prefix!_%%j > NUL
		del %%j
	)
	)
	cd %orgPath%
	rem copy %orgPath%\%%s\*.* %orgPath%
	rem echo %%s

) 
cd %orgPath%
echo on
echo %GTSPARSER% %DATA_PATH_RAW%\gfx\tileset\level\Game.gts %DATA_H_PATH%\GameGtsData GameGtsData
rem echo %GAMEPARSER% %DATA_PATH_RAW%\gfx\tileset\level\Game.game %DATA_H_PATH%\GameLayerData GameLayerData

%GTSPARSER% %DATA_PATH_RAW%\gfx\tileset\level\Game.gts %DATA_H_PATH%\GameGtsData GameGtsData
rem %GAMEPARSER% %DATA_PATH_RAW%\gfx\tileset\level\Game.game %DATA_H_PATH%\GameLayerData GameLayerData

REM pause
rem %TOOLS_PATH%\AuroraGT\AuroraGT.exe "Game.gamecmd"

copy /y *.bin "%DATA_TILESET_PATH%" > NUL
copy /y *.layers "%DATA_TILESET_PATH%" > NUL
copy /y *.cinematics "%DATA_TILESET_PATH%" > NUL
echo %DATA_TILESET_PATH%


move %DATA_H_PATH%\GameGtsData.h %PROJ_DIR%\projectHunter\Classes\Header\GameGtsData.h
rem move %DATA_H_PATH%\GameLayerData.h %PROJ_DIR%\projectHunter\Classes\Header\GameLayerData.h

del %DATA_H_PATH%\GameGtsData*
del %DATA_H_PATH%\GameLayerData*

rem	echo     generating maps files from tileset
rem	cd %TOOLS_PATH%\TilesetExporter
rem	"%JDK_PATH%\bin\java.exe" TilesetExporter %MASTER_DATA%\gfx\tilesets\maps\\map_rafting.aTLMap map_rafting.size map_rafting.data map_rafting.flip
rem	move /y map_rafting.* "%DIR_TO_PACK%" > NUL
rem	cd %MASTER_DATA%\gfx\tilesets\maps

cd ..



:end
endlocal
