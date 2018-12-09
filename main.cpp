#include"WordAnalyser.cpp"
int main(){
#ifdef LOCAL_DEBUG
    freopen("in.cpp","r",stdin);
#endif
    getNextChar();init();
    Token get;int cnt[7]={};
    while(ERROR_CODE==0)//no error >0 and no end of file ==-1
        while(Invalid!=(get=next()))
            colorPrintf(get.type,"%4d of %s %d\t%s\n",
                ++cnt[get.type],description[get.type],get.id,(*id2string[get.type])[get.id].c_str());
    dealWithException();
    return 0;
}