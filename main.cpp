#include <iostream>
#include <string>
using namespace std;

template<typename TipKljuca, typename TipVrijednosti>
class Mapa{
    public:
    Mapa(){}
    virtual ~Mapa(){}
    virtual TipVrijednosti &operator[](TipKljuca k)=0;
    virtual TipVrijednosti operator[](const TipKljuca k) const=0;
    virtual int brojElemenata() const=0;
    virtual void obrisi()=0;
    virtual void obrisi(const TipKljuca& kljuc)=0;
};

template <typename TipKljuca, typename TipVrijednosti>
struct Cvor{
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    Cvor *roditelj=nullptr,*lijevo_podstablo=nullptr,*desno_podstablo=nullptr;
    int balans_cvora=0;

    Cvor(const TipKljuca &key, const TipVrijednosti &value, Cvor *r=nullptr, Cvor *l=nullptr, Cvor *d=nullptr){
        kljuc=key;
        vrijednost=value;
        roditelj=r;
        lijevo_podstablo=l;
        desno_podstablo=d;
        balans_cvora=0;
    }
};

template<typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa: public Mapa<TipKljuca, TipVrijednosti>{
    int velicina;
    Cvor<TipKljuca, TipVrijednosti> *korijen=nullptr;
    public:
    BinStabloMapa():velicina(0),korijen(nullptr){}
    void kopiraj(Cvor<TipKljuca,TipVrijednosti> *p){
        if(p!=nullptr){
            (*this)[p->kljuc]=p->vrijednost;
            kopiraj(p->lijevo_podstablo);
            kopiraj(p->desno_podstablo);
        }
    }
    BinStabloMapa(const BinStabloMapa<TipKljuca, TipVrijednosti> &bsm){
        if(bsm.velicina!=0) kopiraj(bsm.korijen);
        velicina=bsm.velicina;
    }
    BinStabloMapa &operator=(const BinStabloMapa<TipKljuca,TipVrijednosti> &bsm){
        if(this!=&bsm){
            obrisi();
            kopiraj(bsm.korijen);
            velicina=bsm.velicina;
        }
        return *this;
    }
    ~BinStabloMapa(){
        if(velicina!=0) while(korijen!=nullptr) obrisi(korijen->kljuc);
    }
    int brojElemenata()const{return velicina;}
    void obrisi(const TipKljuca &ukloniti){
        Cvor<TipKljuca,TipVrijednosti> *p=korijen, *q=nullptr,*s=nullptr;
        while(p!=nullptr && (p->kljuc)!=ukloniti){
            q=p;
            if(p->kljuc<ukloniti) p=p->desno_podstablo;
            else p=p->lijevo_podstablo;
        }
        if(p==nullptr) return;
        if(p->lijevo_podstablo==nullptr) s=p->desno_podstablo;
        else if(p->desno_podstablo==nullptr) s=p->lijevo_podstablo;
        else{
            s=p->lijevo_podstablo;
            Cvor<TipKljuca,TipVrijednosti> *r,*o;
            r=s->desno_podstablo;
            o=p;
            while(r!=nullptr){
                o=s;
                s=r;
                r=s->desno_podstablo;
            }
            if(p!=o){
                o->desno_podstablo=s->lijevo_podstablo;
                s->lijevo_podstablo=p->lijevo_podstablo;
            }
            s->desno_podstablo=p->desno_podstablo;
        }
        if(q==nullptr) korijen=s;
        else{
            if(q->lijevo_podstablo==p) q->lijevo_podstablo=s;
            else q->desno_podstablo=s;
        }
        velicina--;
        delete p;
    }
    void obrisi(){
        while(korijen!=nullptr) obrisi(korijen->kljuc);
        velicina=0;
        korijen=nullptr;
    }
    TipVrijednosti operator[](const TipKljuca key)const{
        Cvor<TipKljuca,TipVrijednosti> *root=korijen;
        while(root!=nullptr){
            if(root->kljuc==key) return root->vrijednost;
            else if(root->kljuc<key) root=root->desno_podstablo;
            else root=root->lijevo_podstablo;
        }
        if(root==nullptr) return TipVrijednosti();
        return root->vrijednost;
    }
    TipVrijednosti &operator[](TipKljuca key){
        Cvor<TipKljuca,TipVrijednosti> *p=korijen;
        while(p!=nullptr){
            if(p->kljuc==key) return p->vrijednost;
            else if(p->kljuc<key) p=p->desno_podstablo;
            else p=p->lijevo_podstablo;
        }
        if(p==nullptr){
            velicina++;
            Cvor<TipKljuca,TipVrijednosti> *root=korijen,*novi=nullptr;
            p=new Cvor<TipKljuca,TipVrijednosti>(key,TipVrijednosti());
            while(root!=nullptr){
                novi=root;
                if(root->kljuc>p->kljuc) root=root->lijevo_podstablo;
                else root=root->desno_podstablo;
            }

        if(novi==nullptr) korijen=p;
        else if(novi->kljuc>p->kljuc){
            novi->lijevo_podstablo=p;
            p->roditelj=novi;}
        else{
            novi->desno_podstablo=p;
            p->roditelj=novi;}
        }
        return p->vrijednost;
    }
};

