#include"WordAnalyser.cpp"
#include<assert.h>
#include<list>
#include<algorithm>
#define foreach(itor,obj,type,inc) for(type::iterator itor=(obj).begin();itor!=(obj).end();inc)
#define foreachR(itor,obj,type,inc) for(type::reverse_iterator itor=(obj).rbegin();itor!=(obj).rend();inc)
Table stateMap;
Vector<String> state(1);
const int _T_=7;//state
const int _A_=8;//action
#define List std::list
struct ProductionNode{
    int type,id;
    ProductionNode(int t,int i):type(t),id(i){}
    bool operator<(const ProductionNode other)const{
        if(type==other.type)
            return id<other.id;
        return type<other.type;
    }
    bool operator==(const ProductionNode other)const{
        return id==other.id&&type==other.type;
    }
    bool operator!=(const ProductionNode other)const{
        return !(*this==other);
    }
};
typedef List<ProductionNode> Production;
Vector<List<Production>>head(1);
Vector<Vector<ProductionNode>>firstSet(1),followSet(1);
Vector<bool>red(1);
int stateid(const char*s){
    static int scnt=1;
    if(!stateMap[s]){
        stateMap[s]=scnt++;
        state.push_back(s);
        head.push_back(List<Production>());
        firstSet.push_back(Vector<ProductionNode>());
        followSet.push_back(Vector<ProductionNode>());
        red.push_back(false);
    }return stateMap[s];
}
Vector<ProductionNode> Follow(ProductionNode x);
Vector<ProductionNode> First(ProductionNode x){
    Vector<ProductionNode> ret;
    if(x.type!=_T_){
        ret.push_back(x);
        return ret;
    }
    foreach(it,head[x.id],List<Production>,it++){
        if(it->size()==0){
            Vector<ProductionNode>_ret=Follow(x);
            ret.insert(ret.end(),_ret.begin(),_ret.end());
            std::sort(ret.begin(),ret.end());
            ret.erase(std::unique(ret.begin(),ret.end()),ret.end());
        }else{
            Vector<ProductionNode>_ret=First(it->front());
            ret.insert(ret.end(),_ret.begin(),_ret.end());
            std::sort(ret.begin(),ret.end());
            ret.erase(std::unique(ret.begin(),ret.end()),ret.end());
        }
    }
    return ret;
}
Vector<ProductionNode> Follow(ProductionNode x){
    if(followSet[x.id].size())return followSet[x.id];
    Vector<ProductionNode> ret;
    if(red[x.id])return ret;
    red[x.id]=true;
    for(int i=1;i<state.size();i++){
        foreach(it,head[i],List<Production>,it++){
            foreach(jt,*it,Production,){
                if(*jt==x){
                    if(++jt!=it->end()){
                        Vector<ProductionNode>_ret=First(*jt);
                        ret.insert(ret.end(),_ret.begin(),_ret.end());
                        std::sort(ret.begin(),ret.end());
                        ret.erase(std::unique(ret.begin(),ret.end()),ret.end());
                    }else if(i!=x.id){
                        Vector<ProductionNode>_ret=Follow(ProductionNode(_T_,i));
                        ret.insert(ret.end(),_ret.begin(),_ret.end());
                        std::sort(ret.begin(),ret.end());
                        ret.erase(std::unique(ret.begin(),ret.end()),ret.end());
                    }
                }else jt++;
            }
        }
    }red[x.id]=false;
    return followSet[x.id]=ret;
}
void iniF(){
    for(uint i=1;i<state.size();i++){
        foreach(it,head[i],List<Production>,it++){
            Vector<ProductionNode>_ret;
            if(it->size()==0)
                _ret=Follow(ProductionNode(_T_,i));
            else _ret=First(it->front());
            foreach(jt,_ret,Vector<ProductionNode>,jt++){
                foreach(kt,firstSet[i],Vector<ProductionNode>,kt++){
                    assert(*jt!=*kt);
                }
            }
            firstSet[i].insert(firstSet[i].end(),_ret.begin(),_ret.end());
            std::sort(firstSet[i].begin(),firstSet[i].end());
            firstSet[i].erase(std::unique(firstSet[i].begin(),firstSet[i].end()),firstSet[i].end());
        }
    }
}
bool inFirst(Token now,ProductionNode node){
    if(node.type==_T_){
        int i=node.id;
        for(uint j=0;j<firstSet[i].size();j++){
            if(firstSet[i][j].id==-1)
                if(firstSet[i][j].type==now.type)
                    return true;
            if(firstSet[i][j].id==now.id&&firstSet[i][j].type==now.type)
                return true;
        }return false;
    }else if(node.id==-1){
        return node.type==now.type;
    }else
        return node.type==now.type&&node.id==now.id;
}
bool inFollow(Token now,ProductionNode node){
    Vector<ProductionNode>_ret=Follow(node);
    foreach(it,_ret,Vector<ProductionNode>,it++)
        if((it->id==now.id||it->id==-1)&&it->type==now.type)
            return true;
    return false;
}
Token last;
Token Next(){
    Token ret=next();static int cnt[7]={};
    while(ret==Invalid&&ERROR_CODE==0)ret=next();
    // if(ret!=Invalid)
    //     colorPrintf(ret.type,"%4d of %s %d\t%s\n",
    //         ++cnt[ret.type],description[ret.type],ret.id,(*id2string[ret.type])[ret.id].c_str());
    dealWithException(false);
    return ret;
}
#include<stack>
void generateProduction(char*u,char*v){
    char*tmp=new char[1024];
    int i=0,j,k=stateid(u);
    Production l;
    while(v[i]){
        sscanf(v+i,"%s%n",tmp,&j);i+=j;
        if('a'<=tmp[0]&&tmp[0]<='z'){
            l.push_back(ProductionNode(_K_,keyWordMap[tmp]));
        }else if('A'<=tmp[0]&&tmp[0]<='Z'){
            l.push_back(ProductionNode(_T_,stateid(tmp)));
        }else if(tmp[0]=='_'){
            switch(tmp[1]){
                case 'I':l.push_back(ProductionNode(_I_,-1));break;
                case 'C':l.push_back(ProductionNode(_C_,-1));break;
                case 'S':l.push_back(ProductionNode(_S_,-1));break;
                case 'N':l.push_back(ProductionNode(_N_,-1));break;
                case 'A':l.push_back(ProductionNode(_A_,tmp[2]));break;
            }
        }else{
            l.push_back(ProductionNode(_B_,boundaryMap[tmp]));
        }
    }head[k].push_back(l);
    delete [] tmp;
}
String grammerInput(const char*filename){
#ifdef LOCAL_DEBUG
    freopen(filename,"r",stdin);
#endif
    char first[1024],second[1024];
    while(~scanf("%s",first)){
        if(String(first,first+5)=="main:")
            break;
        while(1){
            while(getchar()!='`');
            gets(second);
            if(second[0]==0)
                break;
            else
                generateProduction(first,second);
        }
    }return first+5;
}
void eliminateLeftRecursion();
void printProduction(int i,List<Production>::iterator it);
String grammerInit(){
    init();
    String ret=grammerInput("productions.ini");
    eliminateLeftRecursion();
    for(int i=1;i<state.size();i++){
        head[i].sort();
        foreach(it,head[i],List<Production>,){
            if(it->size()==0){it++;continue;}
            ProductionNode front=it->front();
            int cnt=0;
            while(it!=head[i].end()&&it->front()==front)it++,cnt++;
            if(cnt>=2){
                int k;
                if(front.id==-1)
                    switch(front.type){
                        case _I_:k=stateid((state[i]+"@@I").c_str());break;
                        case _S_:k=stateid((state[i]+"@@S").c_str());break;
                        case _C_:k=stateid((state[i]+"@@C").c_str());break;
                        case _N_:k=stateid((state[i]+"@@N").c_str());break;
                        default:assert(0);break;
                    }
                else
                    switch(front.type){
                        case _B_:k=stateid((state[i]+"@"+boundary[front.id]).c_str());break;
                        case _K_:k=stateid((state[i]+"@"+keyWord[front.id]).c_str());break;
                        case _T_:k=stateid((state[i]+"@"+state[front.id]).c_str());break;
                        case _A_:char name[10];sprintf(name,"@%d",front.id);
                                 k=stateid((state[i]+name).c_str());break;
                    }
                foreach(jt,head[i],List<Production>,){
                    if(jt->size()&&jt->front()==front){
                        Production temp=*jt;
                        temp.pop_front();
                        head[k].push_back(temp);
                        it=head[i].erase(jt++);
                    }else jt++;
                }
                Production x;
                x.push_back(front);
                x.push_back(ProductionNode(_T_,k));
                head[i].push_back(x);
            }
        }
    }
    for(int i=1;i<state.size();i++)//output grammer
        foreach(it,head[i],List<Production>,it++){
            printProduction(i,it);
        }
    iniF();
    return ret;
}
int main(){String start=grammerInit();
#ifdef LOCAL_DEBUG
    freopen("in2.cpp","r",stdin);
#endif
    getNextChar();Token get=Token(0,6);
    while(get==Token(0,6)){
        get=Next();
        std::stack<ProductionNode>s;
        foreach(it,head[stateMap[start]],List<Production>,it++){
            if(inFirst(get,it->front())){
                printProduction(stateMap[start],it);
                foreachR(rit,*it,Production,rit++)
                    s.push(*rit);
                break;
            }
        }
        std::stack<Token>opn;
        int tmpcnt=1;
        while(s.size()&&ERROR_CODE<=0){
            ProductionNode now=s.top();
            if(now.type==_T_){
                s.pop();
                foreach(it,head[now.id],List<Production>,it++){
                    if(it->size()==0)
                        if(inFollow(get,now)){
                            printf("%s -> null\n",state[now.id].c_str());
                            break;
                        }
                        else
                            continue;
                    if(inFirst(get,it->front())){
                        printProduction(now.id,it);
                        foreachR(rit,*it,Production,rit++)
                            s.push(*rit);
                        break;
                    }
                }
            }else if(now.type!=_A_){
                if((now.id==-1||now.id==get.id)&&now.type==get.type){
                    s.pop();
                    last=get;
                    get=Next();
                }else{
                    assert(0);
                }
            }else{
                switch(now.id){
                case'i':opn.push(last);break;
                case'+':case'-':case'*':
                case'/':if(opn.size()==0){ERROR_CODE=12;break;}
                        Token second=opn.top();opn.pop();
                        if(opn.size()==0){ERROR_CODE=12;break;}
                        Token first=opn.top();opn.pop();
                        char name[10];sprintf(name,"t%d",tmpcnt++);
                        String tempvarname=name;
                        Token tmpvar=SaveAndReturn(tempvarname,_I_);
                        colorPrintf(14,"(%c,%s,%s,%s)\n",now.id,
                            (*id2string[first.type])[first.id].c_str(),
                            (*id2string[second.type])[second.id].c_str(),
                            (*id2string[tmpvar.type])[tmpvar.id].c_str()
                        );
                        opn.push(tmpvar);break;
                }s.pop();
            }
        }
        dealWithException();
    }
    return 0;
}
void printProduction(int i,List<Production>::iterator it){
    printf("%s -> ",state[i].c_str());
    foreach(jt,*it,Production,jt++){
        switch(jt->type){
            case _T_:printf("%s ",state[jt->id].c_str());break;
            case _A_:printf("action%c ",jt->id);break;
            default:
                if(jt->id!=-1)
                    printf("%s ",(*id2string[jt->type])[jt->id].c_str());
                else
                    switch(jt->type){
                        case _I_:printf("_I_ ");break;
                        case _S_:printf("_S_ ");break;
                        case _C_:printf("_C_ ");break;
                        case _N_:printf("_N_ ");break;
                    }
        }
    }
    printf("\b\n");
}
inline void eliminateLeftRecursion(){
    for(int i=1;i<(int)state.size();i++){
        for(int j=1;j<i;j++){
            //用(前面的)Pj产生式替换Pi中的类似Pi->Pjxxx产生式
            foreach(it,head[i],List<Production>,){
                if(it->size()&&it->front()==ProductionNode(_T_,j)){
                    foreach(jt,head[j],List<Production>,jt++){
                        head[i].push_back(*it);
                        head[i].back().pop_front();
                        foreachR(kt,*jt,Production,kt++)
                            head[i].back().push_front(*kt);
                    }head[i].erase(it++);
                }else it++;
            }
        }
        //消除Pi的左递归
        bool have=false;//存在左递归不?
        foreach(it,head[i],List<Production>,it++)//遍历产生式
            if(it->size()&&it->front()==ProductionNode(_T_,i))//查看头结点
                have=true;
        if(!have)continue;//不存在
        int j=stateid((state[i]+"`").c_str());//添加新状态Pi`
        head[j].push_back(Production());//添加空产生式Pi`->null
        foreach(it,head[i],List<Production>,){//遍历产生式
            if(it->size()&&it->front()==ProductionNode(_T_,i)){//左递归
                head[j].push_back(*it);//P`->Pa
                head[j].back().pop_front();//P`->a
                head[j].back().push_back(ProductionNode(_T_,j));//P`->aP`
                head[i].erase(it++);
            }else{//非左递归产生式
                it->push_back(ProductionNode(_T_,j));
                it++;
            }
        }
    }
}