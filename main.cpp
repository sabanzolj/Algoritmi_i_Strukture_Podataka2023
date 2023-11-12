#include <iostream>
#include <stdexcept>
#include <cassert>

using namespace std;

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
class NizLista:public Lista<NekiTip>{
    int kapacitet;
    int broj_elemenata;
    NekiTip **kontejner;
    int trenutniElement;
public:
    NizLista():broj_elemenata(0),trenutniElement(-1),kapacitet(100){
        kontejner=new NekiTip*[kapacitet];
        /*for(int i=0;i<kapacitet;i++){
            kontejner[i]= new NekiTip();
        }*/
    }
    NizLista(const NizLista &other) : broj_elemenata(other.broj_elemenata), trenutniElement(other.trenutniElement), kapacitet(other.kapacitet) {
        kontejner = new NekiTip*[kapacitet];
        for (int i = 0; i < broj_elemenata; i++) {
            kontejner[i] = new NekiTip(*(other.kontejner[i]));
        }
    }

    NizLista& operator=(const NizLista &other) {
        if (this == &other) {
            return *this;
        }

        // Oslobodi postojeće resurse
        for (int i = 0; i < broj_elemenata; i++) {
            delete kontejner[i];
        }
        delete[] kontejner;

        broj_elemenata = other.broj_elemenata;
        trenutniElement = other.trenutniElement;
        kapacitet = other.kapacitet;

        kontejner = new NekiTip*[kapacitet];
        for (int i = 0; i < broj_elemenata; i++) {
            kontejner[i] = new NekiTip(*(other.kontejner[i]));
        }
        return *this;
    }


    int brojElemenata()const override {return broj_elemenata;}

    NekiTip trenutni()const override{
        if(broj_elemenata==0)throw std::logic_error("Lista je prazna");
        return *kontejner[trenutniElement];}

    NekiTip &trenutni()override{
        if(broj_elemenata==0)throw std::logic_error("Lista je prazna");
        return *kontejner[trenutniElement];}

    bool prethodni()override{
        if(broj_elemenata==0)throw std::logic_error("Lista je prazna");
        else if(trenutniElement==0)return false;
        else{
            trenutniElement--;
            return true;
        }
    }
    bool sljedeci()override{
        if(broj_elemenata==0)throw std::logic_error("Lista je prazna");
        else if(trenutniElement==broj_elemenata-1)return false;
        else{
            trenutniElement++;
            return true;
        }

    }

    void pocetak()override{
        if(broj_elemenata==0)throw std::logic_error("Lista je prazna");
        trenutniElement=0;
    }

    void kraj()override{
        if(broj_elemenata==0)throw std::logic_error("Lista je prazna");
        trenutniElement=broj_elemenata-1;
    }

   void obrisi() override {
    if (broj_elemenata == 0)
        throw std::logic_error("Lista je prazna");

    int pozicija = trenutniElement;
    delete kontejner[pozicija];

    for (int i = pozicija; i < broj_elemenata - 1; i++) {
        kontejner[i] = kontejner[i + 1];
    }

    if (trenutniElement == broj_elemenata - 1)
        trenutniElement--;

    broj_elemenata--;
}



    void dodajIspred(const NekiTip &element) override {
        if (broj_elemenata == kapacitet) {
        kapacitet *= 2;
        NekiTip **noviKontejner = new NekiTip*[kapacitet];
        for (int i = 0; i < broj_elemenata; i++) {
            noviKontejner[i] = kontejner[i];
        }

        delete[] kontejner;
        kontejner = noviKontejner;
    }

    if (broj_elemenata == 0) {
        kontejner[0] = new NekiTip(element);
        trenutniElement = 0;
        broj_elemenata++;
    } else {
        for (int i = broj_elemenata; i > trenutniElement; i--) {
            kontejner[i] = kontejner[i - 1];
        }
        kontejner[trenutniElement] = new NekiTip(element);
        broj_elemenata++;
        trenutniElement++;
    }
    }



    void dodajIza(const NekiTip &element) override{
        if (broj_elemenata == kapacitet) {
            kapacitet *= 2;
            NekiTip **noviKontejner = new NekiTip*[kapacitet];
            for (int i = 0; i < broj_elemenata; i++) {
                noviKontejner[i] = kontejner[i];
            }

            kontejner=nullptr;
            kontejner = noviKontejner;
        }

        if (broj_elemenata == 0) {
            kontejner[0] = new NekiTip(element);
            trenutniElement = 0;
            broj_elemenata++;
        } else {
            if (trenutniElement < 0) {
                trenutniElement = 0;
            }
            for (int i = broj_elemenata; i > trenutniElement + 1; i--) {
                kontejner[i] = kontejner[i - 1];
            }
            kontejner[trenutniElement + 1] = new NekiTip(element);
            broj_elemenata++;
        }
    }


