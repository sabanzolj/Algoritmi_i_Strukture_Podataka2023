#include <iostream>
#include<stdexcept>
#include<cmath>
using namespace std;
template <typename TipOznake>
class GranaIterator;
template <typename TipOznake>
class Cvor;
template <typename TipOznake>
class Grana;
template<typename TipOznake>
class UsmjereniGraf{
public:
    UsmjereniGraf(){}
    virtual ~UsmjereniGraf(){}
    virtual int dajBrojCvorova()const=0;
    virtual void postaviBrojCvorova(int noviBrojCvorova)=0;
    virtual void dodajGranu(int polazniCvor, int dolazniCvor, double tezinaGrane=0)=0;
    virtual void obrisiGranu(int polazniCvor, int dolazniCvor)=0;
    virtual void postaviTezinuGrane(int polazniCvor,int dolazniCvor, double
    novaTezinaGrane=0)=0;
    virtual double dajTezinuGrane(int polazniCvor, int dolazniCvor)=0;
    virtual bool postojiGrana(int polazniCvor, int dolazniCvor)=0;
    virtual void postaviOznakuCvora(int brojCvora, TipOznake oznaka)=0;
    virtual TipOznake dajOznakuCvora(int cvor)=0;
    virtual void postaviOznakuGrane(int polazniCvor, int dolazniCvor,
                                    TipOznake oznaka)=0;
    virtual TipOznake dajOznakuGrane(int polazniCvor,int dolazniCvor)=0;
    virtual Grana<TipOznake> dajGranu(int,int)=0;
    virtual Cvor<TipOznake> dajCvor(int)=0;
    virtual GranaIterator<TipOznake> dajGranePocetak()=0;
    virtual GranaIterator<TipOznake> dajGraneKraj()=0;
    virtual Cvor<TipOznake>* dajCvorove()=0;
    virtual Grana<TipOznake>**dajGrane()=0;
};template<typename TipOznake>
class Cvor{
    UsmjereniGraf<TipOznake>*graf = nullptr;
    TipOznake oznaka = TipOznake{};
    int redniBrojCvora = 0;
public:
    Cvor(){}
    Cvor(UsmjereniGraf<TipOznake> *usmjerenGraf, int redniBrojCvora){
        graf = usmjerenGraf;
        this->redniBrojCvora = redniBrojCvora;
    }
    Cvor &operator=(const Cvor &novi){
        if(this!=&novi){
            oznaka = novi.oznaka;
            redniBrojCvora =novi.redniBrojCvora;
        }
        return *this;
    }
    void povezi(UsmjereniGraf<TipOznake> *graf){ this->graf=graf;}
    void postaviRedniBroj(int r){redniBrojCvora=r;}
    int dajRedniBroj()const{return redniBrojCvora;}
    TipOznake dajOznaku()const{ return oznaka;}
    void postaviOznaku(TipOznake oznakaCvora){
        this->oznaka=oznakaCvora;
        this->stavi(&graf->dajCvorove()[redniBrojCvora],oznaka);
    }
    void stavi(Cvor *r,TipOznake oznaka){
        r->oznaka = oznaka;
    }
};template<typename TipOznake>
class Grana{
    UsmjereniGraf<TipOznake>*graf = nullptr;
    TipOznake oznaka = TipOznake{};
    double tezina = -1;
    int polazniCvor = -1;
    int dolazniCvor = -1;
public:
    bool operator ==(const Grana&x){return graf==x.graf && oznaka==x.oznaka && tezina==x.tezina &&
                                           polazniCvor==x.polazniCvor && dolazniCvor==x.dolazniCvor;
    }
    bool operator !=(const Grana&x){
        return !((*this)==x);
    }
    Grana(){}
    Grana(int polazniCvor, int dolazniCvor, double tezina){
        this->polazniCvor = polazniCvor;
        this->dolazniCvor = dolazniCvor;
        this->tezina = tezina;
    }
    Grana &operator =(const Grana&nova){
        if(this!=&nova){
            oznaka = nova.oznaka;
            tezina = nova.tezina;
            polazniCvor = nova.polazniCvor;
            dolazniCvor = nova.dolazniCvor;
        }
        return *this;
    }
    void povezi(UsmjereniGraf<TipOznake>*graf){this->graf=graf;}
    double dajTezinu()const{return tezina;}
    void postaviTezinu(double tezina){this->tezina=tezina;}
    TipOznake& dajOznaku(){ return oznaka;}
    void postaviOznaku(TipOznake oznaka){
        this->oznaka=oznaka;
        (graf->dajGrane()[polazniCvor])[dolazniCvor].dajOznaku()=oznaka;
    }
    void postaviOznakuGrani(Grana*g, TipOznake oznaka){
        g->oznaka = oznaka;
    }
    void postavi(int polazniCvor, int dolazniCvor){
        this->polazniCvor=polazniCvor;
        this->dolazniCvor=dolazniCvor;
    }
    Cvor<TipOznake> dajPolazniCvor(){
        return graf->dajCvorove()[polazniCvor];
    }
    Cvor<TipOznake> dajDolazniCvor(){
        return graf->dajCvorove()[dolazniCvor];
    }
};template <typename TipOznake>
class GranaIterator {
    Grana<TipOznake>**grane= nullptr;
    Cvor<TipOznake>*cvorovi=nullptr;
    int polazniCvor=-1, dolazniCvor=-1,velicina=-1;
public:
    void f(){
        cout<<"polazniCvor: "<<polazniCvor<<" dolazniCvor: "<<dolazniCvor<<"\n";
    }
    GranaIterator(Grana<TipOznake>**grane, Cvor<TipOznake>*cvorovi,int
    polazniCvor, int dolazniCvor, int velicina){
        this->grane = grane;
        this->cvorovi=cvorovi;
        this->velicina = velicina;
        this->polazniCvor = polazniCvor;
        this->dolazniCvor = dolazniCvor;
    }
    void set(int polazni, int dolazni){
        polazniCvor = polazni;
        dolazniCvor = dolazni;
    }
    Grana<TipOznake> operator*(){
        return grane[polazniCvor][dolazniCvor];
    }
    bool operator==(const GranaIterator &iter) const{
        return polazniCvor==iter.polazniCvor && dolazniCvor==iter.
                dolazniCvor && velicina==iter.velicina;
    }
    bool operator!=(const GranaIterator &iter) const{
        return !(*this==iter);
    }
    GranaIterator& operator++(){
        if((dolazniCvor+1)==velicina)
            dolazniCvor=0;
        else
            dolazniCvor++;
        for(int i=0; i<velicina;i++){
            for(int j=0; j<velicina; j++){
                if(i>polazniCvor && fabs(grane[i][j].dajTezinu()- (-1)) > 1e-5 || (
                        i == polazniCvor && j>=dolazniCvor && fabs(grane[i][j].dajTezinu()- (-1)) > 1e-5 )){
                    polazniCvor = i;
                    dolazniCvor = j;
                    return *this;
                }
            }
        }
    }

