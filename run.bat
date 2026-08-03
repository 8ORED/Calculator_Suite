@echo off
title DSA Project
color 0B

if not exist CalcSuite.exe (
    echo =====================================
    echo Executable not found.
    echo Please run setup.bat first.
    echo =====================================
    pause
    exit /b
)

cls
echo =====================================
echo      RUNNING DSA PROJECT
echo =====================================
echo.

CalcSuite.exe

echo.
pause