template<typename TipKljuca, typename TipVrijednosti>
class NizMapa: public Mapa<TipKljuca,TipVrijednosti>{
    int velicina, kapacitet;
    pair<TipKljuca,TipVrijednosti> *array_map;
    public:
    NizMapa(){
        velicina=0;
        kapacitet=2;
        array_map=new pair<TipKljuca,TipVrijednosti>[kapacitet];
    }
    NizMapa(const NizMapa<TipKljuca,TipVrijednosti> &nm){
        velicina=nm.velicina;
        kapacitet=nm.kapacitet;
        array_map=new pair<TipKljuca,TipVrijednosti> [nm.kapacitet];
        for(int i=0;i<nm.velicina;i++) array_map[i]=nm.array_map[i];
    }
    NizMapa &operator=(const NizMapa<TipKljuca,TipVrijednosti> &nm){
        if(this==&nm) return *this;
        delete[] array_map;
        velicina=nm.velicina;
        kapacitet=nm.kapacitet;
        array_map=new pair<TipKljuca,TipVrijednosti> [nm.kapacitet];
        for(int i=0;i<velicina;i++) array_map[i]=nm.array_map[i];
        return *this;
    }
    ~NizMapa(){
        delete[] array_map;
    }
    void obrisi(){
        velicina=0;
        kapacitet=2;
        delete[] array_map;
        array_map=new pair<TipKljuca,TipVrijednosti> [kapacitet];
    }
    void obrisi(const TipKljuca& k){
        bool pronadjen=false;
        for(int i=0;i<velicina;i++)
            if(k==array_map[i].first) pronadjen=true;
        if(!pronadjen) throw domain_error("Element sa tim kljucem ne postoji!");
        for(int i=0;i<velicina;i++){
            if(k==array_map[i].first){
                velicina--;
                for(int j=i;j<velicina;j++) array_map[j]=array_map[j+1];
                break;
            }
        }
    }
    int brojElemenata()const{return velicina;}
    TipVrijednosti &operator[](TipKljuca k){
        for(int i=0;i<velicina;i++) if(k==array_map[i].first) return array_map[i].second;
    if(velicina==kapacitet){
        kapacitet*=2;
        pair<TipKljuca,TipVrijednosti> *pomocni=new pair<TipKljuca,TipVrijednosti> [kapacitet];
        for(int i=0;i<velicina;i++) pomocni[i]=array_map[i];
        delete[] array_map;
        array_map=pomocni;
        pomocni=nullptr;
    }
    TipVrijednosti v=TipVrijednosti();
    array_map[velicina].first=k;
    array_map[velicina].second=v;
    velicina++;
    return array_map[velicina-1].second;
    }
    TipVrijednosti operator[](const TipKljuca k) const{
        TipVrijednosti v=TipVrijednosti();
        bool pronadjen=false;
        int vrijednost;
        for(int i=0;i<velicina;i++){
            if(k==array_map[i].first){
                pronadjen=true;
                vrijednost=i;
                break;
            }
        }
        if(pronadjen) return array_map[vrijednost].second;
        return v;
    }
};
template<typename TipKljuca, typename TipVrijednosti>
class AVLStabloMapa: public Mapa<TipKljuca,TipVrijednosti>{
    int velicina;
    Cvor<TipKljuca,TipVrijednosti> *korijen=nullptr;
    public:

