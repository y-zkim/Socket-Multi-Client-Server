#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 30000
#define IP "127.0.0.1"
#define MAX 1024
#define NBRE_QSTS	3

typedef struct{
	struct sockaddr_in address;
	int sockServ;
	char name[MAX];
	int Score;
} client_t;

int main(){
	printf("\n\n ************************* QUIZ LEMLIH ZKIM / SERVER SIDE *************************");
	printf("\n   -------------------------------------------------------------------------------");
	printf("\n                ........... Les rép des clients se trouvent ici ...........\n");

 printf("\n\n ******************************* BIENVENU ADMIN ***********************************\n\n");
	char line[NBRE_QSTS + 1];
	int sockServ;//descripteur socket Serveur
	int ret;//résultat du bind
	struct sockaddr_in serverAddr;//adresse serveur

	int sockClient;// descripteur socket Client
	struct sockaddr_in newAddr;

	socklen_t addr_size;//adresse size

	char buffer[MAX];//buffer pour partager les données
	pid_t childpid;//pid du processus fils

	FILE *fp1;
	FILE *fp2;
	size_t len = 0;
	int k = 0;

	sockServ = socket(AF_INET, SOCK_STREAM, 0);
	if(sockServ < 0){
		perror("[!] Erreur dans la création du socket serveur !");
		exit(1);
	}
	printf("[+] La socket du serveur a bien été crée.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(IP);

	ret = bind(sockServ, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		perror("[!] Erreur : Bind a échoué !");
		exit(1);
	}
	printf("[+] Bind effectué avec succès.\n");

	if(listen(sockServ, 5) == 0){
		printf("[+] Ecoute en cours ...\n");
	}else{
		perror("[!] Ecoute echouée !");
		exit(1);
	}



	//printf("uuuu %c -- %c -- %c -- %c\n", line[0], line[1], line[2], line[3]);
	memset(buffer, 0, strlen(buffer));
	while (1)
	{
		sockClient = accept(sockServ, (struct sockaddr *)&newAddr, &addr_size);
		if (sockClient < 0)
		{
			exit(1);
		}

		if ((childpid = fork()) == 0)
		{ //dans le processus fils
			close(sockServ);
			client_t user;
			user.sockServ = sockClient;
			user.Score = 0;
			memset(buffer, 0, strlen(buffer));

			recv(user.sockServ, buffer, sizeof(buffer), 0);
			strcpy(user.name, buffer);

			printf("[+] %s est connectee\n", buffer);
			memset(buffer, 0, strlen(buffer));
			int choix;
			recv(user.sockServ, buffer, sizeof(buffer), 0);
			printf("[+] le client a choisi le  niveau %s et les reponses de ce choix sont : ",buffer);
			if(strcmp(buffer, "1") == 0){
				choix =1;
				fp2 = fopen("response.txt", "r");
				if (fp2 == NULL)
				{
					perror("[!] Erreur : le fichier des réponses ne peux pas être ouvert !");
					exit(1);
				}
				memset(line, 0, strlen(line));

				fgets(line, sizeof(line), fp2);
				printf("%s\n", line);

				fclose(fp2);
			}else if(strcmp(buffer, "2") == 0){
				choix =2;
				fp2 = fopen("response2.txt", "r");
				if (fp2 == NULL)
				{
					perror("[!] Erreur : le fichier des réponses 2 ne peux pas être ouvert !");
					exit(1);
				}
				memset(line, 0, strlen(line));

				fgets(line, sizeof(line), fp2);
				printf("%s\n", line);

				fclose(fp2);
			}else if(strcmp(buffer, "3") == 0){
				choix =3;
				fp2 = fopen("response3.txt", "r");
				if (fp2 == NULL)
				{
					perror("[!] Erreur : le fichier des réponses 3 ne peux pas être ouvert !");
					exit(1);
				}
				memset(line, 0, strlen(line));

				fgets(line, sizeof(line), fp2);
				printf("%s\n", line);

				fclose(fp2);
			}
			while (1)
			{

				if(choix == 1){
					fp1 = fopen("Questionaire1.txt", "r");

				}else if(choix == 2){
					fp1 = fopen("Questionaire2.txt", "r");

				}else if(choix == 3){
					fp1 = fopen("Questionaire3.txt", "r");

				}
				memset(buffer, 0, strlen(buffer));

				if (fp1 == NULL)
				{
					perror("[!] Erreur : le fichier des questions ne peux pas être ouvert !");
					exit(1);
				}
				memset(buffer, 0, strlen(buffer));
				//recv(user.sockServ, buffer, MAX, 0);
				//printf("[+] Un utilisateur est prêt pour le quiz");

				for (int j = 0; j < NBRE_QSTS; j++)
				{
					fgets(buffer, sizeof(buffer), fp1);
					send(user.sockServ, buffer, strlen(buffer), 0);
					//memset(buffer, 0, strlen(buffer));
					recv(user.sockServ, buffer, MAX, 0);
					if (buffer[0] == line[j])
					{
						user.Score++;
					}
					printf("[+] %s a répondu : %s\n", user.name, buffer);
					memset(buffer, 0, strlen(buffer));
				}
				printf("%s Disconnected score is %d\n", user.name, user.Score);
				sprintf(buffer, "Votre score est : %d point(s)", user.Score);
				send(user.sockServ, buffer, strlen(buffer), 0);
				memset(buffer, 0, strlen(buffer));
				close(user.sockServ);
				exit(EXIT_SUCCESS);
				memset(buffer, 0, strlen(buffer));
				if(strcmp(buffer, "exit") == 0){
					printf("%s Disconnected score is %d\n", user.name, user.Score);
					break;
				}else{
					printf("[ Client ] : %s\n", buffer);
					send(sockClient, buffer, strlen(buffer), 0);
					memset(buffer, 0, strlen(buffer));
				}
				fclose(fp1);
			}
		}
		}
		close(sockClient);
		return 0;
	}
