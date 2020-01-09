#include <sys/wait.h> 
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <ctype.h> 
#include <sys/msg.h> 
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/shm.h>

#define MAX_COLA 200
#define PEDIR 1
#define SERVIR 2
#define BEBIDAS_PEDIR 3
#define COMIDAS_PEDIR 4
#define POSTRES_PEDIR 5
#define RECOGER_SERVIR 6

key_t clave;
sem_t *llamar=NULL;
int qid;
struct mymsgbuf qbuffer;

struct mymsgbuf{
   long mtype;
   char mtext[MAX_COLA];
};

void enviar_anotar(char *lista, int tipo, char mesa[]);
void convertToArray(char * arr[], char list[], int size);
void componer_msg (char pedido_c[10], int pedido, int pid);
void inicializar(); 

int main() {
  int fin=-1;	
  char *bebidas = "Nada,Cerveza,Coca-Cola,Zumo,Nestea,Aquarius,Agua,Vino";
  char *comidas = "Nada,Ensaladilla,Papas Bravas,Ensalada,Tortilla,Puntillitas,Calamares,Revuelto de setas";
  char *postres = "Nada,Flan de huevo,Arroz con leche,Tarta de la abuela,Brownie,Tarta de turron,Helado,Fruta del dia";
  char *pista = "";
  char mesa[5];
  inicializar();
  while(fin!=0){
    printf("Esperando la llamada de algun cliente...\n");
    msgrcv(qid, &qbuffer, MAX_COLA, PEDIR, 0);
    printf("Nuevo cliente en la mesa %s\n", qbuffer.mtext);
    sprintf(mesa, "%s", qbuffer.mtext);
    qbuffer.mtype=SERVIR;
    printf("[Mesa %s] Enviando la carta principal.\n", mesa);

    while (strcmp(pista,"FIN") != 0) {           
      msgrcv(qid, &qbuffer, MAX_COLA, PEDIR, 0);
      printf("[Mesa %s] Recibo %s.\n", mesa, qbuffer.mtext);
      pista = qbuffer.mtext;
      if(strcmp(pista,"BEBIDA") == 0){
	printf("[Mesa %s] Enviando carta de bebidas: ", mesa);
	enviar_anotar(bebidas, BEBIDAS_PEDIR, mesa);
      }
      if( strcmp(pista,"COMIDA") == 0){
	printf("[Mesa %s] Enviando carta de comidas: ", mesa);
	enviar_anotar(comidas, COMIDAS_PEDIR, mesa);
      }
      if( strcmp(pista,"POSTRE") == 0){
	printf("[Mesa %s] Enviando carta de postres: ", mesa);
	enviar_anotar(postres, POSTRES_PEDIR, mesa);
      }
      if(strcmp(pista,"FIN") == 0){
	printf("[Mesa %s] El cliente no desea nada mas.\n", mesa);
      }
    }
       
	// qbuffer.mtype=SERVIR;
	//	msgsnd(qid,&qbuffer,MAX_COLA,IPC_NOWAIT); //Camarero le envia a los clientes las cosas hechas ¿Cliente no tiene nada aun para recibir los productos hechos?
  }
  return 0;
}
//Funciones
void convertToArray(char * arr[], char list[], int size) {
   char *item = strtok(list, "-");
   char **ptr = arr;

   int i = 0;
   while(item != NULL) {
      ptr[i] = item;
      item = strtok(NULL, ",");
      i++;
   }
}

void inicializar() {
   //Abrir semaforo
   llamar = sem_open("llamar_camarero", 0);
   clave=ftok(".",'m');
   if ((qid=msgget(clave,IPC_CREAT|0660))==-1) {
      printf("Error al iniciar la cola\n");
   }
}
void componer_msg (char pedido_c[10], int pedido, int pid) {
   char pid_c[5];
   sprintf(pedido_c, "%d", pedido);
   sprintf(pid_c, "%d", pid);
   strcat(pedido_c, "-");
   strcat(pedido_c, pid_c);
}
void enviar_anotar(char *lista, int tipo, char mesa[]){
  int pedido=-1;
  //Enviar carta
  printf("{%s}\n",lista);
  strncpy(qbuffer.mtext,lista,MAX_COLA);
  qbuffer.mtype=SERVIR;
  msgsnd(qid, &qbuffer, MAX_COLA, 0);
  printf("[Mesa %s] Esperando a que los clientes decidan lo que desean tomar.\n", mesa);
  //Recibir pedidos
  while(pedido!=0){
    msgrcv(qid, &qbuffer, MAX_COLA, PEDIR, 0);
    char *arr[2];
    char **ptr = arr;
    convertToArray(arr, qbuffer.mtext, 2);
    pedido = atoi(ptr[0]);
    //Añadir pedido a la cola que leera el cocinero
    if(pedido!=0){
      qbuffer.mtype=tipo;
      char pedido_c[10];
      componer_msg(pedido_c, pedido, atoi(ptr[1]));
      strncpy(qbuffer.mtext, pedido_c, MAX_COLA);
      msgsnd(qid, &qbuffer, MAX_COLA, 0);
      printf("[Mesa %s] Anotado pedido: %d\n", ptr[1], pedido);
    }
  }
}