    AVLStabloMapa():velicina(0), korijen(nullptr){}
    void kopiraj(Cvor<TipKljuca,TipVrijednosti> *p){
        if(p!=nullptr){
            (*this)[p->kljuc]=p->vrijednost;
            kopiraj(p->lijevo_podstablo);
            kopiraj(p->desno_podstablo);
        }
    }
    AVLStabloMapa(const AVLStabloMapa &avl){
        if(avl.velicina!=0) kopiraj(avl.korijen);
        velicina=avl.velicina;
    }
    AVLStabloMapa &operator=(const AVLStabloMapa &avl){
        if(this!=&avl){
            obrisi();
            kopiraj(avl.korijen);
            velicina=avl.velicina;
        }
        return *this;
    }
    ~AVLStabloMapa(){
        if(velicina!=0) obrisi();
    }
    TipVrijednosti operator[](const TipKljuca key)const{
        Cvor<TipKljuca,TipVrijednosti> *kopija_korijena=korijen;
        while(kopija_korijena!=nullptr){
            if(kopija_korijena->kljuc==key) return kopija_korijena->vrijednost;
            else if(kopija_korijena->kljuc>key) kopija_korijena=kopija_korijena->lijevo_podstablo;
            else kopija_korijena=kopija_korijena->desno_podstablo;
        }
        if(!kopija_korijena) return TipVrijednosti();
        return kopija_korijena->vrijednost;
    }
    TipVrijednosti &operator[](TipKljuca key){
        Cvor<TipKljuca,TipVrijednosti> *p=korijen;
        while(p!=nullptr){
            if(p->kljuc==key) return p->vrijednost;
            else if(p->kljuc<key) p=p->desno_podstablo;
            else p=p->lijevo_podstablo;
        }
        if(p==nullptr){
            velicina++;
            Cvor<TipKljuca,TipVrijednosti> *root=korijen, *novi=nullptr;
            p=new Cvor<TipKljuca,TipVrijednosti>(key, TipVrijednosti());
            while(root!=nullptr){
                novi=root;
                if(root->kljuc>p->kljuc) root=root->lijevo_podstablo;
                else root=root->desno_podstablo;
            }
            if(novi==nullptr) korijen=p;
            else if(novi->kljuc>p->kljuc){
                novi->lijevo_podstablo=p;
                p->roditelj=novi;
            }
            else{
                novi->desno_podstablo=p;
                p->roditelj=novi;
            }
            if(novi==nullptr) return p->vrijednost;
            if(novi->lijevo_podstablo && novi->desno_podstablo) novi->balans_cvora=0;
            bool flag=false;
            if(novi->desno_podstablo) flag=true;
            azurirajBalans(novi,flag);
        }
        return p->vrijednost;
    }

