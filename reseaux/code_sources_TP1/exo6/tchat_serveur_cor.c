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
  int sockfd, newsockfd, compteur=0;
  struct addrinfo s_init, *servinfo, *p;
  struct sockaddr client_addr;
  socklen_t s_taille;
  
  char tab_users[10][10]; // tableau stockant les pseudos des utilisateurs connectés
  int tab_sock_users[10]; // tableau stockant les descripteurs de socket associés aux utilisateurs

  char tab[2];
  struct msg reqm;
  char buf[TAILLE_MAX+10];
  
  int i, nb1, taille_req, type_req, j;
  uint16_t nb2;
  char *token, *saveptr;
  
  fd_set master; // ensemble descripteurs sockets
  fd_set read_fds; // ensemble descripteurs sockets pour select()
  int fdmax; // descripteur socket maximum

  
  /********************************************************************/
  //
  // TO DO : Création Socket d'écoute et attachement
  //
  memset(&s_init, 0, sizeof(s_init));
  s_init.ai_family = AF_INET;
  s_init.ai_socktype = SOCK_STREAM;
  s_init.ai_flags = AI_PASSIVE;

  if (getaddrinfo(NULL, PORTS, &s_init, &servinfo) != 0) {
    fprintf(stderr, "Erreur getaddrinfo\n");
    exit(1);
  }
	  
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("Serveur: socket");
      continue;
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("Serveur: erreur bind");
      continue;
    }
    break;
  }
  /********************************************************************/

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
              
              /********************************************************************/
              //
              // TO DO : Acceptation et gestion nouvelle connexion d'un utilisateur
              //
              
              if(compteur<10)
              {
                // acceptation nouvelle connexion d'un utilisateur
                s_taille = sizeof(client_addr);
                newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &s_taille);
                
                if (newsockfd == -1) {
                  perror("accept");
                  continue;
                }
                
                // reset chaines de caractères
                memset(reqm.pseudo, 0, sizeof(reqm.pseudo));
                memset(reqm.msg, 0, sizeof(reqm.msg));
                memset(buf, 0, sizeof(buf));
                
                // réception taille requete avec pseudo
                recv(newsockfd, tab, 2, 0);
                memcpy(&nb2, tab, 2);
                taille_req=(int) ntohs(nb2);
                
                // réception requete avec pseudo
                recv(newsockfd, buf, taille_req, 0);
                
                // extraction champ avec type requete
                token=strtok_r(buf, ",", &saveptr);
                sscanf(token, "%u", &nb2);
                reqm.type=(int) ntohs(nb2);
                
                // test si requete de type 1 avec pseudo est reçue
                if(reqm.type==1)
                {
                  // extraction champ avec pseudo
                  token=strtok_r(NULL, ",", &saveptr);
                  sscanf(token, "%s", reqm.pseudo);
                  
                  // sauvegarde socket service et pseudo nouvel utilisateur
                  tab_sock_users[compteur]=newsockfd;
                  strcpy(tab_users[compteur], reqm.pseudo);
                  
                  // ajout socket service dans ensemble socket à surveiller par select
                  FD_SET(newsockfd, &master);
                  
                  // mise à jour fdmax avec plus grand identifiant de socket ouvert
                  if(newsockfd>fdmax)
                  {
                    fdmax=newsockfd;
                  }
                  
                  // incrementation compteur utilisateurs connectés
                  compteur=compteur+1;
                  
                  printf("Nouvelle connexion d'un utilisateur ( %s ).\n", tab_users[compteur-1]);
                }
                else{
                  close(newsockfd);
                }
              }

              /********************************************************************/
          }
          else{
              /********************************************************************/
              //
              // TO DO : Gestion nouveau message à transférer dans le tchat
              //
              
              // reset chaine caractères buf
              memset(buf, 0, sizeof(buf));
              
              // réception taille requete avec message de type 2
              recv(i, tab, 2, 0);
              memcpy(&nb2, tab, 2);
              taille_req=(int) ntohs(nb2);
                
              // réception requete avec message de type 2
              recv(i, buf, taille_req, 0);
              
              printf("Message reçu : %s\n", buf);
              
              // retransmission du message de type 2 vers tous les clients connectés
              for(j=0; j<compteur; j++)
              {
                send(tab_sock_users[j], tab, 2, 0);
                send(tab_sock_users[j], buf, taille_req, 0);
              }
              
              /********************************************************************/
              
          }
        }
      }     
    }
    
  exit(0);
}
