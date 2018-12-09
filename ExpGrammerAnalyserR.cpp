#include"WordAnalyser.cpp"
typedef int(functype)(Token&now);
functype E,E1,T,T1,F;
String str;

#include<stack>
std::stack<Token>opn;

int tmpcnt=1;
const char* STR(Token&x){return (*id2string[x.type])[x.id].c_str();}
int E(Token&now){
    if(T(now)>0)return ERROR_CODE;
    if(E1(now)>0)return ERROR_CODE;
    return ERROR_CODE;
}
int E1(Token&now){
    if(now.type==_B_)
    if(boundary[now.id]=="+"||
       boundary[now.id]=="-"){
        const char*op=STR(now);
        str+=(*id2string[now.type])[now.id];
        now=next();
        if(T(now)>0)return ERROR_CODE;
        Token second=opn.top();opn.pop();
        Token first=opn.top();opn.pop();
        char num[10];
        sprintf(num,"t%d",tmpcnt++);
        String tmp=num;
        Token target=SaveAndReturn(tmp,_I_);
        opn.push(target);
        colorPrintf(2,"(%s,%s,%s,%s)\n",op,STR(first),STR(second),STR(target));
        if(E1(now)>0)return ERROR_CODE;
        return ERROR_CODE;
    }return ERROR_CODE;
}
int T(Token&now){
    if(F(now)>0)return ERROR_CODE;
    if(T1(now)>0)return ERROR_CODE;
    return ERROR_CODE;
}
int T1(Token&now){
    if(now.type==_B_)
    if(boundary[now.id]=="*"||
       boundary[now.id]=="/"){
        const char*op=STR(now);
        str+=(*id2string[now.type])[now.id];
        now=next();
        if(F(now)>0)return ERROR_CODE;
        Token second=opn.top();opn.pop();
        Token first=opn.top();opn.pop();
        char num[10];
        sprintf(num,"t%d",tmpcnt++);
        String tmp=num;
        Token target=SaveAndReturn(tmp,_I_);
        opn.push(target);
        colorPrintf(2,"(%s,%s,%s,%s)\n",op,STR(first),STR(second),STR(target));
        if(T1(now)>0)return ERROR_CODE;
        return ERROR_CODE;
    }return ERROR_CODE;
}
int F(Token&now){
    if(now.type==_I_||
       now.type==_N_||
       now.type==_C_){
        str+=(*id2string[now.type])[now.id];
        opn.push(now);//push(i)
        now=next();
    }
    else if(now.type==_B_&&boundary[now.id]=="("){
        str+=(*id2string[now.type])[now.id];
        now=next();
        if(E(now)>0)return ERROR_CODE;
        if(now.type==_B_&&boundary[now.id]==")"){
            str+=(*id2string[now.type])[now.id];
            now=next();
        }else{
            ERROR_CODE=13;
        }
    }
    else
        ERROR_CODE=12;
    return ERROR_CODE;
}
int main(){
#ifdef LOCAL_DEBUG
    freopen("in2.cpp","r",stdin);
#endif
    getNextChar();init();
    Token get=next();
    int cnt=0;
    while(ERROR_CODE==0){//no error >0 and no end of file ==-1
        str="";
        while(opn.size())
            opn.pop();
        E(get);
        if(get.type==_B_&&
            boundary[get.id]!="+"&&
            boundary[get.id]!="-"&&
            boundary[get.id]!="*"&&
            boundary[get.id]!="/"&&
            boundary[get.id]!="("
            ||get.type==-1&&ERROR_CODE==-1)
            get=next();
        else ERROR_CODE=11;
        if(ERROR_CODE<=0){
            printf("Expression %2d:%s->t%d\n",++cnt,str.c_str(),tmpcnt-1);
        }
        else COLS-=str.length();
    }
    dealWithException();
    return 0;
}