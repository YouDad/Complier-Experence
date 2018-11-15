#include"WordAnalyser.cpp"
typedef int(functype)(Token&now);
functype E,E1,T,T1,F;
String str;
int E(Token&now){
    if(T(now)>0)return ERROR_CODE;
    if(E1(now)>0)return ERROR_CODE;
    return ERROR_CODE;
}
int E1(Token&now){
    if(now.type==_B_)
    if(boundary[now.id]=="+"||
       boundary[now.id]=="-"){
        str+=(*id2string[now.type])[now.id];
        now=next();
        if(T(now)>0)return ERROR_CODE;
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
        str+=(*id2string[now.type])[now.id];
        now=next();
        if(F(now)>0)return ERROR_CODE;
        if(T1(now)>0)return ERROR_CODE;
        return ERROR_CODE;
    }return ERROR_CODE;
}
int F(Token&now){
    if(now.type==_I_||
       now.type==_CO_||
       now.type==_CH_){
        str+=(*id2string[now.type])[now.id];
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
    init();Token get=next();
    int cnt=0;
    while(ERROR_CODE==0){//no error >0 and no end of file ==-1
        str="";E(get);
        if(get.type==_B_&&
			boundary[get.id]!="+"&&
			boundary[get.id]!="-"&&
			boundary[get.id]!="*"&&
			boundary[get.id]!="/"&&
			boundary[get.id]!="("
			||get.type==-1&&ERROR_CODE==-1)
			get=next();
        else ERROR_CODE=11;
        if(ERROR_CODE<=0)
            printf("Expression %2d:%s\n",++cnt,str.c_str());
		else COLS-=str.length();
    }
    dealWithException();
    return 0;
}