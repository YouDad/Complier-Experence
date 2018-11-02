@call debug
if "%include_set_path%" == "true" goto :eof
set include_set_path=true
set acm_path=%cd%