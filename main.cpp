#include <iostream>
#include <stdexcept>

template <typename NekiTip>
class DvostraniRed{

    struct Cvor{
        NekiTip element;
        Cvor *sljedeci;
        Cvor *prethodni;
    };

    Cvor *start;
    Cvor *end;
    int size;

    void premjesti(const DvostraniRed &r){
        auto brojac=r.start;

        while(brojac){
            staviNaVrh(brojac->element);
            brojac=brojac->sljedeci;
        }
    }

    public:

    DvostraniRed(){
        size=0;
        start=nullptr;
        end=nullptr;
    }

    void brisi(){
        while(brojElemenata()!=0){
            skiniSaCela();
        }
    }

    int brojElemenata()const{
        return size;
    }

    void staviNaCelo(const NekiTip &e){
        auto noviC=new Cvor{e,nullptr,nullptr};


        if(start==nullptr){
            start=noviC;
            end=noviC;
        }
        else{
            noviC->sljedeci=start;
            start->prethodni=noviC;
            start=noviC;

        }
        size++;

    }

    NekiTip skiniSaCela(){
        if(brojElemenata()==0){
            throw std::domain_error("Nema elemenata");
        }

        NekiTip tempVal=start->element;
        auto zaBrisanje=start;

        start=start->sljedeci;
        delete zaBrisanje;
        zaBrisanje=nullptr;

        if(start!=nullptr){
            start->prethodni=nullptr;
        }
        else{
            end=nullptr;
        }
        size--;
        return tempVal;
    }

    NekiTip skiniSaVrha(){
        if(brojElemenata()==0){
            throw std::domain_error("Nema elemenata");
        }

        NekiTip tempVal=end->element;
        auto zaBrisanje=end;

        end=end->prethodni;

        delete zaBrisanje;
        zaBrisanje=nullptr;

        if(end!=nullptr){
            end->sljedeci=nullptr;
        }
        else{
            start=nullptr;
        }
        size--;
        return tempVal;
    }

    void staviNaVrh(const NekiTip &e){
        auto noviC=new Cvor{e,nullptr,nullptr};


        if(start==nullptr){
            start=noviC;
            end=noviC;
        }

        else{
            end->sljedeci=noviC;
            noviC->prethodni=end;
            end=noviC;
        }
        size++;
    }

    NekiTip vrh()const{
        if(brojElemenata()==0){
            throw std::domain_error("Nema elemenata");
        }

        return end->element;
    }

    NekiTip &vrh(){
        if(brojElemenata()==0){
            throw std::domain_error("Nema elemenata");
        }

        return end->element;
    }

    NekiTip celo()const{
        if(brojElemenata()==0){
            throw std::domain_error("Nema elemenata");
        }
        return start->element;
    }

    NekiTip &celo(){
        if(brojElemenata()==0){
            throw std::domain_error("Nema elemenata");
        }
        return start->element;
    }

    void ispisi()const{
        NekiTip *brojac=start;

        while(brojac!=nullptr){
            std::cout<<brojac->element<<" ";
            brojac=brojac->sljedeci;
        }
        std::cout<<std::endl;
    }

    DvostraniRed &operator =(const DvostraniRed &r){
        if(&r==this){
            return *this;
        }

        brisi();
        premjesti(r);
        return *this;
    }

    DvostraniRed(const DvostraniRed &r):start(nullptr),end(nullptr),size(0){
        premjesti(r);
    }

    ~DvostraniRed(){
        if(start==nullptr){
            return;
        }
        brisi();
    }

};

void testBrojElemenata(){
    DvostraniRed<int> TestRed;
    TestRed.staviNaCelo(5);
    TestRed.staviNaCelo(4);
    TestRed.staviNaVrh(6);
    TestRed.staviNaCelo(1);
    TestRed.staviNaVrh(9);
    int temp=TestRed.brojElemenata();
    if(temp!=5){
        std::cout<<"Test nije prosao"<<std::endl;
    }

}

void testBrisanja(){
    DvostraniRed<int> TestRed;
    TestRed.staviNaCelo(5);
    TestRed.staviNaCelo(4);
    TestRed.staviNaVrh(6);
    TestRed.staviNaCelo(1);
    TestRed.staviNaVrh(9);
    TestRed.brisi();
    int temp=TestRed.brojElemenata();
    if(temp!=0){
        std::cout<<"Test nije prosao"<<std::endl;
    }

}

void testStaviNaVrh(){
    DvostraniRed<int> TestRed;
    TestRed.staviNaVrh(1);
    TestRed.staviNaVrh(2);
    TestRed.staviNaVrh(3);
    TestRed.staviNaVrh(4);
    int temp=TestRed.vrh();
    if(temp!=4){
        std::cout<<"Test nije prosao"<<std::endl;
    }

}

void testStaviNaCelo(){
    DvostraniRed<int> TestRed;
    TestRed.staviNaCelo(1);
    TestRed.staviNaCelo(2);
    TestRed.staviNaCelo(3);
    TestRed.staviNaCelo(4);
    int temp=TestRed.celo();
    if(temp!=4){
        std::cout<<"Test nije prosao"<<std::endl;
    }

}

void testSkiniSaVrha(){
    DvostraniRed<int> TestRed;
    TestRed.staviNaVrh(1);
    TestRed.staviNaVrh(2);
    TestRed.staviNaVrh(3);
    TestRed.staviNaVrh(4);
    int temp=TestRed.skiniSaVrha();
    if(temp!=4){
        std::cout<<"Test nije prosao"<<std::endl;
    }
    temp=TestRed.vrh();
    if(temp!=3){
        std::cout<<"Test nije prosao"<<std::endl;
    }

}

void testSkiniSaCela(){
    DvostraniRed<int> TestRed;
    TestRed.staviNaCelo(1);
    TestRed.staviNaCelo(2);
    TestRed.staviNaCelo(3);
    TestRed.staviNaCelo(4);
    int temp=TestRed.skiniSaCela();
    if(temp!=4){
        std::cout<<"Test nije prosao"<<std::endl;
    }
    temp=TestRed.celo();
    if(temp!=3){
        std::cout<<"Test nije prosao"<<std::endl;
    }

}

int main(){
    testBrojElemenata();
    testBrisanja();
    testStaviNaVrh();
    testStaviNaCelo();
    testSkiniSaVrha();
    testSkiniSaCela();
    return 0;
}