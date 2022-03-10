#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 30000
#define IP "127.0.0.1"
#define MAX 1024

typedef struct{
	struct sockaddr_in address;
	int sockServ;
	char name[MAX];
	int Score;
} client_t;

int main(){
	char nameC[MAX];
	int clientSocket;
	int ret;
	struct sockaddr_in serverAddr;
	char buffer[MAX];

	//create socket client
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		perror("Socket Failed");
		exit(1);
	}
	printf("socket du Client est créee SUCCES.\n");

	//Remplir les données d'aresse Client
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(IP);

	//connecter au Serveur :
	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		perror("connect Failed");
		exit(1);
	}
	printf("connexion au serveur.\n");
	//Entrez le nim du client
	printf("Bonjour,Veuillez entrez votre nom :\n");
	memset(nameC, 0, strlen(nameC));
	scanf("%s" , nameC);

	if (strlen(nameC) > 32 || strlen(nameC) < 2) {
		printf("Name must be less than 30 and more than 2 characters.\n");
		return EXIT_FAILURE;//quitter l'exécution
	}

	client_t user;
	user.Score = 0;
	strcpy(user.name, nameC);
	send(clientSocket, user.name, strlen(user.name), 0);

		printf("\n\n ************************* Avant de commancer le quizz *************************");
		printf("\n   -------------------------------------------------------------------------------");
		printf("\n                       ........... Directives ...........\n");
		printf("\n [+] Vous aurez un total de 3 questions, les questions sont choisis aléatoirement");
		printf("\n [+] Vous devez choisir la réponse juste en choisissanr A, B ou C");
		printf("\n [+] Un bonus d'un point pour chaque réponse juste");
		printf("\n [+] le score ne sera pas débité en cas de fausse réponse");

	 printf("\n\n ******************************* BONNE CHANCE ***********************************\n\n");
	//printf("[+] Taper [y] pour commencer ...\n");
	printf("|-----------------------| \n");
	printf("| Choisissez le niveau  | \n");
	printf("|-----------------------| \n");
	printf("[+] Niveau 1 : Facile \n");
	printf("    --------- \n");
	printf("[+] Niveau 2 : Moyen \n");
	printf("    --------- \n");
	printf("[+] Niveau 3 : Difficile \n");
	printf("    --------- \n");
	int choix;
	scanf("%d",&choix);
	memset(buffer, 0, strlen(buffer));
	sprintf(buffer, "%d", choix);
	send(clientSocket, buffer, sizeof(int), 0);
	memset(buffer, 0, strlen(buffer));
	while(1){
		if(strcmp(buffer, "exit") == 0){
			close(clientSocket);
			printf("[+] Déconnecté du serveur.\n");
			exit(1);
		}
		//catch_ctrl_c_and_exit(2);


		else if (recv(clientSocket, buffer, MAX, 0) < 0)
		{
			printf("[!]Erreur dans la reception des données.\n");
		}
		else if (strcmp(buffer, "exit") == 0)
		{
			close(clientSocket);
			printf("[+] Déconnecté du serveur.\n");
			exit(EXIT_SUCCESS);
		} else
		{
			printf("[ Serveur ] : %s\n", buffer);
			memset(buffer, 0, strlen(buffer));
		}
		printf("[ Client ] : ");
		memset(buffer, 0, strlen(buffer));
		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer), 0);

	}

	return 0;
}