    GranaIterator operator++(int){
        GranaIterator iterator2(grane,cvorovi,velicina);
        iterator2.set(polazniCvor,dolazniCvor);
        (*this).operator++;
        return iterator2;
    }
};
template<typename TipOznake>
class MatricaGraf : public UsmjereniGraf<TipOznake>{
    int maxBrojCvorova;
    int brojCvorova, brojGrana;Grana<TipOznake> **mat;
    Cvor<TipOznake> *cvorovi = nullptr;
public:
    MatricaGraf(int maxV);
    ~MatricaGraf(){
        delete []cvorovi;
        for(int i=0; i<maxBrojCvorova;i++)
            delete[]mat[i];
        delete []mat;
    }
    Cvor<TipOznake>* dajCvorove(){
        return cvorovi;
    }
    Grana<TipOznake>** dajGrane(){
        return mat;
    }
    int dajBrojCvorova()const{ return brojCvorova;}
    void postaviBrojCvorova(int noviBrojCvorova){
        if(noviBrojCvorova<brojCvorova) throw domain_error(
                    "Novi broj cvorova je manji od trenutnog");
        Cvor<TipOznake> *novi = new Cvor<TipOznake>[noviBrojCvorova]{};
        for(int i=0;i<brojCvorova;i++)
            novi[i] = cvorovi[i];
        for(int i=brojCvorova;i<noviBrojCvorova;i++)
            novi[i].postaviRedniBroj(i);
        brojCvorova = noviBrojCvorova;
        delete []cvorovi;
        cvorovi = novi;
    }
    void dodajGranu(int polazniCvor,int dolazniCvor, double tezinaGrane=0);
    void obrisiGranu(int polazniCvor, int dolazniCvor);
    void postaviTezinuGrane(int polazniCvor,int dolazniCvor, double
    novaTezinaGrane=0);
    double dajTezinuGrane(int polazniCvor,int dolazniCvor);
    bool postojiGrana(int polazniCvor, int dolazniCvor);
    void postaviOznakuCvora(int brojCvora, TipOznake oznaka);
    TipOznake dajOznakuCvora(int cvor);
    void postaviOznakuGrane(int polazniCvor,int dolazniCvor, TipOznake oznaka);
    TipOznake dajOznakuGrane(int polazniCvor,int dolazniCvor);
    Grana<TipOznake> dajGranu(int , int);
    Cvor<TipOznake> dajCvor(int);
    GranaIterator<TipOznake> dajGranePocetak();
    GranaIterator<TipOznake> dajGraneKraj();
};template<typename TipOznake>
MatricaGraf<TipOznake>::MatricaGraf(int maxV){
    maxBrojCvorova = maxV;
    brojCvorova = maxV;
    brojGrana = 0;
    cvorovi = new Cvor<TipOznake>[maxBrojCvorova]{};
    for(int i=0; i<maxBrojCvorova;i++){
        cvorovi[i].povezi(this);
        cvorovi[i].postaviRedniBroj(i);
    }
    mat = new Grana<TipOznake> *[maxBrojCvorova]{};
    for(int i=0; i<maxBrojCvorova;i++){
        mat[i] = new Grana<TipOznake>[maxBrojCvorova]{};
        for(int j=0; j<maxBrojCvorova; j++)
            mat[i][j].povezi(this);
    }
}template<typename TipOznake>
void MatricaGraf<TipOznake>::dodajGranu(int polazniCvor, int dolazniCvor, double tezinaGrane){

    if(mat[polazniCvor][dolazniCvor].dajTezinu() == 0)
        brojCvorova++;
    mat[polazniCvor][dolazniCvor].postavi(polazniCvor,dolazniCvor);
    mat[polazniCvor][dolazniCvor].postaviTezinu(tezinaGrane);
}

