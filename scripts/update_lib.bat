@echo off
setlocal

set "BUILD_DIR=cmake-build-debug"
set "TARGET_DIR=python\masterchip8"

if not exist "%TARGET_DIR%" mkdir "%TARGET_DIR%"

echo Searching for masterchip8_py.pyd in %BUILD_DIR%...

for /r "%BUILD_DIR%" %%f in (masterchip8_py*.pyd) do (
    echo Found: %%f
    copy /Y "%%f" "%TARGET_DIR%\"
    echo Done!
    goto :eof
)

echo Error: Could not find masterchip8_py*.pyd
echo Did you build the project?
exit /b 1
