@call debug
call set_path.bat
call get_filename.bat "%~1"
echo ��ʼ����%filename%.cpp
g++ %filename%.cpp -o %filename%.exe -ggdb3 -O0 -DLOCAL_DEBUG -DLOCAL_DEBUGER
echo �������
call run_cpp "%~1"