#include<stdio.h>
#include<string>
#include<vector>
#include<map>
int ERROR_CODE,LINE=1,COLS=1;
char nowchar;//now visiting character
bool getNextChar(){//return is'\n'?
    nowchar=getchar();COLS++;
    if(nowchar=='\n'){
        nowchar=getchar(),LINE++,COLS=1;
        return true;
    }else return false;
}
#define Vector std::vector
typedef std::string String;
typedef std::map<String,int> Table;//string->id
const int KEYWORD=1,IDENTIFIER=2,CONSTANT=3,CHARACTER=4,STRING=5,BOUNDARY=6;
const char description[7][11]={"","KeyWord   ","Identifier","Constant  ","Character ","String    ","Boundary  "};
const String _keyWord[]={"goto","break","continue","false","true","int","float","void","long","double","for","while","return","do","char","sizeof","struct","class","static","const","auto","register","if","else","switch","case","default","enum","union","this","typedef","using","try","catch","throw","delete","new","extern","inline","namespace","operator","friend","private","public","protected"};
const String _boundary[]={";","[","]","(",")","{","}",",","<",">","=","+","-","|","/","?",".","~","!","%","^","&","*",":","->","::","++","--","<<",">>","<=",">=","==","!=","&&","||","+=","-=","*=","/=","%=","&=","^=","|=","<<=",">>="};
Table keyWordMap,identifierMap,constantMap,characterMap,stringMap,boundaryMap;
Vector<String> keyWord(_keyWord,_keyWord+45),identifier,constant,character,string,boundary(_boundary,_boundary+46);
Table *string2id[7]={0,&keyWordMap,&identifierMap,&constantMap,&characterMap,&stringMap,&boundaryMap};
Vector<String>*id2string[7]={0,&keyWord,&identifier,&constant,&character,&string,&boundary};
inline bool isDigit(char x){return '0'<=x&&x<='9';}
inline bool isHex(char x){return isDigit(x)||'a'<=x&&x<='f'||'A'<=x&&x<='F';}
inline bool isOct(char x){return '0'<=x&&x<='7';}
inline bool isLowerCase(char x){return 'a'<=x&&x<='z';}
inline bool isAlpha(char x){return isLowerCase(x)||'A'<=x&&x<='Z';}
inline bool isIdfirst(char x){return isAlpha(x)||x=='_'||x=='$';}
inline bool isBlank(char x){return x==' '||x=='\t';}
struct Token{
    int id,type;
    Token(int i=0,int t=0):id(i),type(t){}
    inline bool operator==(const Token&other){return id==other.id&&type==other.type;}
    inline bool operator!=(const Token&other){return !(*this==other);}
}Invalid;
inline Token SaveAndReturn(String&now,int type){
    if(!string2id[type]->count(now)){
        id2string[type]->push_back(now);
        (*string2id[type])[now]=id2string[type]->size()-1;
        return Token(id2string[type]->size()-1,type);
    }return Token((*string2id[type])[now],type);
}
Token nextIdentifier(String now){
    while(isIdfirst(nowchar)||isDigit(nowchar)){
        now+=nowchar;getNextChar();
    }
    if(!keyWordMap.count(now))
        return SaveAndReturn(now,IDENTIFIER);
    return SaveAndReturn(now,KEYWORD);
}
Token nextReal(String now){
    bool e=false,dot=false;
    if(now!="."){
        dot=nowchar=='.';
        if((nowchar|32)=='e')
            dot=true,e=true;
        getNextChar();
    }if(dot&&!e)now+=".";if(e)now+="e";
    if(e&&!isDigit(nowchar)){ERROR_CODE=1;return Invalid;}//invaild real literial
    while(isDigit(nowchar)||(!e&&(nowchar|32)=='e')){
        if(!e)e=(nowchar|32)=='e';
        now+=nowchar;getNextChar();
    }
    if((nowchar|32)=='l'){
        now+=nowchar;getNextChar();
    }return SaveAndReturn(now,CONSTANT);
}
Token nextConst(String now){
    bool(*function)(char chr)=isDigit;
    if(now[0]=='0'){
        if((nowchar|32)=='x'){//hex
            now+=nowchar;getNextChar();
            function=isHex;
        }else{//oct
            function=isOct;
        }
    }int LL=0;//no suffix or L or LL
    while(LL==0&&function(nowchar)||LL<=1&&(nowchar|32)=='l'){
        LL+=(nowchar|32)=='l';
        now+=nowchar;getNextChar();
    }//suffix is b or B
    if((nowchar|32)=='b')
        now+=nowchar,getNextChar();
    if(LL==0&&(nowchar=='.'||(nowchar|32)=='e'))
        return nextReal(now);
    return SaveAndReturn(now,CONSTANT);
}
Token nextCharacter(String now){
    bool in=true,transfer=false;
    while(transfer||nowchar!='\''){
        transfer=!transfer&&nowchar=='\\';
        now+=nowchar;getNextChar();
    }now+="'";getNextChar();
    return SaveAndReturn(now,CHARACTER);
}
Token nextString(String now){
    bool in=true,transfer=false;
    while(transfer||nowchar!='"'){
        transfer=!transfer&&nowchar=='\\';
        now+=nowchar;getNextChar();
    }now+="\"";getNextChar();
    return SaveAndReturn(now,STRING);
}
Token nextBoundary(String now){
    if(now[0]=='/'&&nowchar=='/'){//line commit
        while(!getNextChar());
        return Invalid;
    }
    if(now[0]=='/'&&nowchar=='*'){//block commit
        while(1){
            do getNextChar();while(nowchar!='*');
            getNextChar();
            if(nowchar=='/'){
                getNextChar();
                return Invalid;
            }
        }
    }
    while(boundaryMap.count(now+nowchar)){
        now+=nowchar;getNextChar();
    }return SaveAndReturn(now,BOUNDARY);
}
#define nextif(condition,whilecond,function) \
else if(condition){do getNextChar();while(whilecond);return function(param);}
Token next(String param=""){
    param=String(1,nowchar);
    if(nowchar==EOF)return Invalid;//End of File
    nextif(isBlank(nowchar),isBlank(nowchar) ,next)
	nextif(isIdfirst(nowchar)              ,0,nextIdentifier)
    nextif(isDigit(nowchar)                ,0,nextConst)
    nextif(nowchar=='\''                   ,0,nextCharacter)
    nextif(nowchar=='"'                    ,0,nextString)
    nextif(nowchar=='.'&&isDigit(nowchar)  ,0,nextReal)
    nextif(true                            ,0,nextBoundary)
}
#include<stdarg.h>
#include<Windows.h>
void colorPrintf(int id,const char*format,...){
    HANDLE hwnd=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hwnd,8+id);
    va_list ap;va_start(ap,format);
    vprintf(format,ap);va_end(ap);
    SetConsoleTextAttribute(hwnd,7);
}
int main(){
#ifdef LOCAL_DEBUG
    freopen("in.cpp","r",stdin);
#endif
    getNextChar();Invalid.id=Invalid.type=-1;
    for(int i=0;i<keyWord .size();i++)keyWordMap [keyWord [i]]=i;
    for(int i=0;i<boundary.size();i++)boundaryMap[boundary[i]]=i;
    Token get;int cnt[7]={};
    while(ERROR_CODE==0&&nowchar!=EOF)//no error and no end of file
        while(Invalid!=(get=next()))
            colorPrintf(get.type,"%4d of %s %d\t%s\n",
                ++cnt[get.type],description[get.type],get.id,(*id2string[get.type])[get.id].c_str());
    if(ERROR_CODE)
        colorPrintf(7,"Line %d:%d ",LINE,COLS);
    switch(ERROR_CODE){
        case 1:colorPrintf(7,"ERROR 1:invaild real literial\n");break;
        default:;
    }
    return 0;
}