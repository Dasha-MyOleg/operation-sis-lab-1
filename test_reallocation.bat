@echo off
echo Compiling test_reallocation.c...
gcc allocator.c test_reallocation.c -o test_reallocation.exe -Wall -O2
if %ERRORLEVEL% == 0 (
    echo Compilation successful.
    echo Running test_reallocation.exe...
    test_reallocation.exe
) else (
    echo Compilation failed.
)
pause
