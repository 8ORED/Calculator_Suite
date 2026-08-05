@echo off
REM ============================================================
REM  build_and_run.bat
REM  Compiles and runs Calc-Suite (all 4 modules + CLI).
REM  Requires g++ (MinGW / MSYS2) to be installed and on PATH.
REM  Run this from the folder containing all the .cpp/.h files.
REM ============================================================

echo Compiling Calc-Suite...

g++ -std=c++17 -Wall -o calcsuite.exe main.cpp symbol_table.cpp expression_engine.cpp data_storage.cpp huffman.cpp

REM %errorlevel% is set by g++: 0 means success, non-zero means it failed.
if %errorlevel% neq 0 (
    echo.
    echo Build FAILED. Fix the errors above and try again.
    pause
    exit /b %errorlevel%
)

echo Build succeeded.
pause