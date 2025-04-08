@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by TIMER_APP.HPJ. >"hlp\timer_app.hm"
echo. >>"hlp\timer_app.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\timer_app.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\timer_app.hm"
echo. >>"hlp\timer_app.hm"
echo // Prompts (IDP_*) >>"hlp\timer_app.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\timer_app.hm"
echo. >>"hlp\timer_app.hm"
echo // Resources (IDR_*) >>"hlp\timer_app.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\timer_app.hm"
echo. >>"hlp\timer_app.hm"
echo // Dialogs (IDD_*) >>"hlp\timer_app.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\timer_app.hm"
echo. >>"hlp\timer_app.hm"
echo // Frame Controls (IDW_*) >>"hlp\timer_app.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\timer_app.hm"
REM -- Make help for Project TIMER_APP


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\timer_app.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\timer_app.hlp" goto :Error
if not exist "hlp\timer_app.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\timer_app.hlp" Debug
if exist Debug\nul copy "hlp\timer_app.cnt" Debug
if exist Release\nul copy "hlp\timer_app.hlp" Release
if exist Release\nul copy "hlp\timer_app.cnt" Release
echo.
goto :done

:Error
echo hlp\timer_app.hpj(1) : error: Problem encountered creating help file

:done
echo.
