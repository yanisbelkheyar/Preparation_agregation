#include<stdio.h>
#include<queue>
#include <string>

std::queue<int> file ;
int main(int argc, char* argv[]) {
    int n=std::stoi(argv[1]);
    printf("n=%d\n",n);
    for(int i=0;i<n;i++) 
       file.push(5);
    int somme=0;
    while(file.size()>0) {
      somme = somme+file.front();
      file.pop();
    }
    printf("somme = %d\n",somme);
}
