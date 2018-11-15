#pragma once
#include<stdio.h>
#include<string>
#include<vector>
#include<map>
int ERROR_CODE,LINE=1,COLS=1;
#include"AnalyzeError.cpp"
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
const int _K_=1,_I_=2,_CO_=3,_CH_=4,_S_=5,_B_=6;
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
        return SaveAndReturn(now,_I_);
    return SaveAndReturn(now,_K_);
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
    }return SaveAndReturn(now,_CO_);
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
    return SaveAndReturn(now,_CO_);
}
Token nextCharacter(String now){
    bool in=true,transfer=false;
    while(transfer||nowchar!='\''){
        transfer=!transfer&&nowchar=='\\';
        now+=nowchar;getNextChar();
		if(nowchar==-1){
			ERROR_CODE=2;
			return Invalid;
		}
    }now+="'";getNextChar();
    return SaveAndReturn(now,_CH_);
}
Token nextString(String now){
    bool in=true,transfer=false;
    while(transfer||nowchar!='"'){
        transfer=!transfer&&nowchar=='\\';
        now+=nowchar;getNextChar();
		if(nowchar==-1){
			ERROR_CODE=3;
			return Invalid;
		}
    }now+="\"";getNextChar();
    return SaveAndReturn(now,_S_);
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
    }return SaveAndReturn(now,_B_);
}
#define nextif(condition,function) \
else if(condition){getNextChar();return function(param);}
Token next(String param=""){
    param=String(1,nowchar);
    if(nowchar==EOF){
        ERROR_CODE=-1;//End of File
        return Invalid;
    }else if(isBlank(nowchar)){
        do getNextChar();
        while(isBlank(nowchar));
        return next();
    }
    nextif(isIdfirst(nowchar)              ,nextIdentifier)
    nextif(isDigit(nowchar)                ,nextConst)
    nextif(nowchar=='\''                   ,nextCharacter)
    nextif(nowchar=='"'                    ,nextString)
    nextif(nowchar=='.'&&isDigit(nowchar)  ,nextReal)
    nextif(true                            ,nextBoundary)
}
inline void init(){
    getNextChar();Invalid.id=Invalid.type=-1;
    for(int i=0;i<keyWord .size();i++)keyWordMap [keyWord [i]]=i;
    for(int i=0;i<boundary.size();i++)boundaryMap[boundary[i]]=i;
}