    NekiTip &operator[](int i)override{
        if(i<0 || i>=broj_elemenata)throw std::range_error("Indeks van opsega");
        return *kontejner[i];
    }

    NekiTip operator[](int i)const override{
        if(i<0 || i>=broj_elemenata)throw std::range_error("Indeks van opsega");
        return *kontejner[i];
    }

    ~NizLista(){

        for(int i=0;i<broj_elemenata;i++){
            delete kontejner[i];
        }
        delete [] kontejner;
    }











};

template<typename TipElemenata>
struct Cvor {
    TipElemenata element;
    Cvor<TipElemenata> *veza;
};

template <typename NekiTip>
class JednostrukaLista:public Lista<NekiTip>{
    Cvor<NekiTip> *prvi;
    Cvor<NekiTip> *trenutniC;
    int broj_elemenata;

    void odstrani(){
        Cvor<NekiTip> *del=prvi;
        while (del) {
        Cvor<NekiTip> *temp=del;
        del=del->veza;
        delete temp;
        temp=nullptr;
        }
    }
public:
    JednostrukaLista():prvi(nullptr),trenutniC(nullptr),broj_elemenata(0){}

    JednostrukaLista(const JednostrukaLista &temp):broj_elemenata(temp.broj_elemenata),prvi(nullptr){

        Cvor<NekiTip> *before=nullptr;
        Cvor<NekiTip> *tmp=temp.prvi;
        while(tmp){
            Cvor<NekiTip> *noviC=new Cvor<NekiTip>();
            noviC->element=tmp->element;
            noviC->veza=nullptr;
            if(!prvi){
                prvi=noviC;

            }
            else{
                before->veza=noviC;
            }
            before=noviC;
            tmp=tmp->veza;
        }
    }

    JednostrukaLista &operator=(const JednostrukaLista temp){
        if(this==&temp){
            return *this;
        }

        odstrani();
        broj_elemenata=temp.broj_elemenata;
        prvi=nullptr;
        Cvor<NekiTip> *before=nullptr;
        Cvor<NekiTip> *tmp=temp.prvi;
        while (tmp) {
        Cvor<NekiTip> *noviC=new Cvor<NekiTip>();
        noviC->element=tmp->element;
        noviC->veza=nullptr;
        if(!prvi){
            prvi=noviC;
        }
        else{
            before->veza=noviC;
        }
        before=noviC;
        tmp=tmp->veza;
        }

        return *this;
    }

    int brojElemenata()const override{
        return broj_elemenata;
    }

    NekiTip &trenutni()override{
        if(broj_elemenata==0){
            throw std::logic_error("Lista je prazna");
        }
        return trenutniC->element;
    }

    NekiTip trenutni()const override{
        if(broj_elemenata==0){
            throw std::logic_error("Lista je prazna");
        }
        return trenutniC->element;
    }

    bool prethodni()override{
        if(broj_elemenata==0) throw std::logic_error("Lista je prazna");
        if(trenutniC==prvi){
            return false;
        }

        Cvor<NekiTip> *tmp=prvi;
        while (tmp->veza!=trenutniC) {
        tmp=tmp->veza;
        }
        trenutniC=tmp;
        return true;
    }

    bool sljedeci()override{
        if(broj_elemenata==0) throw std::logic_error("Lista je prazna");
        if(trenutniC->veza==nullptr){
            return false;
        }
        trenutniC=trenutniC->veza;
        return true;

    }

    void pocetak()override{
        if(broj_elemenata==0) throw std::logic_error("Lista je prazna");
        trenutniC=prvi;
    }

    void kraj()override{

        if(broj_elemenata==0) throw std::logic_error("Lista je prazna");
        while (trenutniC->veza!=nullptr) {
        trenutniC=trenutniC->veza;
        }
    }

