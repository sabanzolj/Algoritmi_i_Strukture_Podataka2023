#include <iostream>
#include <utility>
#include <stdexcept>
#include <string>

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
///

template<typename TipKljuca, typename TipVrijednosti>
class HashMapa : public Mapa<TipKljuca, TipVrijednosti> {
    std::pair<TipKljuca, TipVrijednosti>** niz;
    int velicina;
    int kapacitet;
    unsigned int (*funkcija)(TipKljuca, unsigned int);
public:
    void definisiHashFunkciju(unsigned int (*fun)(TipKljuca, unsigned int)) {
        funkcija = fun;
    }
    HashMapa() : velicina(0), kapacitet(1000), funkcija(nullptr) {
        niz = new std::pair<TipKljuca, TipVrijednosti>*[kapacitet];
        for(int i=0; i<kapacitet; i++) niz[i] = nullptr;
    }
    HashMapa(const HashMapa<TipKljuca, TipVrijednosti>& mapa) : velicina(mapa.
            velicina), kapacitet(mapa.kapacitet) , funkcija(mapa.funkcija){
        niz = new std::pair<TipKljuca, TipVrijednosti>*[kapacitet];
        for(int i=0; i<kapacitet; i++) niz[i] = nullptr;
        for(int i=0; i<kapacitet; i++) if(mapa.niz[i]!=nullptr) niz[i] = new std::pair<
                        TipKljuca, TipVrijednosti>(*mapa.niz[i]);
    }
    HashMapa<TipKljuca, TipVrijednosti> &operator=(const HashMapa<TipKljuca,
            TipVrijednosti>& mapa) {
        if(this == &mapa) return *this;
        oslobodiMemoriju();
        kapacitet = mapa.kapacitet;
        velicina = mapa.velicina;this->funkcija = mapa.funkcija;
        niz = new std::pair<TipKljuca, TipVrijednosti>*[kapacitet];
        for(int i=0; i<kapacitet; i++) niz[i] = nullptr;
        for(int i=0; i<kapacitet; i++) if(mapa.niz[i] != nullptr) niz[i] = new std::pair<
                        TipKljuca, TipVrijednosti>(*mapa.niz[i]);
        return *this;
    }
    ~HashMapa() override {
        oslobodiMemoriju();
    }
    int brojElemenata() const override{
        return velicina;
    }
    void provjera() {
        if(velicina != kapacitet) return;
        auto tmp = new std::pair<TipKljuca, TipVrijednosti>*[kapacitet];
        for(int i=0; i<kapacitet; i++) tmp[i] = nullptr;
        for(int i=0; i<kapacitet; i++) {
            if(niz[i] != nullptr) tmp[i] = new std::pair<TipKljuca, TipVrijednosti>(*niz[i]);
        }
        for(int i=0; i<kapacitet; i++) delete niz[i];
        delete[] niz;
        int oldKapacitet = kapacitet;
        niz = new std::pair<TipKljuca, TipVrijednosti>*[kapacitet = kapacitet + 1000];
        for(int i =0; i<kapacitet; i++) niz[i] = nullptr;
        for(int i=0; i<oldKapacitet; i++) if(tmp[i] != nullptr) (*this)[tmp[i]->first] =
                                                                        tmp[i]->second;
        for(int i=0; i<oldKapacitet; i++) delete tmp[i];
        delete[] tmp;
    }
    TipVrijednosti& operator[](TipKljuca kljuc) override {
        if(funkcija==nullptr) throw std::range_error("Hash funkcija nije definisana");
        provjera();
        for(int i=0; i<kapacitet; i++) {
            int pozicija = (funkcija(kljuc, kapacitet)+i)%kapacitet;
            if(niz[pozicija]==nullptr) {
                niz[pozicija] = new std::pair<TipKljuca, TipVrijednosti>(kljuc,
                                                                         TipVrijednosti());
                velicina++;
                return niz[pozicija]->second;
            }
            else if(niz[pozicija]->first==kljuc) return niz[pozicija]->second;
        }
    }void obrisi() override{
        for(int i=0; i<kapacitet; i++) {
            delete niz[i];
            niz[i] = nullptr;
        }
        velicina = 0;
    }
    void oslobodiMemoriju() {
        for(int i=0; i<kapacitet; i++) {
            delete niz[i];
            niz[i] = nullptr;
        }
        delete[] niz;
        niz = nullptr;
        velicina = 0;
    }
    void obrisi(const TipKljuca& kljuc) override { // popravi
        for(int i=0; i<kapacitet; i++) {
            int pozicija = (funkcija(kljuc,kapacitet)+i)%kapacitet;
            if(niz[pozicija]==nullptr) break;
            else if(niz[pozicija]->first==kljuc) {
                niz[pozicija]->first = TipKljuca();
                velicina--;
                return;
            }
        }
        throw std::domain_error("Takav element ne postoji");
    }
    TipVrijednosti operator[](TipKljuca kljuc) const override {
        for(int i=0; i<kapacitet; i++) {
            int pozicija = (funkcija(kljuc, kapacitet)+i)%kapacitet;
            if(niz[pozicija]==nullptr) return TipVrijednosti();
            if(niz[pozicija]->first==kljuc) return niz[pozicija]->second;
        }
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
    BinStabloMapa<double, int> BinMapa;
    NizMapa<double,int> NizMapa;
    HashMapa<int, int> HashMapa;
    HashMapa.definisiHashFunkciju([](int ulaz, unsigned int max){
        unsigned int suma=23;
        suma = suma*31 + ulaz;
        return suma % max;
    });
    double tmp = 0;
    for(int i=0; i<1000; i++) {
        double temp = std::rand();

        if(i == 5000) tmp = temp;
        BinMapa[temp] = i;
        NizMapa[temp] = i;
        HashMapa[temp] = i;
    }
    clock_t vrijeme1 = clock();
    NizMapa[-12] = 1232;
    clock_t vrijeme2 = clock();
    std::cout <<
              "Vrijeme izvrsenja dodavanja novog elementa u nizMapa: " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) <<
              " ms." << std::endl;
    vrijeme1 = clock();
    BinMapa[-12] = 1232;
    vrijeme2 = clock();
    std::cout <<"Vrijeme izvrsenja dodavanja novog elementa u binStabloMapa: " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;
    vrijeme1 = clock();
    HashMapa[-12] = 1232;
    vrijeme2 = clock();
    std::cout <<"Vrijeme izvrsenja dodavanjanovog elementa u hashMapa: " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;