    void rotirajUlijevo(Cvor<TipKljuca,TipVrijednosti> *cvor){
        if(cvor==nullptr) return;
        Cvor<TipKljuca,TipVrijednosti> *pom;
        pom=cvor->desno_podstablo->lijevo_podstablo;
        cvor->balans_cvora=0;
        Cvor<TipKljuca,TipVrijednosti> *right;
        right=cvor->desno_podstablo;
        Cvor<TipKljuca,TipVrijednosti> *right_parent=cvor->roditelj;
        right->roditelj=right_parent;
        right->lijevo_podstablo=cvor;
        bool flag=false;
        if(!right->roditelj && !flag){
            korijen=right;
            flag=true;
        }
        else if(right_parent->desno_podstablo==cvor && !flag){
            right->roditelj->desno_podstablo=right;
            flag=true;
        }
        if(!flag) right->roditelj->lijevo_podstablo=right;
        right->balans_cvora=0;
        if(pom) pom->roditelj=cvor;
        cvor->desno_podstablo=pom;
        cvor->roditelj=right;
    }
    void rotirajUdesno(Cvor<TipKljuca,TipVrijednosti> *cvor){
        if(cvor==nullptr) return;
        Cvor<TipKljuca,TipVrijednosti> *pom;
        pom=cvor->lijevo_podstablo->desno_podstablo;
        cvor->balans_cvora=0;
        auto left=cvor->lijevo_podstablo;
        auto left_parent=cvor->roditelj;
        bool flag=false;
        left->roditelj=left_parent;
        left->desno_podstablo=cvor;
        if(!left->roditelj ){
            korijen=left;
            flag=true;
        }
        else if(left_parent->desno_podstablo==cvor ){
            left->roditelj->desno_podstablo=left;
            flag=true;
        }
        if(!flag) left->roditelj->lijevo_podstablo=left;
        left->balans_cvora=0;
        if(pom) pom->roditelj=cvor;
        cvor->lijevo_podstablo=pom;
        cvor->roditelj=left;
    }

    void azurirajBalans(Cvor<TipKljuca,TipVrijednosti> *cvor,bool flag=false){
       if(cvor->roditelj==nullptr) return;
       else if(flag) cvor->balans_cvora=cvor->balans_cvora-1;
       else cvor->balans_cvora=cvor->balans_cvora+1;
       int balance=cvor->balans_cvora;
       if(cvor->roditelj!=nullptr){
           if(cvor->roditelj->desno_podstablo==cvor) flag=true;
           else flag=false;
       }
       if(balance>1){
           if(cvor->lijevo_podstablo->balans_cvora==1) rotirajUdesno(cvor);
           else{
               rotirajUlijevo(cvor->lijevo_podstablo);
               rotirajUdesno(cvor);
           }
       }
       else if(balance<-1){
           if(cvor->desno_podstablo->balans_cvora==-1) rotirajUlijevo(cvor);
           else{
               rotirajUdesno(cvor->desno_podstablo);
               rotirajUlijevo(cvor);
           }
       }
       else{
           if(!balance) return;
           azurirajBalans(cvor->roditelj,flag);
       }
   }

    void obrisi(){
        while(korijen!=nullptr) obrisi(korijen->kljuc);
        korijen=nullptr;
        velicina=0;
    }

