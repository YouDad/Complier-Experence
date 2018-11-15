#pragma once
#include<stdarg.h>
#include<Windows.h>
void colorPrintf(int id,const char*format,...){
    HANDLE hwnd=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hwnd,8+id);
    va_list ap;va_start(ap,format);
    vprintf(format,ap);va_end(ap);
    SetConsoleTextAttribute(hwnd,7);
}
#define _throw_(str) colorPrintf(-4,str);break
void dealWithException(){
    if(ERROR_CODE>0)
        colorPrintf(7,"Line %d:%d ",LINE,COLS);
    switch(ERROR_CODE){
        case 0:_throw_("Complie Over\n");

        case 1:_throw_("ERROR 1:Invaild real literial\n");
        case 2:_throw_("ERROR 2:Character not ending\n");
        case 3:_throw_("ERROR 3:String not ending\n");

        case 11:_throw_("ERROR 11:Expression not ending\n");
        case 12:_throw_("ERROR 12:Invaild expression\n");
        case 13:_throw_("ERROR 13:Expression parenthesis does not match\n");
        default:;
    }
}
#undef _throw_