    vrijeme1 = clock();
    NizMapa[tmp];
    vrijeme2 = clock();
    std::cout <<"Vrijeme izvrsenja pristupa elementuu nizMapa: " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;
    vrijeme1 = clock();
    BinMapa[tmp];
    vrijeme2 = clock();
    std::cout <<"Vrijeme izvrsenja pristupa elementu u binStabloMapa: " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000)<<
              " ms." << std::endl;
    vrijeme1 = clock();
    HashMapa[tmp];
    vrijeme2 = clock();
    std::cout <<"Vrijeme izvrsenja pristupa elementuu hashMapa: " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) << " ms." << std::
    endl;

    vrijeme1 = clock();
    BinMapa.obrisi(tmp);
    vrijeme2 = clock();
    std::cout <<"Vrijeme izvrsenja brisanja elementa u nizMapa: " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) << " ms." << std::
    endl;
    vrijeme1 = clock();
    NizMapa.obrisi(tmp);
    vrijeme2 = clock();
    std::cout <<"Vrijeme izvrsenja brisanja elementa u binStabloMapa: " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000)<<
              " ms." << std::endl;
    vrijeme1 = clock();
    HashMapa.obrisi(tmp);
    vrijeme2 = clock();
    std::cout <<"Vrijeme izvrsenja brisanja elementau hashMapa: " << (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000) << " ms." << std::endl;
    return 0;
}
