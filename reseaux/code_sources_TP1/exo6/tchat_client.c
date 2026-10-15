#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>

#define STDIN 0

#define SERVEUR "127.0.0.1"
#define PORTS "55555"

#define TAILLE_MAX 260


struct msg {
    int type;
    char pseudo[10];
    char msg[TAILLE_MAX];
};


int main(int argc, char **argv)
{
  int sockfd, rv;
  struct addrinfo hints, *servinfo, *p;
  char tab[2], pseudo[10];
  int i, nb1, taille_req, nbmessage;
  uint16_t nb2;
  char *token, *saveptr;

  struct msg reqm;
  
  fd_set master; // ensemble descripteurs sockets
  fd_set read_fds; // ensemble descripteurs sockets pour select()
  int fdmax; // descripteur socket maximum

  
  printf("Bienvenue dans ce tchat!\n\n");
  printf("Quelle est votre pseudo ?\n");
  fgets(pseudo, 10, stdin);
  pseudo[strlen(pseudo)-1] = '\0';
  printf("Veuillez patientez pendant que nous vous connectons au serveur de tchat ...\n");


  //
  // TO DO : Creation Socket et connexion au serveur
  //


  if(p == NULL) {
    fprintf(stderr, "server: failed to bind\n");
    exit(2);
  }

  freeaddrinfo(servinfo); 

  //
  // TO DO : Envoie pseudo au serveur pour ajout dans liste utilisateurs
  //

  printf("\nVous etes beien connecté au tchat!\n");
  
  printf("\nEntrez un message : ");

  FD_ZERO(&master); // initialisation ensembles de sockets
  FD_ZERO(&read_fds);

  FD_SET(sockfd, &master); // Ajout sockfd à ensemble master
  FD_SET(STDIN, &master); // Ajout STDIN à ensemble master
  
  fdmax=sockfd;
  
  while(1) {    
    read_fds = master; // ensemble socket attente lecture
    if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1){
       perror("select");
       exit(4);
    }
    
    for(i = 0; i <= fdmax; i++){
      if (FD_ISSET(i, &read_fds)){
          if (i == sockfd){
            
            printf("\x1b[1F"); // Move cursor to beginning of previous line
            printf("\x1b[2K"); // Clear entire line
            
            //
            // TO DO : Reception message du serveur à afficher
            //
            
            printf("\nEntrez un message : ");
          }
          else{
            fgets(reqm.msg, TAILLE_MAX, stdin);
            reqm.msg[strlen(reqm.msg) - 1] = '\0';
            strcpy(reqm.pseudo, pseudo);
            reqm.type = 2;
            
            printf("\x1b[1F"); // Move cursor to beginning of previous line
            printf("\x1b[2K"); // Clear entire line
            
            //
            // TO DO : Envoi nouveau message au serveur
            //
            
          }
        }
      }     
    }

    exit(0);
  }

