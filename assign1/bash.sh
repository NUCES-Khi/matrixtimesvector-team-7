@echo off

REM Define input sizes and maximum input size
set "input_sizes=64 128 256 512 1024 2048 4096 8192 16384"
set "max_input_size=32768"

REM Set programs' paths
set "sequential_program=sequential_matrix_vector.exe"
set "openmp_naive_program=openmp_naive_matrix_vector.exe"
set "mpi_naive_program=mpi_naive_matrix_vector.exe"
set "openmp_tiled_program=openmp_tiled_matrix_vector.exe"
set "mpi_tiled_program=mpi_tiled_matrix_vector.exe"

REM Function to execute a program with a given input size and measure its time
:run_program
set "program=%~1"
set "size=%~2"
echo Input size: %size%
echo -----------------------------------
echo Sequential program:
call :time_program "%sequential_program%" %size% %size%
echo -----------------------------------
echo OpenMP naive program:
call :time_program "%openmp_naive_program%" %size% %size%
echo -----------------------------------
echo MPI naive program:
call :time_program "%mpi_naive_program%" %size% %size%
echo -----------------------------------
echo OpenMP tiled program:
call :time_program "%openmp_tiled_program%" %size% %size%
echo -----------------------------------
echo MPI tiled program:
call :time_program "%mpi_tiled_program%" %size% %size%
echo -----------------------------------
goto :eof

REM Subroutine to measure program execution time
:time_program
set "start_time=%time%"
%*
set "end_time=%time%"
set "start_time_sec=%start_time:~0,2%*3600+%start_time:~3,2%*60+%start_time:~6,2%"
set "end_time_sec=%end_time:~0,2%*3600+%end_time:~3,2%*60+%end_time:~6,2%"
set /a "execution_time_sec=%end_time_sec%-%start_time_sec%"
echo Execution time: %execution_time_sec% seconds
goto :eof

echo Running tests...

REM Loop through input sizes and run each program
for %%s in (%input_sizes%) do (
    if %%s LEQ %max_input_size% (
        call :run_program %%s
    ) else (
        echo Reached maximum input size. Exiting.
        exit /b
    )
)

echo Tests completed.
