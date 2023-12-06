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

void test1() {
    //STRING-STRING
    NizMapa<std::string, std::string> mapa;
    mapa["Sarajevo"] = "BiH";
    mapa["Zagreb"] = "Hrvatska";

    if(mapa.brojElemenata()!=2){
        std::cout<<"TEST1: Nije OK"<<std::endl;
        return;
    }

    if(mapa["Sarajevo"]!="BiH"){
        std::cout<<"TEST1: Nije OK"<<std::endl;
        return;
    }


    try{
        mapa.obrisi("Svrakino Selo");//Brisanje nepostojeceg elementa
        std::cout<<"TEST1: Nije OK";
        return;
    }
    catch(...){}

    mapa["Berlin"]="Njemacka";
    if(mapa["Berlin"]!="Njemacka"){
        std::cout<<"TEST1: Nije OK"<<std::endl;
        return;
    }

    mapa.obrisi("Berlin");

    NizMapa<std::string, std::string> novaMapa;
    try{
        novaMapa=mapa;
    }
    catch(...){
        std::cout<<"TEST1: Nije OK"<<std::endl;
    }

    novaMapa["Podgorica"]="Crna Gora";

    if(novaMapa.brojElemenata()!=3){
        std::cout<<"TEST1: Nije OK"<<std::endl;
        return;
    }

    mapa.obrisi();

    if(mapa.brojElemenata()!=0){
        std::cout<<"TEST1: Nije OK"<<std::endl;
        return;
    }

}

void test2() {
    //INT-DOUBLE
    NizMapa<int, double> mapa;
    mapa[1] = 1.5;
    mapa[2] = 2.5;

    if(mapa.brojElemenata()!=2){
        std::cout<<"TEST2: Nije OK"<<std::endl;
        return;
    }

    if(mapa[1]!=1.5){
        std::cout<<"TEST2: Nije OK"<<std::endl;
        return;
    }


    try{
        mapa.obrisi(10);//Brisanje nepostojeceg elementa
        std::cout<<"TEST2: Nije OK";
        return;
    }
    catch(...){}

    mapa[3]=3.5;
    if(mapa[3]!=3.5){
        std::cout<<"TEST2: Nije OK"<<std::endl;
        return;
    }
    mapa.obrisi(3);

    NizMapa<int, double> novaMapa;
    try{
        novaMapa=mapa;
    }
    catch(...){
        std::cout<<"TEST2: Nije OK"<<std::endl;
    }

    novaMapa[4]=4.5;

    if(novaMapa.brojElemenata()!=3){
        std::cout<<"TEST2: Nije OK"<<std::endl;
        return;
    }

    mapa.obrisi();

    if(mapa.brojElemenata()!=0){
        std::cout<<"TEST2: Nije OK"<<std::endl;
        return;
    }


}

void test3() {
    //CHAR-INT
    NizMapa<char, int> mapa;
    mapa['a'] = 1;
    mapa['b'] = 2;

    if(mapa.brojElemenata()!=2){
        std::cout<<"TEST3: Nije OK"<<std::endl;
        return;
    }

    if(mapa['a']!=1){
        std::cout<<"TEST3: Nije OK"<<std::endl;
        return;
    }


    try{
        mapa.obrisi('k');//Brisanje nepostojeceg elementa
        std::cout<<"TEST3: Nije OK";
        return;
    }
    catch(...){}

    mapa['c']=3;
    if(mapa['c']!=3){
        std::cout<<"TEST3: Nije OK"<<std::endl;
        return;
    }
    mapa.obrisi('c');

    NizMapa<char, int> novaMapa;
    try{
        novaMapa=mapa;
    }
    catch(...){
        std::cout<<"TEST3: Nije OK"<<std::endl;
    }

    novaMapa['d']=5;

    if(novaMapa.brojElemenata()!=3){
        std::cout<<"TEST3: Nije OK"<<std::endl;
        return;
    }

    mapa.obrisi();

    if(mapa.brojElemenata()!=0){
        std::cout<<"TEST3: Nije OK"<<std::endl;
        return;
    }
}

void test4() {
    //STRING-FLOAT
    NizMapa<std::string, float> mapa;
    mapa["A"] = 1.5;
    mapa["B"] = 2.5;

    if(mapa.brojElemenata()!=2){
        std::cout<<"TEST4: Nije OK"<<std::endl;
        return;
    }

    if(mapa["A"]!=1.5){
        std::cout<<"TEST4: Nije OK"<<std::endl;
        return;
    }


    try{
        mapa.obrisi("t");//Brisanje nepostojeceg elementa
        std::cout<<"TEST4: Nije OK";
        return;
    }
    catch(...){}

    mapa["C"]=3.5;
    if(mapa["C"]!=3.5){
        std::cout<<"TEST4: Nije OK"<<std::endl;
        return;
    }
    mapa.obrisi("C");

    NizMapa<std::string, float> novaMapa;
    try{
        novaMapa=mapa;
    }
    catch(...){
        std::cout<<"TEST4: Nije OK"<<std::endl;
    }

    novaMapa["D"]=5;

    if(novaMapa.brojElemenata()!=3){
        std::cout<<"TEST4: Nije OK"<<std::endl;
        return;
    }

    mapa.obrisi();

    if(mapa.brojElemenata()!=0){
        std::cout<<"TEST4: Nije OK"<<std::endl;
        return;
    }
}

void test5() {
    //BOOL-STRING
    NizMapa<bool, std::string> mapa;
    mapa[true] = "OK";
    mapa[false] = "NOT OK";

    if(mapa.brojElemenata()!=2){
        std::cout<<"TEST5: Nije OK"<<std::endl;
        return;
    }

    if(mapa[true]!="OK"){
        std::cout<<"TEST5: Nije OK"<<std::endl;
        return;
    }


    /*try{
    mapa.obrisi();//Brisanje nepostojeceg elementa
    std::cout<<"TEST4: Nije OK";
    return;
    }
    catch(...){}*/

    mapa[true]="PONOVO OK";
    if(mapa[true]!="PONOVO OK"){
        std::cout<<"TEST5: Nije OK"<<std::endl;
        return;
    }
    mapa.obrisi(true);

    NizMapa<bool, std::string> novaMapa;
    try{
        novaMapa=mapa;
    }
    catch(...){
        std::cout<<"TEST5: Nije OK"<<std::endl;
    }

    novaMapa[true]="vol3 OK";

    if(novaMapa.brojElemenata()!=2){
        std::cout<<"TEST5: Nije OK"<<std::endl;
        return;
    }

    mapa.obrisi();

    if(mapa.brojElemenata()!=0){
        std::cout<<"TEST5: Nije OK"<<std::endl;
        return;
    }
}



int main() {
    test1();
    test2();
    test3();
    test4();
    test5();

    return 0;
}