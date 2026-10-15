#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORTS "55555"

#define TAILLE_MAX 260


struct msg {
    int type;
    char pseudo[10];
    char msg[TAILLE_MAX];
};


int main(int argc, char **argv)
{
  int sockfd, newsockfd, compteur =0;
  struct addrinfo s_init, *servinfo, *p;
  struct sockaddr client_addr;
  socklen_t s_taille;
  
  char tab_users[10][10]; // tableau stockant les pseudos des utilisateurs connectés
  int tab_sock_users[10]; // tableau stockant les descripteurs de socket associés aux utilisateurs

  char tab[2];
  struct msg m;
  
  int i, nb1, taille_req, type_req;
  uint16_t nb2;
  char *token, *saveptr;
  
  fd_set master; // ensemble descripteurs sockets
  fd_set read_fds; // ensemble descripteurs sockets pour select()
  int fdmax; // descripteur socket maximum

  
  
  //
  // TO DO : Création Socket d'écoute
  //

  if (p == NULL) {
    fprintf(stderr, "Serveur: echec bind\n");
    exit(2);
  }
	  
  freeaddrinfo(servinfo);

  if (listen(sockfd, 5) == -1) {
    perror("listen");
    exit(1);
  }
  
  printf("Serveur de tchat démarré et en attente de connexion ...\n");

  FD_ZERO(&master); // initialisation ensembles de sockets
  FD_ZERO(&read_fds);

  //
  // TO DO : Création socket et attachement
  //

  FD_SET(sockfd, &master); // Ajout sockfd à ensemble master
  fdmax = sockfd; // Garde valeur max descripteur socket
  
  while(1) {
    read_fds = master; // ensemble socket attente lecture
    if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1){
       perror("select");
       exit(4);
    }
    
    for(i = 0; i <= fdmax; i++){
      if (FD_ISSET(i, &read_fds)){
          if (i == sockfd){
              
              //
              // TO DO : Acceptation et gestion nouvelle connexion d'un utilisateur
              //

            printf("Nouvelle connexion d'un utilisateur.\n");
          }
          else{
              
              //
              // TO DO : Gestion nouveau message à transférer dans le tchat
              //
              
          }
        }
      }     
    }
    
  exit(0);
}
