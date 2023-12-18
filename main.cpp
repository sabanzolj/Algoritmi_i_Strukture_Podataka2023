#include <bits/types/clock_t.h>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;
template <typename TipKljuca, typename TipVrijednosti> class Mapa {

public:
    Mapa() {};
    virtual TipVrijednosti operator[](TipKljuca key) const = 0;
    virtual TipVrijednosti &operator[](TipKljuca key) = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &key) = 0;
    virtual ~Mapa() {};
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {

    std::pair<TipKljuca, TipVrijednosti> **kolekcija;
    int kapacitet;
    int size;

public:
    NizMapa() {
        kapacitet = 50;
        size = 0;
        kolekcija = new std::pair<TipKljuca, TipVrijednosti> *[kapacitet];
    }

    void TestKapaciteta() {
        if (size != kapacitet)
            return;

        kapacitet=kapacitet*2;
        auto temp =new std::pair<TipKljuca, TipVrijednosti> *[kapacitet];

        for (int i = 0; i < size; i++) {
            temp[i] = kolekcija[i];
        }


        delete[] kolekcija;
        kolekcija = temp;
    }

    TipVrijednosti operator[](TipKljuca key) const override {
        for (int i = 0; i < size; i++) {
            if (kolekcija[i]->first == key) {
                return kolekcija[i]->second;
            }
        }
        return TipVrijednosti();
    }

    TipVrijednosti &operator[](TipKljuca key) override {
        TestKapaciteta();

        for (int i = 0; i < size; i++) {
            if (kolekcija[i]->first == key) {
                return kolekcija[i]->second;
            }
        }

        kolekcija[size++] = new std::pair<TipKljuca, TipVrijednosti>(key, TipVrijednosti());

        return kolekcija[size - 1]->second;
    }

    int brojElemenata() const override { return size; }

    void obrisi() override {
        for (int i = 0; i < size; i++) {
            delete kolekcija[i];
            kolekcija[i] = nullptr;
        }
        size = 0;
    }

    void obrisi(const TipKljuca &key) override {
        for (int i = 0; i < size; i++) {
            if (kolekcija[i]->first == key) {
                delete kolekcija[i];
                for (int j = i; j < size - 1; j++) {
                    kolekcija[j] = kolekcija[j + 1];
                }
                kolekcija[size - 1] = nullptr;
                size--;
                return;
            }
        }
        throw std::domain_error("Element sa unesnim kljucem ne postoji");
    }

    NizMapa(const NizMapa<TipKljuca,TipVrijednosti> &m){
        size=m.size;
        kapacitet=m.kapacitet;
        kolekcija=new std::pair<TipKljuca,TipVrijednosti>*[kapacitet];
        for(int i=0;i<size;i++){
            kolekcija[i]=new std::pair<TipKljuca,TipVrijednosti>(*m.kolekcija[i]);
        }
    }

    NizMapa<TipKljuca,TipVrijednosti> &operator=(const NizMapa<TipKljuca,TipVrijednosti> &m){
        if(&m==this)return *this;

        for (int i = 0; i < size; i++) {
            delete kolekcija[i];
            kolekcija[i] = nullptr;
        }
        delete[] kolekcija;
        kolekcija=nullptr;
        size=0;
        kapacitet=m.kapacitet;
        size=m.size;
        kolekcija=new std::pair<TipKljuca,TipVrijednosti>*[kapacitet];
        for(int i=0;i<size;i++){
            kolekcija[i]=new std::pair<TipKljuca,TipVrijednosti>(*m.kolekcija[i]);
        }
        return *this;



    }

    ~NizMapa() override {
        for (int i = 0; i < size; i++) {
            delete kolekcija[i];
            kolekcija[i] = nullptr;
        }

        delete[] kolekcija;
        kolekcija = nullptr;
        size = 0;
    }
};

template<typename NekiTipKey, typename NekiTipValue>
struct Cvor{
    Cvor *parent;
    Cvor *leftChild;
    Cvor *rightChild;
    NekiTipKey key;
    NekiTipValue element;
};

template<typename NekiTipKey, typename NekiTipValue>
class BinStabloMapa:public Mapa<NekiTipKey, NekiTipValue>{

    int broj_elemenata;
    void pOrder(Cvor<NekiTipKey, NekiTipValue> **p){
        if((*p)->leftChild!=nullptr || (*p)->rightChild!=nullptr){
            if((*p)->leftChild!=nullptr){
                pOrder(&((*p)->leftChild));
            }

            if((*p)->rightChild!=nullptr){
                pOrder(&((*p)->rightChild));
            }
        }

        (*p)->parent=nullptr;
        (*p)->leftChild=nullptr;
        (*p)->rightChild=nullptr;
        delete *p;
        *p=nullptr;
    }

    void pOrderCopy(Cvor<NekiTipKey, NekiTipValue> *p){
        if(p->leftChild!=nullptr || p->rightChild!=nullptr){
            if(p->rightChild!=nullptr){
                pOrderCopy(p->rightChild);
            }
            if(p->leftChild!=nullptr){
                pOrderCopy(p->leftChild);
            }
        }
        (*this)[p->key]=p->element;
    }

public:

