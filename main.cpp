#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <cassert>

using namespace std;

template <typename NekiTip>
class Iterator;

template <typename NekiTip>
class Lista{
public:
    Lista(){}
    virtual ~Lista(){}
    virtual int brojElemenata()const =0;
    virtual NekiTip &trenutni() =0;
    virtual NekiTip trenutni()const =0;
    virtual bool prethodni() =0;
    virtual bool sljedeci()=0;
    virtual void pocetak()=0;
    virtual void kraj()=0;
    virtual void obrisi()=0;
    virtual void dodajIspred(const NekiTip &element)=0;
    virtual void dodajIza(const NekiTip &element)=0;
    virtual NekiTip &operator[](int i)=0;
    virtual NekiTip operator[](int)const=0;



};

template <typename NekiTip>
struct DoubleCvor{
    DoubleCvor *prethodni;
    DoubleCvor *sljedeci;
    NekiTip element;

};

template <typename NekiTip>
class DvostrukaLista:public Lista<NekiTip>{

    int broj_elemenata;
    DoubleCvor<NekiTip> *trenutniC;
    DoubleCvor<NekiTip> *start;
    DoubleCvor<NekiTip> *end;

    void odstrani(){

        DoubleCvor<NekiTip> *temp = start;
    while (temp != nullptr) {
        DoubleCvor<NekiTip> *sljedeci = temp->sljedeci;
        delete temp;
        temp = sljedeci;
    }
    start = nullptr;
    end = nullptr;
    trenutniC = nullptr;
    broj_elemenata = 0;
    }

public:

DvostrukaLista():broj_elemenata(0), trenutniC(nullptr), start(nullptr), end(nullptr){}



int brojElemenata()const override{
    return broj_elemenata;
}

bool prethodni()override{
    if(broj_elemenata==0)throw std::domain_error("Lista je prazna");

    if(trenutniC->prethodni==nullptr)return false;
    trenutniC=trenutniC->prethodni;
    return true;
}

bool sljedeci()override{
    if(broj_elemenata==0)throw std::domain_error("Lista je prazna");

    if(trenutniC->sljedeci==nullptr)return false;
    trenutniC=trenutniC->sljedeci;
    return true;
}


void pocetak() override{
    if(broj_elemenata==0) throw std::domain_error("Lista je prazna");

    trenutniC=start;
}

void kraj() override{
    if(broj_elemenata==0) throw std::domain_error("Lista je prazna");

    trenutniC=end;
}

NekiTip &trenutni()override{
    if(broj_elemenata==0) throw std::domain_error("Lista je prazna");
    return trenutniC->element;
}

NekiTip trenutni()const override{
    if(broj_elemenata==0) throw std::domain_error("Lista je prazna");
    return trenutniC->element;
}

void obrisi() override{

    if(broj_elemenata==0) throw std::domain_error("Lista je prazna");

    broj_elemenata--;

    DoubleCvor<NekiTip> *temp=trenutniC;

    if(trenutniC->prethodni==nullptr){
        start=trenutniC->sljedeci;
        if(start!=nullptr)start->prethodni=nullptr;
        trenutniC=trenutniC->sljedeci;
    }

    else if(trenutniC->sljedeci==nullptr){
        trenutniC->prethodni->sljedeci=trenutniC->sljedeci;
        trenutniC=trenutniC->prethodni;
    }
    else{
        trenutniC->prethodni->sljedeci=trenutniC->sljedeci;
        trenutniC->sljedeci->prethodni=trenutniC->prethodni;
        trenutniC=trenutniC->sljedeci;
    }

    delete temp;
    temp=nullptr;
}

void dodajIza(const NekiTip &e)override{

    broj_elemenata++;
    DoubleCvor<NekiTip> *noviC=new DoubleCvor<NekiTip>();
    noviC->element=e;
    noviC->prethodni=nullptr;
    noviC->sljedeci=nullptr;

    if(start==nullptr){
        trenutniC=noviC;
        start=noviC;
        end=noviC;

    }

    else if(trenutniC->sljedeci==nullptr){
        trenutniC->sljedeci=noviC;
        noviC->prethodni=trenutniC;
        noviC->sljedeci=nullptr;
        end=noviC;
    }

    else{
        noviC->sljedeci=trenutniC->sljedeci;
        noviC->prethodni=trenutniC;
        trenutniC->sljedeci->prethodni=noviC;
        trenutniC->sljedeci=noviC;
    }
}

void dodajIspred(const NekiTip &e)override{

    broj_elemenata++;

    DoubleCvor<NekiTip> *noviC= new DoubleCvor<NekiTip>();
    noviC->element=e;
    noviC->prethodni=nullptr;
    noviC->sljedeci=nullptr;

    if(start==nullptr){
        trenutniC=noviC;
        start=noviC;
        end=noviC;
    }

    else if(trenutniC->prethodni==nullptr){
        start->prethodni=noviC;
        noviC->sljedeci=start;
        noviC->prethodni=nullptr;
        start=noviC;
    }

    else{

        noviC->sljedeci=trenutniC;
        noviC->prethodni=trenutniC->prethodni;
        trenutniC->prethodni->sljedeci=noviC;
        trenutniC->prethodni=noviC;
    }

}

NekiTip &operator[](int i) override{
    if(i<0 || i>broj_elemenata) throw std::domain_error("Indeks koji ste unijeli je van opsega");

    DoubleCvor<NekiTip> *temp=start;
    for(int a=0;a<i;a++){
        temp=temp->sljedeci;
    }

    return temp->element;
    }




NekiTip operator[](int i)const override{
    if(i<0 || i>broj_elemenata) throw std::domain_error("Indeks koji ste unijeli je van opsega");

    DoubleCvor<NekiTip> *temp=start;
    for(int a=0;a<i;a++){
        temp=temp->sljedeci;
    }

    return temp->element;
    }

