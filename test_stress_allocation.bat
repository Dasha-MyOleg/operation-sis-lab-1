@echo off
echo Compiling test_stress_allocation.c...
gcc allocator.c test_stress_allocation.c -o test_stress_allocation.exe -Wall -O2
if %ERRORLEVEL% == 0 (
    echo Compilation successful.
    echo Running test_stress_allocation.exe...
    test_stress_allocation.exe
) else (
    echo Compilation failed.
)
pause
