@call debug
call set_path.bat
call get_filename.bat "%~1"
echo 开始编译%filename%.cpp
g++ %filename%.cpp -o %filename%.exe -ggdb3 -O0 -DLOCAL_DEBUG
echo 编译完成