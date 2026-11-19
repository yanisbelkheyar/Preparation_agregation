#include <stdio.h>
#include <stdlib.h>
typedef struct queue_{
    int val;
    struct queue_* next;
} queue;
queue* q = NULL;
queue* qfin = NULL;
int qVide(){ //renvoyer 1 si vrai, 0 sinon
    if(q==NULL)
        return 1;
    else
        return 0;
}
void qDefiler(){
    queue* qOld = q;
    q = q->next;
    free(qOld);
}
int qEnfiler(int x){
    queue* qNew = (queue*) malloc(sizeof(queue));
    qNew -> val = x;
    qNew -> next = NULL;
    if(q==NULL){
        q = qNew;
        qfin = q;
    }else{
        qfin -> next = qNew;
        qfin = qNew;
    }
//    queue* qOld = q;
//    q = (queue*) malloc(sizeof(queue));
//    q->val = x;
//    q->next = qOld;
}
int qTete(){
    return q->val;
}
int main(int argc, char *argv[]) {
    int n=atoi(argv[1]);
    printf("n=%d\n",n);
    for(int i=0;i<n;i++)
       qEnfiler(5);
    int somme=0;
    while(!qVide()){
        // printf("qTete=%d\n",qTete());
        somme=somme+qTete();
        qDefiler();
    }
    printf("somme = %d\n",somme);
}