    DvostrukaLista(const DvostrukaLista &lista):broj_elemenata(lista.broj_elemenata){
        start=nullptr;
        DoubleCvor<NekiTip> *temp=nullptr;
        auto brojac=lista.start;
        while(brojac){
            auto noviC=new DoubleCvor<NekiTip>{nullptr,nullptr,brojac->element};
            if(!start)start=noviC;
            else temp->sljedeci=noviC;
            temp=noviC;
            brojac=brojac->sljedeci;

        }

}

DvostrukaLista &operator=(const DvostrukaLista &lista) {
    if (&lista == this) {
        return *this;
    }


    broj_elemenata = 0;
    odstrani();

    DoubleCvor<NekiTip> *prethodniCvor = nullptr;
    DoubleCvor<NekiTip> *brojac = lista.start;

    while (brojac != nullptr) {
        DoubleCvor<NekiTip> *noviC = new DoubleCvor<NekiTip>();
        noviC->element = brojac->element;
        noviC->prethodni = prethodniCvor;
        noviC->sljedeci = nullptr;

        if (start == nullptr) {
            start = noviC;
            trenutniC = start;
        } else {
            prethodniCvor->sljedeci = noviC;
        }

        end = noviC;
        broj_elemenata++;

        prethodniCvor = noviC;

        brojac = brojac->sljedeci;
    }

    return *this;
}






~DvostrukaLista(){
    odstrani();
}


friend class Iterator<NekiTip>;

};



template <typename NekiTip>
NekiTip dajMaksimum(const Lista<NekiTip> &l){
    const DvostrukaLista<NekiTip> &novi=static_cast<const DvostrukaLista<NekiTip>&>(l);
    Iterator<NekiTip> it(novi);
    NekiTip max=it.trenutni();
    for(int i=0;i<novi.brojElemenata();i++){
        if(it.trenutni()>max)max=it.trenutni();
        it.sljedeci();
    }
    return max;
}

template <typename NekiTip>
class Iterator{
    const DvostrukaLista<NekiTip> *lista;

    DoubleCvor<NekiTip> *temp;

    public:

    Iterator(const DvostrukaLista<NekiTip> &t):lista(&t){
        if(t.broj_elemenata!=0){
           temp=t.start;
        }

    }

    NekiTip trenutni() const{
        if(lista->broj_elemenata!=0){
            return temp->element;
        }
    }

    NekiTip &trenutni(){
        if(lista->broj_elemenata!=0){
            return temp->element;
        }
    }

    void sljedeci(){
        if(lista->broj_elemenata!=0){
            if(temp->sljedeci!=nullptr){
                temp=temp->sljedeci;
            }
        }
    }

    void prethodni(){
        if(lista->broj_elemenata!=0){
            auto novi = lista->start;
            if(novi != temp){
                while(novi->sljedeci != temp){
                    novi = novi->sljedeci;
                }
                temp = novi;
            }
            else temp = nullptr;
        }
    }

