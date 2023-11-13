#include <iostream>
#include <stdexcept>
#include <cassert>


using namespace std;

template <typename NekiTip>
struct Cvor{
    NekiTip element;
    Cvor *veza;
};

template <typename NekiTip>
class Red{
    Cvor<NekiTip> *start;
    Cvor<NekiTip> *end;
    int broj_elemenata;

    void copy(const Red &r){
        auto temp = r.start;
        while (temp) {
            stavi(temp->element);
            temp = temp->veza;
        }

    }

public:

    Red():start(nullptr),end(nullptr),broj_elemenata(0){}

    Red(const Red &r):start(nullptr),end(nullptr),broj_elemenata(0){
        copy(r);
    }

    void brisi(){
        while (start!=nullptr) {
            Cvor<NekiTip> *temp=start;
            start=start->veza;
            delete temp;
            temp=nullptr;

        }
        broj_elemenata=0;
        start=nullptr;
        end=nullptr;

    }

    void stavi(const NekiTip &e){
        Cvor<NekiTip> *noviC=new Cvor<NekiTip>();
        noviC->element=e;
        noviC->veza=nullptr;
        if(end==nullptr){
            start=end=noviC;
        }
        else{
            end->veza=noviC;
            end=noviC;
        }
        broj_elemenata++;
    }

    NekiTip skini(){
        if(start==nullptr){
            throw std::range_error("Red je prazan");
        }

        NekiTip r=start->element;
        auto brisanje=start;

        start=start->veza;
        delete brisanje;
        broj_elemenata--;
        if(broj_elemenata==0){
            end=nullptr;
        }

        return r;
    }


    NekiTip celo()const{
        if(start==nullptr){
            throw std::range_error("Red je prazan");
        }

        return start->element;
    }

    NekiTip &celo(){
        if(start==nullptr){
            throw std::range_error("Red je prazan");
        }

        return start->element;
    }

    int brojElemenata()const{
        return broj_elemenata;
    }

    Red &operator=(const Red &r){
        if(this==&r){
            return *this;
        }

        brisi();
        copy(r);
        return *this;
    }

    ~Red(){
        brisi();
    }
};

void testStavi() {
    Red<int> r;
    r.stavi(1);
    assert(r.brojElemenata() == 1);
    assert(r.celo() == 1);
}

void testSkini() {
    Red<int> r;
    r.stavi(1);
    r.stavi(2);

    int skini1 = r.skini();
    assert(skini1 == 1);
    assert(r.brojElemenata() == 1);

    int skini2 = r.skini();
    assert(skini2 == 2);
    assert(r.brojElemenata() == 0);
}



int main(){
    testSkini();
    testStavi();


    return 0;
}