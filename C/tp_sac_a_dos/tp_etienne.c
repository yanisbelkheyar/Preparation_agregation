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

long max(long a, long b) {
    return a > b ? a : b;
}

long min(long a, long b) {
    return a < b ? a : b;
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

struct mon_thread_args
{
   long debut;
   long fin;
   long i;
   long ** opt;
};


void* calculThread(long debut,long fin,long i, long** opt){
    // printf("thread n° %ld and debut: %ld fin %ld et i %ld\n",(unsigned long)pthread_self(),debut,fin,i);
    for(long p=debut;p<=fin;p++){
        if (p>=poids[i]){
            opt[i][p]=max(opt[i-1][p],opt[i-1][p-poids[i]]+vals[i]);
        }
        else opt[i][p]=opt[i-1][p];
    }
}

void *calculThreadsStruct(void *pointeurArguments)
{
    struct mon_thread_args *p = pointeurArguments;
    calculThread(p->debut, p->fin,p->i,p->opt);
}

long calc_dynamique(){
    pthread_t thread[8];
    struct mon_thread_args mes_structs[8];
    long** opt=(long **) calloc(n,sizeof(long*));
    for (long i=0;i<n;i++){
        opt[i]= (long *) calloc(pMax,sizeof(long));
    }
    opt[0][poids[0]]=vals[0];
    for (long i=1;i<n;i++){
        for (int k=0;k<7;k++){
            mes_structs[k].debut=k*(pMax/8);
            mes_structs[k].fin=(k+1)*(pMax/8)-1;
            mes_structs[k].opt=opt;
            mes_structs[k].i=i;
            pthread_create(&thread[k], NULL, calculThreadsStruct, &mes_structs[k]);
        }
        mes_structs[7].debut=7*(pMax/8);
        mes_structs[7].fin=pMax;
        mes_structs[7].opt=opt;
        mes_structs[7].i=i;
        pthread_create(&thread[7], NULL, calculThreadsStruct, &mes_structs[7]);
        for (int i=0;i<8;i++){ 
             pthread_join(thread[i], NULL);
        }
    }
    long maxi=opt[n-1][0];
    for(long p=1;p<=pMax;p++){
        maxi=max(maxi,opt[n-1][p]);
    }
    return(maxi);
}

int main(int argc, char** argv){
    init_sad();
    if(print_final_opt)
        print_sad();
    long bsup = calc_dynamique();
    printf("bsup=%ld\n",bsup);
    return 0;
}
