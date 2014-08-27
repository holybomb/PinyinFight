@echo off
setlocal enabledelayedexpansion

rem script used to build the data

rem ---- call each data make script, warning, order is important
rem icon
if exist icon (
    cd icon
    if exist make.bat (
        call make.bat %1 %2 %3
    )
    cd ..
)
if not %ERRORLEVEL%==0 (
    goto end
)

rem math
if exist math (
    cd math
    if exist make.bat (
        call make.bat %1 %2 %3
    )
    cd ..
)

rem sound
if exist sound (
    cd sound
    if exist make.bat (
        call make.bat %1 %2 %3
    )
    cd ..
)
if not %ERRORLEVEL%==0 (
    goto end
)

rem gfx
if exist gfx (
    cd gfx
    if exist make.bat (
        call make.bat %1 %2 %3
    )
    cd ..
)

rem font
if exist font (
    cd font
    if exist make.bat (
        call make.bat %1 %2 %3
    )
    cd ..
)

rem tileset
if exist tileset (
    cd tileset
    if exist make.bat (
        call make.bat %1 %2 %3
    )
    cd ..
)

rem text
if exist text (
    cd text
    if exist make.bat (
        call make.bat %1 %2 %3
    )
    cd ..
)

rem text
if exist sounds (
    cd sounds
    if exist make.bat (
        call make.bat %1 %2 %3
    )
    cd ..
)


rem math
if exist math (
    cd math
    if exist make.bat (
        call make.bat %1 %2 %3
    )
    cd ..
)

if not %ERRORLEVEL%==0 (
    goto end
)

rem text
rem if exist text (
rem cd text
rem if exist make.bat (
rem     call make.bat %1 %2 %3
rem )
rem cd ..
rem )
rem if not %ERRORLEVEL%==0 (
rem     goto end
rem )

:end
endlocal