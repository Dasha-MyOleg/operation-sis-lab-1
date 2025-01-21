@echo off
echo Compiling test_free.c...
gcc allocator.c test_free.c -o test_free.exe -Wall -O2
if %ERRORLEVEL% == 0 (
    echo Compilation successful.
    echo Running test_free.exe...
    test_free.exe
) else (
    echo Compilation failed.
)
pause