template<typename TipOznake>
void MatricaGraf<TipOznake>::obrisiGranu(int polazniCvor, int dolazniCvor){
    if(mat[polazniCvor][dolazniCvor].dajTezinu() != 0)
        brojCvorova--;
    mat[polazniCvor][dolazniCvor].postavi(-1,-1);
    mat[polazniCvor][dolazniCvor].postaviTezinu(-1);
}
template<typename TipOznake>
void MatricaGraf<TipOznake>::postaviTezinuGrane(int polazniCvor, int dolazniCvor, double novaTezinaGrane){

    mat[polazniCvor][dolazniCvor].postaviTezinu(novaTezinaGrane);
}

template<typename TipOznake>
double MatricaGraf<TipOznake>::dajTezinuGrane(int polazniCvor, int dolazniCvor){
    return mat[polazniCvor][dolazniCvor].dajTezinu();
}
template<typename TipOznake>
bool MatricaGraf<TipOznake>::postojiGrana(int polazniCvor, int dolazniCvor){
    return mat[polazniCvor][dolazniCvor].dajTezinu() != -1;
}
template<typename TipOznake>
void MatricaGraf<TipOznake>::postaviOznakuCvora(int cvor, TipOznake oznaka){
    this->cvorovi[cvor].postaviOznaku(oznaka);
}
template<typename TipOznake>
TipOznake MatricaGraf<TipOznake>::dajOznakuCvora(int cvor){
    return cvorovi[cvor].dajOznaku();
}
template<typename TipOznake>
void MatricaGraf<TipOznake>::postaviOznakuGrane(int polazniCvor, int
dolazniCvor, TipOznake oznaka){
    mat[polazniCvor][dolazniCvor].postaviOznaku(oznaka);
}
template<typename TipOznake>
TipOznake MatricaGraf<TipOznake>::dajOznakuGrane(int polazniCvor, int dolazniCvor){
    return mat[polazniCvor][dolazniCvor].dajOznaku();
}
template<typename TipOznake>
Grana<TipOznake> MatricaGraf<TipOznake>::dajGranu(int polazniCvor, int dolazniCvor){
    return mat[polazniCvor][dolazniCvor];
}
template<typename TipOznake>
Cvor<TipOznake> MatricaGraf<TipOznake>::dajCvor(int cvor){return cvorovi[cvor];
}
template<typename TipOznake>
GranaIterator<TipOznake> MatricaGraf<TipOznake>::dajGranePocetak(){
    for(int i=0;i<maxBrojCvorova;i++)
        for(int j=0; j<maxBrojCvorova; j++)
            if(mat[i][j].dajTezinu() != -1){

                return GranaIterator<TipOznake>(mat,cvorovi,i,j,maxBrojCvorova);
            }

}
template<typename TipOznake>
GranaIterator<TipOznake> MatricaGraf<TipOznake>::dajGraneKraj(){
    int x=-1,y=-1;
    for(int i=0;i<maxBrojCvorova;i++)
        for(int j=0; j<maxBrojCvorova; j++)
            if((mat[i][j].dajTezinu()- (-1))>1e-5){
                x=i;
                y=j;
            }
    if(y+1==maxBrojCvorova){
        return GranaIterator<TipOznake>(mat,cvorovi,x+1,0,maxBrojCvorova);
    }
    return GranaIterator<TipOznake>(mat,cvorovi,x,y+1,maxBrojCvorova);
}

int main(){
    return 0;
}