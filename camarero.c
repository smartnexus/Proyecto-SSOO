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
#define BEBIDAS_SERVIR 6
#define COMIDAS_SERVIR 7
#define POSTRES_SERVIR 8

key_t clave;
sem_t *llamar=NULL;
int qid;
struct mymsgbuf qbuffer;

struct mymsgbuf{
   long mtype;
   char mtext[MAX_COLA];
};

void enviar_anotar(char *lista, int tipo);
void inicializar(); 

int main() {
  int fin=-1;
  int i=0;
  char *bebidas = "Nada,Cerveza,Coca-Cola,Zumo,Nestea,Aquarius,Agua,bibaerVino";
  char *comidas = "Nada,Ensaladilla,Papas Bravas,Ensalada,Tortilla,Puntillitas,Calamares,Revuelto de setas";
  char *postres = "Nada,Flan de huevo,Arroz con leche,Tarta de la abuela,Brownie,Tarta de turron,Helado,Fruta del dia";
  char *pista = "";
  inicializar();
  //TODO: Hacer bucle esperando mensaje de pedir de un cliente.
  while(fin!=0){
      printf("Esperando la llamada de algun cliente.\n");
      msgrcv(qid, &qbuffer, MAX_COLA, PEDIR, 0);
    //Recibir
    printf("Nuevo cliente en la mesa %s\n", qbuffer.mtext);     
    qbuffer.mtype=SERVIR;
    printf("Enviando la carta al cliente en la mesa  %s\n", qbuffer.mtext);
    //TODO: enviar BEBIDA COMIDA POSTRE

    while (strcmp(pista,"FIN") != 0){           
      msgrcv(qid, &qbuffer, MAX_COLA, PEDIR, 0);
      printf("Recibo %s.\n",qbuffer.mtext);
      pista = qbuffer.mtext;
      if(strcmp(pista,"BEBIDA") == 0){
	printf("Enviando bebidas.\n");
	enviar_anotar(bebidas, BEBIDAS_PEDIR);
      }
      if( strcmp(pista,"COMIDA") == 0){
	printf("Enviando comidas.\n");
	enviar_anotar(comidas, COMIDAS_PEDIR);
      }
      if( strcmp(pista,"POSTRE") == 0){
	printf("Enviando postres.\n");
	enviar_anotar(postres, POSTRES_PEDIR);
      }
      if(strcmp(pista,"FIN") == 0){
	printf("El cliente no desea nada mas.\n");
      }
    }

     for(i=BEBIDAS_SERVIR;i<=POSTRES_SERVIR;i++){ //Camarero recoge las cosas preparadas
       msgrcv(qid,&qbuffer,MAX_COLA,i,IPC_NOWAIT);
	printf("Producto: %s, listo para sevir\n",qbuffer.mtext);
       
	// qbuffer.mtype=SERVIR;
	//	msgsnd(qid,&qbuffer,MAX_COLA,IPC_NOWAIT); //Camarero le envia a los clientes las cosas hechas ¿Cliente no tiene nada aun para recibir los productos hechos?
     }
  }
  return 0;
}
//Funciones
void inicializar() {
   //Abrir semaforo
   llamar = sem_open("llamar_camarero", 0);
   clave=ftok(".",'m');
   if ((qid=msgget(clave,IPC_CREAT|0660))==-1) {
      printf("Error al iniciar la cola\n");
   }
}

void enviar_anotar(char *lista, int tipo){
  int pedido=-1;
  //Enviar carta
  printf(" lista %s\n",lista);
  strncpy(qbuffer.mtext,lista,MAX_COLA);
  qbuffer.mtype=SERVIR;
  msgsnd(qid, &qbuffer, MAX_COLA, 0);
  printf("Esperando a que los clientes decidan lo que desean tomar.\n");
  //Recibir pedidos
  while(pedido!=0){
    msgrcv(qid, &qbuffer, MAX_COLA, PEDIR, 0);
    pedido = atoi(qbuffer.mtext);
    //Añadir pedido a la cola que leera el cocinero
    if(pedido!=0){
      qbuffer.mtype=tipo;
      msgsnd(qid,&qbuffer,MAX_COLA,0);
      printf("Anotado pedido: %d\n", pedido);
    }
  }
}




