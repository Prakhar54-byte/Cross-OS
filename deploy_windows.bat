@echo off
:: OS DropZone - Windows Build & Packaging Script
:: Run this from a Visual Studio Developer Command Prompt or with CMake and Qt in your PATH.

setlocal enabledelayedexpansion

echo ===================================================
echo   OS DropZone Windows Deployment Tool
echo ===================================================

:: Step 1: Check environment prerequisites
where cmake >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [ERROR] CMake was not found on your PATH.
    echo Please run this script from a VS Developer Command Prompt or add CMake to your PATH.
    exit /b 1
)

where rc >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [WARNING] Windows Resource Compiler (rc.exe) was not found.
    echo Make sure you are using a VS Developer Command Prompt so the icon gets embedded correctly.
)

:: Step 2: Set up directories
set PROJECT_DIR=%~dp0
set BUILD_DIR=%PROJECT_DIR%build-windows
set RELEASE_DIR=%BUILD_DIR%\Release
set DIST_DIR=%PROJECT_DIR%dist-windows

echo Project Dir: %PROJECT_DIR%
echo Build Dir:   %BUILD_DIR%
echo Release Dir: %RELEASE_DIR%
echo.

if exist "%BUILD_DIR%" (
    echo Cleaning old build directory...
    rmdir /s /q "%BUILD_DIR%"
)
mkdir "%BUILD_DIR%"

:: Step 3: Run CMake Configure & Build
cd "%PROJECT_DIR%"
echo Configuring CMake...
cmake -S . -B "%BUILD_DIR%" -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% neq 0 (
    echo [ERROR] CMake configuration failed.
    exit /b %ERRORLEVEL%
)

echo Building OS DropZone in Release mode...
cmake --build "%BUILD_DIR%" --config Release
if %ERRORLEVEL% neq 0 (
    echo [ERROR] CMake build failed.
    exit /b %ERRORLEVEL%
)

:: Step 4: Run windeployqt
echo.
echo Running windeployqt to collect Qt dependencies...
:: Try to find windeployqt in typical locations if not on PATH
where windeployqt >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo windeployqt is not directly on PATH, searching standard Qt installations...
    for /d %%D in ("C:\Qt\6.*") do (
        for /d %%V in ("%%D\msvc*") do (
            if exist "%%V\bin\windeployqt.exe" (
                set PATH=%%V\bin;!PATH!
                echo Found windeployqt at: %%V\bin\windeployqt.exe
            )
        )
    )
)

where windeployqt >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo [ERROR] windeployqt.exe not found on PATH or standard C:\Qt folders.
    echo Please add Qt's bin directory to your PATH or run in the Qt Command Prompt.
    exit /b 1
)

:: Run windeployqt targeting the release executable and specifying the QML directory
windeployqt --verbose 1 --qmldir "%PROJECT_DIR%" "%RELEASE_DIR%\cross-os.exe"
if %ERRORLEVEL% neq 0 (
    echo [WARNING] windeployqt completed with warnings/errors. 
    echo Please verify that all Qt DLLs and the QML engine folder are in build-windows\Release.
)

:: Copy icon.ico to Release directory for packaging
copy "%PROJECT_DIR%icon.ico" "%RELEASE_DIR%\" >nul

:: Step 5: Check and compile Inno Setup Installer
echo.
echo Checking for Inno Setup compiler (ISCC)...
where ISCC >nul 2>nul
if %ERRORLEVEL% neq 0 (
    if exist "C:\Program Files (x86)\Inno Setup 6\ISCC.exe" (
        set PATH="C:\Program Files (x86)\Inno Setup 6";!PATH!
        echo Found ISCC at typical installation directory.
    )
)

where ISCC >nul 2>nul
if %ERRORLEVEL% equ 0 (
    echo Compiling Installer using Inno Setup...
    if exist "%DIST_DIR%" rmdir /s /q "%DIST_DIR%"
    mkdir "%DIST_DIR%"
    ISCC "%PROJECT_DIR%installer.iss"
    if %ERRORLEVEL% equ 0 (
        echo.
        echo ===================================================
        echo SUCCESS! Windows Setup Installer created:
        echo   %DIST_DIR%\OS_DropZone_Windows_Setup.exe
        echo ===================================================
    ) else (
        echo [ERROR] Inno Setup compilation failed.
    )
) else (
    echo [INFO] Inno Setup compiler (ISCC.exe) was not found. 
    echo Standalone executable and DLLs are successfully compiled in:
    echo   %RELEASE_DIR%
    echo To create a single installer, install Inno Setup 6 and run:
    echo   ISCC installer.iss
)

echo.
pause