    void obrisi()override{

        if(broj_elemenata==0) throw std::logic_error("Lista je prazna");

        if(trenutniC==prvi){
            trenutniC=trenutniC->veza;
            Cvor<NekiTip> *noviC=prvi;
            prvi=prvi->veza;
            delete noviC;
            broj_elemenata--;
            return;
        }

        Cvor<NekiTip> *noviC=prvi;
        while (noviC->veza!=trenutniC) {
        noviC=noviC->veza;
        }

        if (noviC->veza != nullptr && noviC->veza->veza == nullptr) {
            trenutniC = noviC;
         }
         else{
             trenutniC=trenutniC->veza;
         }

         Cvor<NekiTip> *zaBrisanje=noviC->veza;
         noviC->veza=noviC->veza->veza;
         delete zaBrisanje;
         broj_elemenata--;

    }

    void dodajIspred(const NekiTip &element)override{
        Cvor<NekiTip> *novi=new Cvor<NekiTip>();
        novi->element=element;
        novi->veza=nullptr;

        broj_elemenata++;

        if(prvi== nullptr){
            prvi=novi;
            trenutniC=prvi;
            return;
        }

        if(trenutniC==prvi){
            novi->veza=prvi;
            prvi=novi;
            return;
        }

            Cvor<NekiTip> *prethodniC=prvi;
            Cvor<NekiTip> *tmp=prvi;
            while (tmp!=trenutniC){
                prethodniC=tmp;
                tmp=tmp->veza;
            }
            Cvor<NekiTip> *noviC=prethodniC->veza;
            prethodniC->veza=novi;
            novi->veza=noviC;

    }

    void dodajIza(const NekiTip &element)override{
        Cvor<NekiTip> *novi=new Cvor<NekiTip>();
        novi->element=element;
        novi->veza=nullptr;


        if(prvi== nullptr){
            prvi=novi;
            trenutniC=prvi;
            broj_elemenata++;
            return;
        }

        Cvor<NekiTip> *noviC=trenutniC->veza;
        trenutniC->veza=novi;
        novi->veza=noviC;
        broj_elemenata++;
    }

    NekiTip &operator[](int i)override{
        if(i<0 || i>=broj_elemenata){
            throw std::range_error("Indeks van opsega");
        }

        Cvor<NekiTip> *temp=prvi;
        for(int j=0;j<i;j++){
            temp=temp->veza;
        }
        return temp->element;
    }

    NekiTip operator[](int i)const override{
        if(i<0 || i>=broj_elemenata){
            throw std::range_error("Indeks van opsega");
        }

        Cvor<NekiTip> *temp=prvi;
        for(int j=0;j<i;j++){
            temp=temp->veza;
        }
        return temp->element;
    }

    ~JednostrukaLista(){
        odstrani();
    }




};

template <typename NekiTip>
class Stek{
    JednostrukaLista<NekiTip> lista;
    public:
    Stek(){}

    void brisi(){
        int br_el=brojElemenata();
        for(int i=0;i<br_el;i++){
            skini();
        }

    }

    void stavi(const NekiTip &element){
        lista.dodajIspred(element);
        lista.pocetak();

    }

    NekiTip skini(){
        if(!lista.brojElemenata()){
            throw std::range_error("Stek je prazan!");
        }
        NekiTip temp=lista.trenutni();
        lista.obrisi();
        return temp;

    }

    NekiTip &vrh(){
        if(lista.brojElemenata()==0){
            throw std::range_error("Stek je prazan!");
        }

        return lista.trenutni();

    }

    int brojElemenata(){
        return lista.brojElemenata();
    }

    Stek(const Stek &S):lista(S.lista){
        lista.pocetak();
    }

    Stek &operator=(const Stek &S) {
    if (this == &S) {
        return *this;
    }

    lista = S.lista;
    lista.pocetak();

    return *this;
}



    ~Stek(){}







};

void testStavi() {
    Stek<int> s;
    s.stavi(5);
    assert(s.brojElemenata() == 1);
    assert(s.vrh() == 5);
}

void testSkini() {
    Stek<int> s;
    s.stavi(10);
    s.stavi(20);
    assert(s.brojElemenata() == 2);
    assert(s.skini() == 20);
    assert(s.brojElemenata() == 1);
}

void testVrh() {
    Stek<int> s;
    s.stavi(15);
    s.stavi(25);
    assert(s.vrh() == 25);
}

void testBrisi() {
    Stek<int> s;
    s.stavi(3);
    s.stavi(6);
    s.stavi(9);
    s.brisi();
    assert(s.brojElemenata() == 0);
}

int main(){

testStavi();
    testSkini();
    testVrh();
    testBrisi();

    std::cout << "Testovi uspješno izvršeni." << std::endl;

    return 0;
}