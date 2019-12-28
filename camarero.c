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
#define BEBIDAS 3
#define ENTRANTES 4
#define PLATOS 5

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
  char *bebidas = "Nada,Cerveza,Coca-Cola,Zumo,Nestea,Aquarius,Agua,Vino";
  char *entrantes = "Nada,Ensaladilla,Papas Bravas,Ensalada,Tortilla,Puntillitas,Calamares,Revuelto de setas";
  char *platos = "Nada,Paella,Solomillo,Entrecot,Bacalao,Arroz con pollo";
  inicializar();
   //TODO: Hacer bucle esperando mensaje de pedir de un cliente.
   while(1){
    //Recibir
     printf("Esperando la llamada de algun cliente.\n");
     msgrcv(qid, &qbuffer, MAX_COLA, PEDIR, 0);
     printf("Nuevo cliente en la mesa %s\n", qbuffer.mtext);
     
     qbuffer.mtype=SERVIR;
     printf("Enviando la carta al cliente en la mesa %s\n", qbuffer.mtext);
     //Bebidas
     enviar_anotar(bebidas, BEBIDAS);
     //Entrantes
     enviar_anotar(entrantes, ENTRANTES);
     //Platos
     enviar_anotar(platos, PLATOS);     
     
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
  int pedido=100;
   //Enviar bebidas
     strncpy(qbuffer.mtext,lista,MAX_COLA);
     qbuffer.mtype=SERVIR;
     msgsnd(qid, &qbuffer, MAX_COLA, 0);
     //Recibo pedidos de bebidas
     while(pedido!=0){
       msgrcv(qid, &qbuffer, MAX_COLA, PEDIR, 0);
       pedido = atoi(qbuffer.mtext);
       //Añadir pedido a la cola que leera el cocinero
       qbuffer.mtype=tipo;
       msgsnd(qid,&qbuffer,MAX_COLA,0);
       printf("Anotado pedido: %d ", pedido);
     }
}




