#include "tas.h"

int main(int _argc, char *_argv[]) {
    tas new_tas=creer(10);
    tas* t1=&new_tas;
    for (int i=10;i>=0;i=i-1){
        inserer(t1,i);
    }
    print(t1);
    printf("====================\n");
    for (int i=8;i>=0;i=i-1){
        printf("Min : %d\n",supprime_min(t1));
        print(t1);
        printf("====================\n");
    }
    print(t1);
}