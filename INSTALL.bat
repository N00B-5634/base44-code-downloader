@echo off
setlocal
set "EXT_DIR=%~dp0"

echo.
echo Base44 Code Downloader - install
echo ==================================
echo.
echo 1. In Chrome, enable "Developer mode".
echo 2. Click "Load unpacked".
echo 3. Select this folder:
echo.
echo    %EXT_DIR%
echo.
echo Opening chrome://extensions now...
echo.
pause

start chrome "chrome://extensions"

echo.
echo Once Chrome is open, select this folder:
echo %EXT_DIR%
echo.
pause
