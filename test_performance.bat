@echo off
echo Compiling test_performance.c...
gcc allocator.c test_performance.c -o test_performance.exe -Wall -O2
if %ERRORLEVEL% == 0 (
    echo Compilation successful.
    echo Running test_performance.exe...
    test_performance.exe
) else (
    echo Compilation failed.
)
pause
