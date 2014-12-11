#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
//#define debug
using namespace std;
typedef vector<int> vi;
const int N=100000+10;
const int MinSup=300;
struct Pair {
    int x,p;
    Pair() {}
    Pair(int _x,int _p) {
        x=_x;
        p=_p;
    }
};
struct Scope {
    int tid;    //tree id
    vi m;       //match label of the k-1 length prefix
    Pair s;     //scope of the last item
    Scope() {}
    Scope(int _tid,vi _m,int lx,Pair _s) {
        tid=_tid;
        m=_m;
        m.push_back(lx);
        s=_s;
    }
};
struct Element {
    Pair label;
    vector<Scope> scopes;
    bool check() {
        int cnt=0,pre=-1;
        for(int i=0; i<scopes.size(); i++) {
            int tid=scopes[i].tid;
            if(tid!=pre) cnt++, pre=tid;
        }
        return cnt>=MinSup;
    }
} element[N];
struct Subtree {
    vi prefix;
    vector<Element> elements;
    void output() {
        if(elements.size()==0) return;
        printf("prefix: ");
        for(int i=0; i<prefix.size(); i++) printf("%d ",prefix[i]);
        puts("");
        for(int i=0; i<elements.size(); i++) {
            //puts("----------");
            printf("%d %d\n",elements[i].label.x,elements[i].label.p);
            #ifdef debug
            vector<Scope> &scopes=elements[i].scopes;
            for(int j=0; j<scopes.size(); j++) {
                printf("%d,",scopes[j].tid);
                for(int k=0; k<scopes[j].m.size(); k++)
                    printf("%d%c",scopes[j].m[k],k+1==scopes[j].m.size()?',':' ');
                printf("[%d,%d]\n",scopes[j].s.x,scopes[j].s.p);
            }
            #endif
        }
    }
};
void pt(vi v) {
    for(int i=0; i<v.size(); i++) printf("%d ",v[i]);
    puts("");
}
void work(const vi &v,int tid) {
    #ifdef debug
        for(int i=0; i<v.size(); i++)
            printf("%d ",v[i]);
        puts("\n~~~");
    #endif
    int lx=0;
    for(int i=0; i<v.size(); i++) {
        if(v[i]==-1) continue;
        Scope scope;
        scope.tid=tid;
        scope.s.x=lx;
        for(int j=i+1,tot=1,ux=lx; j<v.size(); j++) {
            if(v[j]==-1) tot--;
            else tot++, ux++;
            if(tot==0) {
                scope.s.p=ux;
                element[v[i]].label=Pair(v[i],-1);
                element[v[i]].scopes.push_back(scope);
                break;
            }
        }
        ++lx;
    }
}
void Prefix_Generate(const vi &pre1 ,const Pair &label,vi &pre2) {
    pre2=pre1;
    for(int i=0,fa=0; i<pre1.size(); i++) {
        if(pre1[i]==-1) continue;
        if(fa++==label.p) {
            int cnt=0;
            for(int j=i+1; j<pre1.size(); j++) {
                if(pre1[j]!=-1) cnt++;
                else cnt--;
            }
            for(int j=0; j<cnt; j++) pre2.push_back(-1);
            break;
        }
    }
    pre2.push_back(label.x);
}
void TMdfs(Subtree P) {
    P.output();
    for(int i=0; i<P.elements.size(); i++) {
        Subtree PP;
        Pair px=P.elements[i].label;
        Prefix_Generate(P.prefix,px,PP.prefix);
        vector<Scope> &scopex=P.elements[i].scopes;
        for(int j=0; j<P.elements.size(); j++) {
            Pair py=P.elements[j].label;
            vector<Scope> &scopey=P.elements[j].scopes;
            //In-scope Test
            if(px.p==py.p) {
                Element eleIn;
                eleIn.label=Pair(py.x,py.p+1); //(y,j+1)
                for(int r=0; r<scopex.size(); r++)
                    for(int t=0; t<scopey.size(); t++) {
                        if((&scopex[r])==(&scopey[t])) continue;
                        if(scopex[r].tid==scopey[t].tid&&scopex[r].m==scopey[t].m&&scopex[r].s.x<=scopey[t].s.x&&scopex[r].s.p>=scopey[t].s.p) {
                            eleIn.scopes.push_back(Scope(scopex[r].tid,scopex[r].m,scopex[r].s.x,scopey[t].s));
                        }
                    }
                if(eleIn.check()) PP.elements.push_back(eleIn);
            }
            //Out-scope Test
            if(py.p!=-1) {
                Element eleOut;
                eleOut.label=Pair(py.x,py.p); //(y,j)
                for(int r=0; r<scopex.size(); r++)
                    for(int t=0; t<scopey.size(); t++) {
                        if(scopex[r].tid==scopey[t].tid&&scopex[r].m==scopey[t].m&&scopex[r].s.p<scopey[t].s.x) {
                            eleOut.scopes.push_back(Scope(scopex[r].tid,scopex[r].m,scopex[r].s.x,scopey[t].s));
                        }
                    }
                if(eleOut.check()) PP.elements.push_back(eleOut);
            }
        }
        TMdfs(PP);
    }
}
void F1() {
    int i,tot=0,tid=0;
    vi v;
    while(~scanf("%d",&i)) {
        v.push_back(i);
        if(i==-1&&--tot==0) {
            work(v,tid++);
            v.clear();
        } else if(i!=-1&&++tot);
    }
    Subtree P1;
    for(int j=1; j<N; j++) {
        if(element[j].check()) P1.elements.push_back(element[j]);
    }
    TMdfs(P1);
}
int main() {
    //freopen("test.txt","r",stdin);
    freopen("treedata/F5.data","r",stdin);
    freopen("F5.data.out","w",stdout);
    F1();
    return 0;
}
