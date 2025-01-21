@echo off
echo Compiling test_edge_cases.c...
gcc allocator.c test_edge_cases.c -o test_edge_cases.exe -Wall -O2
if %ERRORLEVEL% == 0 (
    echo Compilation successful.
    echo Running test_edge_cases.exe...
    test_edge_cases.exe
) else (
    echo Compilation failed.
)
pause