    void pocetak(){
        if(lista->broj_elemenata!=0){
            temp=lista->start;
        }
    }

    void kraj(){}



};

void testTrenutni() {
DvostrukaLista<int> Testlista;
Testlista.dodajIspred(3);
Testlista.dodajIspred(5);
Testlista.dodajIspred(4);
Testlista.dodajIspred(1);
Testlista.prethodni();
if(Testlista.trenutni() != 1) std::cout<<"Nije prosao"<<std::endl;
Testlista.dodajIspred(3);
Testlista.kraj();
if(Testlista.trenutni() != 3) std::cout<<"Nije prosao"<<std::endl;
}

void testDodajIspred() {
DvostrukaLista<int> Testlista;
Testlista.dodajIspred(9);
if(Testlista.trenutni() != 9) std::cout<<"Nije prosao"<<std::endl;
}

void testDodajIza() {
DvostrukaLista<int> Testlista;
Testlista.dodajIza(1);
if(Testlista.trenutni() != 1){
     std::cout<<"Nije prosao"<<std::endl;
}
}

void testPrethodni() {
DvostrukaLista<int> Testlista;
Testlista.dodajIspred(5);
Testlista.dodajIspred(9);
Testlista.dodajIspred(4);
Testlista.prethodni();
if(Testlista.trenutni() != 4) std::cout<<"Nije prosao"<<std::endl;
Testlista.pocetak();
if(Testlista.trenutni() != 9) std::cout<<"Nije prosao"<<std::endl;
}

void testSljedeci() {
DvostrukaLista<int> Testlista;
Testlista.dodajIza(7);
Testlista.dodajIza(4);
Testlista.dodajIza(2);
Testlista.dodajIza(8);
Testlista.sljedeci();
if(Testlista.trenutni() != 8) std::cout<<"Nije prosao"<<std::endl;
}

void testBrojElemenata() {
DvostrukaLista<int> Testlista;
Testlista.dodajIspred(1);
Testlista.dodajIspred(2);
Testlista.dodajIspred(3);
Testlista.dodajIspred(4);
Testlista.dodajIza(5);
Testlista.dodajIza(6);
Testlista.dodajIza(7);
Testlista.dodajIza(8);

if(Testlista.brojElemenata() != 8) std::cout<<"Nije prosao"<<std::endl;

}

void testPocetak() {
DvostrukaLista<int> Testlista;
try {
Testlista.pocetak();
std::cout<<"Nije prosao"<<std::endl;
}
catch(...) {
Testlista.dodajIza(1);
Testlista.dodajIza(2);
Testlista.dodajIza(3);
Testlista.pocetak();

if(Testlista.trenutni() != 1) std::cout<<"Nije prosao"<<std::endl;
}
}

void testKraj() {
DvostrukaLista<int> Testlista;
try {
Testlista.kraj();
std::cout<<"Nije prosao"<<std::endl;
}
catch(...) {
Testlista.dodajIspred(8);
Testlista.dodajIza(6);
Testlista.dodajIspred(5);

Testlista.kraj();
if(Testlista.trenutni() != 6) std::cout<<"Nije prosao"<<std::endl;
}
}

void testOperator() {
DvostrukaLista<int> Testlista;
Testlista.dodajIza(1);
Testlista.dodajIspred(5);
if(Testlista[0] != 5) std::cout<<"Nije prosao"<<std::endl;
}

void testObrisi() {
DvostrukaLista<int> Testlista;
Testlista.dodajIza(1);
Testlista.dodajIza(2);
Testlista.dodajIza(5);
for(int i=0; i<3; i++) Testlista.obrisi();
if(Testlista.brojElemenata() != 0) std::cout<<"Nije prosao"<<std::endl;
}

void testDajMaksimum() {
DvostrukaLista<int> Testlista;
Testlista.dodajIza(1);
Testlista.dodajIza(2);
Testlista.dodajIza(3);
Testlista.dodajIza(4);
if(dajMaksimum(Testlista) != 4) std::cout<<"Nije prosao"<<std::endl;
}



int main(){

testTrenutni();
testDodajIza();
testPrethodni();
testDodajIspred();
testSljedeci();
testBrojElemenata();
testPocetak();
testKraj();
testObrisi();
testOperator();
testDajMaksimum();



return 0;
}