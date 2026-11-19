#include <stdio.h>
#define QFILESIZEMAX 25000000       
int q[QFILESIZEMAX];
int premier=0;
int dernier=-1;
int qVide(){ //renvoyer 1 si vrai, 0 sinon
    return(premier>dernier);
}
void qDefiler(){
    premier++;
}
int qEnfiler(int x){
    dernier++;
    q[dernier]=x;
}
int qTete(){
    return q[premier];
}
int main(){
    for(int i=0;i<25000000;i++)
       qEnfiler(5);
    int somme=0;
    while(!qVide()){
        somme=somme+qTete();
        qDefiler();
    }
    printf("somme = %d\n",somme);
}

