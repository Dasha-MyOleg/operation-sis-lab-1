@echo off
echo Compiling all tests...
gcc allocator.c test_stress_allocation.c -o test_stress_allocation.exe -Wall -O2
gcc allocator.c test_reallocation.c -o test_reallocation.exe -Wall -O2
gcc allocator.c test_free.c -o test_free.exe -Wall -O2
gcc allocator.c test_edge_cases.c -o test_edge_cases.exe -Wall -O2
gcc allocator.c test_performance.c -o test_performance.exe -Wall -O2

if %ERRORLEVEL% == 0 (
    echo All tests compiled successfully.
    echo Running tests...
    test_stress_allocation.exe
    test_reallocation.exe
    test_free.exe
    test_edge_cases.exe
    test_performance.exe
) else (
    echo Compilation failed for one or more tests.
)
pause

