#include <iostream>

using namespace std;


int fib2_0(int n, int prvi_clan=1, int drugi_clan=1){

    if(n==1){
        return prvi_clan;
    }
    return fib2_0(n-1,drugi_clan,prvi_clan+drugi_clan);
}


int nzd(int x, int y){
    if(y==0){
        return x;
    }

    return nzd(y,x%y);
}



int main(){

    return 0;
}