    void obrisi(const TipKljuca &ukloniti){
        Cvor<TipKljuca, TipVrijednosti> *p=korijen, *q=nullptr, *s=nullptr;
        while(p!=nullptr && (p->kljuc)!=ukloniti){
            q=p;
            if(p->kljuc<ukloniti) p=p->desno_podstablo;
            else p=p->lijevo_podstablo;
        }
        if(p==nullptr) return;
        Cvor<TipKljuca,TipVrijednosti> *r,*o;
        if(p->lijevo_podstablo==nullptr) s=p->desno_podstablo;
        else if(p->desno_podstablo==nullptr) s=p->lijevo_podstablo;
        else{
            s=p->lijevo_podstablo;
            r=s->desno_podstablo;
            o=p;
            while(r!=nullptr){
                o=s;
                s=r;
                r=s->desno_podstablo;
            }
            if(p!=o){
                o->desno_podstablo=s->lijevo_podstablo;
                s->lijevo_podstablo=p->lijevo_podstablo;
                o->roditelj=s;
                s->roditelj=q;
                if(o->desno_podstablo) o->desno_podstablo->roditelj=o;
            }
            s->desno_podstablo=p->desno_podstablo;
            p->desno_podstablo->roditelj=s;
        }
        if(q==nullptr){
            korijen=s;
            if(korijen!=nullptr) korijen->roditelj=nullptr;
        }
        else if(p==q->lijevo_podstablo){
            q->lijevo_podstablo=s;
            if(s!=nullptr) s->roditelj=q;
        }
        else{
            q->desno_podstablo=s;
            if(s!=nullptr) s->roditelj=q;
        }
        if(p->roditelj && p->roditelj->lijevo_podstablo==p) p->roditelj->balans_cvora--;
        else if(p->roditelj) p->roditelj->balans_cvora++;
        delete p;
        velicina--;
    }
    int brojElemenata()const{return velicina;}
};
double averageTime(clock_t time1,clock_t time2){
    return((time2-time1)/(CLOCKS_PER_SEC/1000.));
}
int main(){
    NizMapa<int,int> nm;
    BinStabloMapa<int,int> bsm;
    AVLStabloMapa<int, int> avl;
    clock_t time1_nm,time2_nm,time1_bsm,time2_bsm,time1_avl,time2_avl;

    time1_nm=clock();
    for(int i=0;i<10000;i++) nm[rand()]=rand();
    time2_nm=clock();

    time1_bsm=clock();
    for(int i=0;i<10000;i++) bsm[rand()]=rand();
    time2_bsm=clock();

    time1_avl=clock();
    for(int i=0;i<10000;i++) avl[rand()]=rand();
    time2_avl=clock();
    cout<<"Vrijeme potrebno za umetanje 10000 elemenata u NizMapa je: "<<averageTime(time1_nm,time2_nm)<<" ms."<<endl;
    cout<<"Vrijeme potrebno za umetanje 10000 elemenata u BinStabloMapa je: "<<averageTime(time1_bsm,time2_bsm)<<" ms."<<endl;
    cout<<"Vrijeme potrebno za umetanje 10000 elemenata u AVLStabloMapa je: "<<averageTime(time1_avl, time2_avl)<<" ms."<<endl;
    clock_t time3_nm,time4_nm,time3_bsm,time4_bsm,time3_avl,time4_avl;

    time3_nm=clock();
    nm[10001]=rand();
    time4_nm=clock();

    time3_bsm=clock();
    bsm[10001]=rand();
    time4_bsm=rand();

    time3_avl=clock();
    avl[10001]=rand();
    time4_avl=clock();
    cout<<"Vrijeme potrebno za umetanje jednog elementa u NizMapa je: "<<averageTime(time3_nm,time4_nm)<<" ms."<<endl;
    cout<<"Vrijeme potrebno za umetanje jednog elementa u BinStabloMapa je: "<<averageTime(time3_bsm,time4_bsm)<<" ms."<<endl;
    cout<<"Vrijeme potrebno za umetanje jednog elementa u AVLStabloMapa je: "<<averageTime(time1_avl, time2_avl)<<" ms."<<endl;
    clock_t time5_nm,time6_nm,time5_bsm,time6_bsm,time5_avl,time6_avl;

    time5_nm=clock();
    nm[5788];
    time6_nm=clock();

    time5_bsm=clock();
    bsm[5788];
    time6_bsm=clock();

    time5_avl=clock();
    avl[5788];
    time6_avl=clock();
    cout<<"Vrijeme potrebno za pristup postojecem elementu u NizMapa je: "<<averageTime(time5_nm,time6_nm)<<" ms."<<endl;
    cout<<"Vrijeme potrebno za pristup postojecem elementu u BinStabloMapa je: "<<averageTime(time5_bsm,time6_bsm)<<" ms."<<endl;
    cout<<"Vrijeme potrebno za pristup postojecem elementu u AVLStabloMapa je: "<<averageTime(time5_avl, time6_avl);


    return 0;
}