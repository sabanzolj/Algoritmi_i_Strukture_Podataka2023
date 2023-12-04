#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

template <typename NekiTip>
class Stek{

    struct Cvor{
        NekiTip element;
        Cvor *next;
    };

    int size;
    Cvor *vrhS;

    public:

    Stek(){
        vrhS=nullptr;
        size=0;
    }

    int brojElemenata()const{
        return size;
    }

    void brisi(){
        while (brojElemenata()!=0) {
        skini();
        }
    }

    NekiTip skini(){
        if(brojElemenata()==0){
            throw std::domain_error("Stek je prazan");
        }

        NekiTip tempVal=vrhS->element;
        auto zaBrisanje=vrhS;
        vrhS=vrhS->next;
        delete zaBrisanje;

        size--;
        return tempVal;
    }

    void stavi(const NekiTip &s){
        auto noviC=new Cvor{s,nullptr};

        noviC->next=vrhS;
        vrhS=noviC;
        size++;
    }

    NekiTip &vrh(){
        if(brojElemenata()==0){
            throw std::domain_error("Stek je prazan");
        }
        return vrhS->element;

    }

    bool jeLiPrazan()const{
        if(size==0)return true;
        else return false;
    }

    Stek(const Stek &S){
        size=S.size;
        auto tempPok=S.vrhS;
        Cvor *tempC=nullptr;

        while(tempPok){
            auto noviC=new Cvor{tempPok->element,nullptr};

            if(tempPok==S.vrhS){
                vrhS=noviC;
            }
            else{
                tempC->next=noviC;
            }

            tempC=noviC;
            tempPok=tempPok->next;
        }
    }

    Stek &operator=(const Stek &S){
        if(&S==this){
            return *this;
        }

        brisi();

        size=S.size;
        auto tempPok=S.vrhS;
        Cvor *tempC=nullptr;

        while(tempPok){
            auto noviC=new Cvor{tempPok->element,nullptr};
            if(tempPok==S.vrhS){
               vrhS=noviC;
            }
            else{
                tempC->next=noviC;
            }
            tempC=noviC;
            tempPok=tempPok->next;

        }
    }

    ~Stek(){
        brisi();
    }
};

void pretraga(Stek<std::vector<int>> &S,int t){

    if(S.brojElemenata()==0){
        std::cout<<"Nema elementa"<<std::endl;
        return;
    }

    std::vector<int> vektor=S.skini();

    int l=0;
    int h=vektor.size()-1;

    while(h>=l){
        int prosjek=(l+h)/2;

        if(vektor[prosjek]==t){
            std::cout<<prosjek<<" "<<S.brojElemenata();
            S.stavi(vektor);
            return;
        }
        else if(vektor[prosjek]<t){
            l=prosjek+1;

        }
        else{
            h=prosjek-1;

        }
    }

    pretraga(S, t);
    S.stavi(vektor);
}

void testJedanVektor() {
    Stek<std::vector<int>> s;
    std::vector<int> a = {1, 2, 3, 4, 5};
    s.stavi(a);
    pretraga(s, 3); // "2 0"
}

void testPrazanStek() {
    Stek<std::vector<int>> s;
    pretraga(s, 999999); // "Nema elementa"
}



int main(){
    testJedanVektor();
    std::cout<<std::endl;
    testPrazanStek();

    return 0;
}