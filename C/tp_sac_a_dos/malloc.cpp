#include<iostream>
using namespace std;
int main(){
    int i;
    int n = 30;    
    int z = 1<<n;         //2^n
    z++;
    //int* tab = (int*)calloc(z, sizeof(int));
    int* tab = (int*)malloc(z * sizeof(int));
    // for(i=0;i<z;i++)
    //     tab[i] = 0;
    for(i=0;i<n;i++)
        if( tab[1<<(i+1)] < 7 + tab[1<<i])
            tab[1<<(i+1)] = 7 + tab[1<<i];
    cout<<tab[z-1]<<endl; //always 7*n
}

