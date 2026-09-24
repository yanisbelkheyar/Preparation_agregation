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
    int tri_fini=0;
    while (!tri_fini){
        tri_fini=1;
        for (int i=0; i<n-1;i++){
            if(vals[i]*poids[i+1]<vals[i+1]*poids[i]){
                tri_fini=0;
                int val_tmp = vals[i];
                vals[i]=vals[i+1];
                vals[i+1]=val_tmp;
                int poids_tmp = poids[i];
                poids[i]=poids[i+1];
                poids[i+1]=poids_tmp;
            }
        }
    }
    long borne=0;
    long reste=pMax;
    for (int i=0;i<n;i++){
        if (reste>poids[i]){
            reste=reste-poids[i];
            borne=borne+vals[i];
        }
        else if (reste>0){
            borne=borne+((vals[i]*reste)/poids[i]);
            reste=0;
        }
        else {return borne;}
    }
    return borne;

}

int main(int argc, char** argv){
    init_sad();
    if(print_final_opt)
        print_sad();
    long bsup = calc_valBorneSup();
    printf("bsup=%ld\n",bsup);
    return 0;
}
