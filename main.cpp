#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

int brCifara(int x){
    int rezultat=0;

    while(x>0){
        x=x/10;
        rezultat++;
    }
    return rezultat;
}

void radixSort(std::vector<int> &v){
    int size=v.size();

    if(size==0)return;

    int brojCifara=brCifara(v[0]);

    for(int i=1;i<size;i++){
        int temp=brCifara(v[i]);
        if(brojCifara<temp)brojCifara=temp;
    }

    std::vector<std::queue<int>> redovi(10);
    int mult=10;
    int pMult=0;

    for(int i=1;i<=brojCifara;i++){
        for(int j=0;j<size;j++){
            int clan=v[j];
            int temp=clan%mult;

            if(i>1){
                temp=temp/pMult;
            }

            redovi[temp].push(clan);
        }

        int d=0;

        for(int j=0;j<10;j++){
            while(!redovi[j].empty()){
                v[d++]=redovi[j].front();
                redovi[j].pop();
            }
        }

        pMult=mult;
        mult*=10;
    }
}

int desnoDijete(const std::vector<int> &v,int i){
    return i*2+2;
}

int lijevoDijete(const std::vector<int> &v,int i){
    return i*2+1;
}

int roditelj(const std::vector<int> &v,int i){
    return floor((i-1)/2);
}

bool jeLiList(const std::vector<int> &v,int i,int size){
    return i>=size/2 && i<size;
}

void popraviDolje(std::vector<int> &v,int i,int size){
    while (jeLiList(v,i,size)==false) {
        int high=lijevoDijete(v, i);
        int rightChild=desnoDijete(v, i);

        if(rightChild<size && v[rightChild]>v[high]){
            high=rightChild;
        }

        if(v[i]<v[high]){
            std::swap(v[high],v[i]);
        }
        else return;

        i=high;
    }
}

void popraviGore(std::vector<int> &v,int i){
    while(i!=0 && v[i]>v[roditelj(v,i)]){
        std::swap(v[i],v[roditelj(v,i)]);
        i=roditelj(v,i);
    }
}

void stvoriGomilu(std::vector<int> &v){
    int size=v.size();

    for(int i=(size-1)/2;i>=0;i--){
        popraviDolje(v, i, v.size());
    }
}

void umetniUGomilu(std::vector<int> &v, int umetnuti, int &velicina){
    int size=v.size();

    v.push_back(umetnuti);
    popraviGore(v,size);
    velicina++;
}

int izbaciPrvi(std::vector<int> &v,int &velicina){
    if(velicina==0)return -1;
    velicina--;

    std::swap(v[0],v[velicina]);

    int rezultat=v[velicina];

    if(velicina)popraviDolje(v,0, velicina);

    return rezultat;
}

void gomilaSort(std::vector<int> &v){
    stvoriGomilu(v);
    int size=v.size();

    for(int i=0;i<v.size();i++){
        izbaciPrvi(v, size);
    }
}

void ispisiGomilu(const std::vector<int> &v){
    int i=1;

    while(i<=v.size()){
        for(int j=0;j<i;j++){
            if(i-1+j<v.size()){
                std::cout<<v[i-1+j]<<", ";
                std::cout<<std::endl;
                i*=2;
            }
        }
    }
}

int main(){

    return 0;
}