    Cvor<NekiTipKey, NekiTipValue> *root;

    BinStabloMapa():broj_elemenata(0),root(nullptr){}

    int brojElemenata()const override{
        return broj_elemenata;
    }

    void obrisi() override{
        Cvor<NekiTipKey, NekiTipValue> **temp=&root;
        pOrder(temp);
        broj_elemenata=0;
        root=nullptr;
    }

    void obrisi(const NekiTipKey &kljuc) override{
        Cvor<NekiTipKey, NekiTipValue> *temp1=root;
        Cvor<NekiTipKey, NekiTipValue> *temp2=nullptr;
        Cvor<NekiTipKey, NekiTipValue> *tmp=nullptr;
        Cvor<NekiTipKey, NekiTipValue> *pp=nullptr;
        Cvor<NekiTipKey, NekiTipValue> *rp=nullptr;

        while(temp1!=nullptr && temp1->key!=kljuc){
            temp2=temp1;

            if(temp1->key>kljuc){
                temp1=temp1->leftChild;
            }
            else if(temp1->key<kljuc){
                temp1=temp1->rightChild;
            }
        }

        if(temp1==nullptr){
            return;
        }

        if(temp1->leftChild==0){
            rp=temp1->rightChild;
        }
        else if(temp1->rightChild==0){
            rp=temp1->leftChild;
        }

        else {
            pp=temp1;
            rp=temp1->leftChild;
            tmp=rp->rightChild;

            while(tmp!=0){
                pp=rp;
                rp=tmp;
                tmp=rp->rightChild;
            }

            if(pp!=temp1){
                pp->rightChild=rp->leftChild;
                rp->leftChild=temp1->leftChild;
            }
            rp->rightChild=temp1->rightChild;
        }

        if(temp2==0){
            root=rp;
        }

        else if(temp1==temp2->leftChild){
            temp2->leftChild=rp;
        }
        else{
            temp2->rightChild=rp;
        }

        delete temp1;
        broj_elemenata--;

    }

    BinStabloMapa(const BinStabloMapa &m):broj_elemenata(0),root(nullptr){
        pOrderCopy(m.root);
    }

    BinStabloMapa &operator=(const BinStabloMapa &m){
        if(&m==this){
            return *this;
        }
        obrisi();
        pOrderCopy(m.root);
        return *this;
    }

    NekiTipValue &operator[](NekiTipKey kljuc)override{
        Cvor<NekiTipKey,NekiTipValue> **p=&root;
        Cvor<NekiTipKey,NekiTipValue> *pair=nullptr;

        while(*p!=nullptr && kljuc!=(*p)->key){
            pair=*p;
            if(kljuc<(*p)->key){
                p=&((*p)->leftChild);
            }
            else{
                p=&((*p)->rightChild);
            }
        }

        if(*p==nullptr){
            *p=new Cvor<NekiTipKey, NekiTipValue>{pair,nullptr,nullptr,kljuc,NekiTipValue()};

            broj_elemenata++;
        }

        return (*p)->element;
    }

    NekiTipValue operator[](NekiTipKey kljuc)const override{
        auto p=root;
        while(p!=nullptr && kljuc!=p->key){
            if(kljuc<p->key){
                p=p->leftChild;
            }
            else{
                p=p->rightChild;
            }
        }

        return p==nullptr ? NekiTipValue():p->element;
    }

    ~BinStabloMapa()override{
        if(broj_elemenata==0){
            return;
        }
        obrisi();
    }

};


int main(){

    BinStabloMapa<double, int> binarnamapa;
    NizMapa<double, int> nizmapa;

    double tmp=0;

    for(int i=0;i<10000;i++){
        double temp=std::rand();
        if(i==5000){
            tmp=temp;
        }
        nizmapa[temp]=i;
        binarnamapa[temp]=i;
    }

    clock_t time1=clock();
    nizmapa[-12]=1000;
    clock_t time2=clock();

    std::cout<<"Vrijeme dodavanja elementa u niz mapu je "<<(time2-time1)/(CLOCKS_PER_SEC/1000)<<" ms."<<std::endl;

    time1=clock();
    binarnamapa[-12]=1000;
    time2=clock();

    std::cout<<"Vrijeme dodavanja elementa u binarnu mapu je "<<(time2-time1)/(CLOCKS_PER_SEC/1000)<<" ms."<<std::endl;

    time1=clock();
    binarnamapa[tmp];
    time2=clock();

    std::cout<<"Vrijeme pristupa elementu u binarnoj mapi je "<<(time2-time1)/(CLOCKS_PER_SEC/1000)<<" ms."<<std::endl;


    time1=clock();
    nizmapa[tmp];
    time2=clock();

    std::cout<<"Vrijeme pristupa elementu u niz mapi je "<<(time2-time1)/(CLOCKS_PER_SEC/1000)<<" ms."<<std::endl;
    return 0;
}
