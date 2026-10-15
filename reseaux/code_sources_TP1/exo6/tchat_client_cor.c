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

#define TAILLE_MAX 270


struct msg {
    int type;
    char pseudo[10];
    char msg[TAILLE_MAX];
};


int main(int argc, char **argv)
{
  int sockfd, rv;
  struct addrinfo hints, *servinfo, *p;
  char tab[2], pseudoclient[10];
  int i, nb1, taille_req, nbmessage;
  uint16_t nb2;
  char *token, *saveptr;

  struct msg reqm;
  char buf[TAILLE_MAX];
  
  fd_set master; // ensemble descripteurs sockets
  fd_set read_fds; // ensemble descripteurs sockets pour select()
  int fdmax; // descripteur socket maximum
  
  if(argc<2)
  {
      printf("\nErreur paramètre manquant!\n\nUsage : %s <pseudo>\n\n", argv[0]);
  }
  else
  {
    printf("Bienvenue dans ce tchat %s!\n\n", argv[1]);
    
    printf("Veuillez patientez pendant que nous vous connectons au serveur de tchat ...\n");

    /********************************************************************/
    //
    // TO DO : Creation Socket et connexion au serveur
    //
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    rv = getaddrinfo(SERVEUR, PORTS, &hints, &servinfo);

    if(rv != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }
    
    for(p = servinfo; p != NULL; p = p->ai_next) 
    {
        if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
        perror("client: socket");
        continue;
        }
        if((connect(sockfd, p->ai_addr, p->ai_addrlen)) == -1) {
        close(sockfd);
        perror("client: connect");
        continue;
        }
        break;
    }
    /********************************************************************/

    if(p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(servinfo); 

    /********************************************************************/
    //
    // TO DO : Envoie pseudo au serveur pour ajout dans liste utilisateurs
    //
    
    // initialisation structure requete
    reqm.type=1;
    strcpy(reqm.pseudo, argv[1]);
    
    // preparation chaine caracteres avec contenu message à envoyer
    snprintf(buf, TAILLE_MAX+10, "%u,%s,", htons((uint16_t) reqm.type), reqm.pseudo);
    
    // calcul taille message à envoyer
    taille_req=strlen(buf);
    nb2=htons((uint16_t) taille_req);
    memcpy(tab, &nb2, 2);
    
    // envoi taille du message avec pseudo
    send(sockfd, tab, 2, 0);
    
    // envoie message avec pseudo
    send(sockfd, buf, taille_req, 0);
    
    /********************************************************************/

    printf("\nVous etes bien connecté au tchat!\n");
    
    printf("\nEntrez un message : ");
    fflush(stdout);

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
                printf("\33[2K\r"); // Clear entire line
                fflush(stdout);
                
                /********************************************************************/
                //
                // TO DO : Reception message du serveur à afficher
                //
                
                // reset chaines de caractères
                memset(reqm.pseudo, 0, sizeof(reqm.pseudo));
                memset(reqm.msg, 0, sizeof(reqm.msg));
                memset(buf, 0, sizeof(buf));
                memset(tab, 0, sizeof(tab));
                
                // réception taille requete avec message de type 2
                recv(sockfd, tab, 2, 0);
                memcpy(&nb2, tab, 2);
                taille_req=(int) ntohs(nb2);
                
                // réception requete avec message de type 2
                recv(sockfd, buf, taille_req, 0);
                
                // extraction champ avec type requete
                token=strtok_r(buf, ",", &saveptr);
                sscanf(token, "%u", &nb2);
                reqm.type=(int) ntohs(nb2);
                
                // extraction champ avec pseudo
                token=strtok_r(NULL, ",", &saveptr);
                strcpy(reqm.pseudo, token);
                
                // extraction champ avec message
                token=strtok_r(NULL, ",", &saveptr);
                strcpy(reqm.msg, token);
                
                printf("%s : %s\n", reqm.pseudo, reqm.msg);
                
                /********************************************************************/
                
                printf("\nEntrez un message : ");
                fflush(stdout);
            }
            else{                
                // reset chaines de caractères
                memset(reqm.pseudo, 0, sizeof(reqm.pseudo));
                memset(reqm.msg, 0, sizeof(reqm.msg));
                memset(buf, 0, sizeof(buf));
                memset(tab, 0, sizeof(tab));
                
                fgets(buf, TAILLE_MAX, stdin);
                buf[strlen(buf) - 1] = '\0';
            
                reqm.type = 2;
                strcpy(reqm.pseudo, argv[1]);
                strcpy(reqm.msg, buf);

                printf("\x1b[1F"); // Move cursor to beginning of previous line
                printf("\33[2K\r"); // Clear entire line
                fflush(stdout);
                
                /********************************************************************/
                //
                // TO DO : Envoi nouveau message au serveur
                //
    
                // preparation chaine caracteres avec contenu message à envoyer
                snprintf(buf, TAILLE_MAX+10, "%u,%s,%s,", htons((uint16_t) reqm.type), reqm.pseudo, reqm.msg);
    
                // calcul taille message à envoyer
                taille_req=strlen(buf);
                nb2=htons((uint16_t) taille_req);
                memcpy(tab, &nb2, 2);
    
                // envoi taille du message avec pseudo
                send(sockfd, tab, 2, 0);
    
                // envoie message avec pseudo
                send(sockfd, buf, taille_req, 0);
                
                /********************************************************************/
                
                }
            }
        }     
    }
  }

    exit(0);
  }

