#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include <ctime>

using namespace std;

template<typename NekiTip>
void Transfer(NekiTip &a,NekiTip &b){
    NekiTip temp=a;
    a=b;
    b=temp;
}

template<typename NekiTip>
void selection_sort(NekiTip *niz,int vel){
    for(int i=0;i<vel-1;i++){
        NekiTip minEl=niz[i];
        int minInd=i;
        for(int j=i+1;j<vel;j++){
            if(minEl>niz[j]){
                minEl=niz[j];
                minInd=j;
            }
        }
        if(minInd!=i){
            Transfer(niz[i], niz[minInd]);
        }
    }
}

template<typename NekiTip>
void bubble_sort(NekiTip *niz,int vel){
    for(int i=vel-1;i>0;i--){
        for(int j=1;j<i+1;j++){
            if(niz[j]<niz[j-1]){
                /*NekiTip temp=niz[j-1];
                niz[j-1]=niz[j];
                niz[j]=temp;*/
                Transfer(niz[j-1], niz[j]);

            }
        }
    }

}



template<typename NekiTip>
int particija(NekiTip *niz,int u,int l){
    NekiTip pivot=niz[u];
    int p=u+1;
    while(p<=l && niz[p]<pivot)p++;
    for(int i=p+1;i<=l;i++){
        if(niz[i]<pivot){
            NekiTip temp=niz[i];
            niz[i]=niz[p];
            niz[p]=temp;
            p++;
        }
    }

    NekiTip temp=niz[u];
    niz[u]=niz[p-1];
    niz[p-1]=temp;

    return p-1;

}

template<typename NekiTip>
void quicksort_POMOCNA(NekiTip *niz,int u,int l){
    if(l<=u)return;
    int j=particija(niz, u, l);
    quicksort_POMOCNA(niz, u, j-1);
    quicksort_POMOCNA(niz, j+1, l);


}

template<typename NekiTip>
void quick_sort(NekiTip *niz,int vel){
    quicksort_POMOCNA(niz, 0, vel-1);
}

template<typename NekiTip>
void Merge(NekiTip *niz,int l,int p,int q,int u){
    int i=0;
    int j=q-l;
    int k=l;
    int vel=u-l+1;
    NekiTip *B=new NekiTip[vel];

    for(int a=0;a<vel;a++){
        B[a]=niz[l+a];
    }

    while(i<=p-l || j<=u-l){
        if(!(j<=u-l) && i<=p-l){
            niz[k]=B[i++];
        }
        else if(!(i<=p-l) && j<=u-l){
            niz[k]=B[j++];
        }
        else if(B[i]<B[j]){
            niz[k]=B[i++];
        }
        else{
            niz[k]=B[j++];
        }
        k++;
    }
    delete[] B;
}

template<typename NekiTip>
void mergesort_POMOCNA(NekiTip *niz,int l,int u){
    if(u<=l)return;

    int p=(u+l-1)/2;
    int q=p+1;
    mergesort_POMOCNA(niz, l, p);
    mergesort_POMOCNA(niz,q,u);
    Merge(niz,l,p,q,u);

}

template<typename NekiTip>
void merge_sort(NekiTip *niz,int vel){
    mergesort_POMOCNA(niz, 0, vel-1);
}

void ucitaj(std::string filename,int *&niz,int &vel){
    std::ifstream ulaz2(filename);
    if(!ulaz2){
        throw std::domain_error("Doslo je do greske pri otvaranju datoteke");
    }
    vel=0;
    int temp;
    while(ulaz2>>temp)vel++;
    niz=new int[vel];
    ulaz2.close();
    std::ifstream ulaz(filename);
    for(int i=0;i<vel;i++){
        ulaz>>niz[i];
        if(!ulaz)break;
    }

    ulaz.close();
}

void generisi(std::string filename,int vel){
    std::ofstream izlaz(filename);
    if(!izlaz){
        throw std::logic_error("Doslo je do greske pri stvaranju datoteke");

    }
    for(int i=0;i<vel;i++){
        izlaz<<std::rand()<<" ";
    }

}

void interfejs(){
    generisi("fajl", 10000);

    int vel;
    int *niz;

    ucitaj("fajl", niz, vel);
    std::cout<<"Uneseni niz glasi: "<<std::endl;
    for(int i=0;i<vel;i++){
        std::cout<<niz[i]<<" ";
    }

    std::cout<<"Izaberite algoritam za sortiranje: "<<std::endl;
    std::cout<<"1. bubble sort"<<std::endl<<"2. selection sort"<<std::endl<<"3. quick sort"<<std::endl<<"4. merge sort"<<std::endl;

    int izbor;

    std::cin>>izbor;

    if(!cin){
        std::cin.clear();
        std::cin.ignore(1000,'\n');
        return;
    }

    clock_t cin;
    switch(izbor){
        case 1:{
            cin=clock();
            bubble_sort(niz, vel);
            break;
        }
        case 2:{
            cin=clock();
            selection_sort(niz, vel);
            break;
        }
        case 3:{
            cin=clock();
            quick_sort(niz, vel);
            break;
        }
        case 4:{
            cin=clock();
            merge_sort(niz, vel);
            break;
        }
        default:return;
    }

    clock_t cout=clock();

    std::cout<<"Algoritam sortiranja je trajao "<<(cout-cin)/(CLOCKS_PER_SEC/1000)<<" milisekundi"<<std::endl;
    bool sortirano=true;

    for(int i=0;i<vel-1;i++){
        if(niz[i+1]<niz[i])sortirano=false;
    }

    if(sortirano==true)std::cout<<"Niz je uspjesno sortiran!"<<std::endl;

    std::cout<<"U koju datoteku zelite spremiti sortirani niz(unesite ime): ";

    std::cin.clear();
    std::cin.ignore(1000,'\n');

    std::string datoteka;
    std::getline(std::cin,datoteka);
    std::ofstream izlaz(datoteka);
    for(int i=0;i<vel;i++){
        izlaz<<niz[i]<<std::endl;
    }
    izlaz.close();
    delete[] niz;


}


int main(){
    interfejs();

    return 0;
}