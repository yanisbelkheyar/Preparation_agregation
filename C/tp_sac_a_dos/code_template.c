#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<assert.h>
#include<math.h>
#include<pthread.h>                                                            
//Ã  compiler sous linux via gcc start_code.c -lm 

int  n    = 1000;
long pMax = 60000;
long poids_div = 5000;      //<pMAx
long vals_div  = 100;

long* vals;            //profits
long* poids;
int print_final_opt = 0;//afficher le optleau Ã  la fin (pour un pb de petite taille)
int i;

void init_sad()
{
    assert(poids_div<pMax);
    vals = (long*) calloc(n, sizeof(long));
    poids = (long*) calloc(n, sizeof(long));
    long a = 7;
    long b = 11;
    for(i=0;i<n;i++){
        a = a *7 % vals_div;
        b = b *11% poids_div;
        vals[i]  =  a + 1;
        poids[i] =  b + 1;
        assert(poids[i]<=pMax);
    }
}

void print_sad(){
    printf("Articles: %d, pMax=%ld:\nProfs:", n,pMax);
    for(i=0;i<n;i++)
        printf("%5ld ",vals[i]);
    printf("\nPoids:");
    for(i=0;i<n;i++)
        printf("%5ld ",poids[i]);
    printf("\n");
}


long calc_valBorneSup(){
}

void main(int argc, char** argv){
    init_sad();
    if(print_final_opt)
        print_sad();
    long bsup = calc_valBorneSup();
    printf("bsup=%ld\n",bsup);
}
