@echo off
title DSA Project Setup
color 0A

echo =====================================
echo        DSA PROJECT SETUP
echo =====================================
echo.

g++ --version >nul 2>&1

if %errorlevel% neq 0 (
    echo ERROR: g++ compiler not found.
    echo.
    pause
    exit /b
)

echo g++ compiler found.
echo.

echo Building project...

g++ *.cpp -std=c++17 -o CalcSuite.exe

if %errorlevel% neq 0 (
    echo.
    echo Build Failed!
    pause
    exit /b
)

echo.
echo =====================================
echo Setup Complete!
echo Executable Created:
echo CalcSuite.exe
echo =====================================

pause