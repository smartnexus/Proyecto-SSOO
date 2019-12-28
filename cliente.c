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
#include <assert.h>

#define MAX_COLA 80
#define PEDIR 1
#define SERVIR 2

struct mymsgbuf{
   long mtype;
   char mtext[MAX_COLA];
};

key_t clave;
sem_t *llamar=NULL;
int msgqueue_id;
struct mymsgbuf qbuffer;

void convertToArray(char * arr[], char list[], int size) {
   char *item = strtok(list, ",");
   char **ptr = arr;

   int i = 0;
   while(item != NULL) {
      ptr[i] = item;
      item = strtok(NULL, ",");
      i++;
   }
   for (i = 0; i < size; i++)
      printf("String %d : %s\n", i+1, ptr[i]);
}

void inicializar() {
   //Abrir semaforo
   llamar = sem_open("llamar_camarero", 0);
   clave=ftok(".",'m');
   if ((msgqueue_id=msgget(clave,IPC_CREAT|0660))==-1) {
      printf("Error al iniciar la cola\n");
   }
}

int main() {
   int num_mesa=0;
   int error=1;
   inicializar();
   while (error==1){
      printf("¡Bienvenido!, por favor indique el ńumero de mesa: ");
      scanf("%d", &num_mesa);
      if(num_mesa > 0) {
         error=0;
      } else {
         printf("Número erróneo.\n");
      }
   }
   fgetc(stdin);   
   printf("Pulse ENTER para llamar al camarero.");
   char ch=fgetc(stdin);
   if(ch==0x0A) {
      printf("Se ha llamado al camarero, espere por favor.\n");
      sem_wait(llamar);
      //TODO: Enviar mensaje de pedir por cola y recibir menú. Cómo solo hay una cola entre camarero y mesas el tipo es 1 para pedir y 2 para servir.
      qbuffer.mtype=PEDIR;
      char mesa[10];
      sprintf(mesa, "%d", num_mesa); 
      strncpy(qbuffer.mtext, mesa, MAX_COLA);
      msgsnd(msgqueue_id, &qbuffer, MAX_COLA, 0); 
      msgrcv(msgqueue_id, &qbuffer, MAX_COLA, SERVIR, 0);
      char bebidas[] = qbuffer.mtext;
      msgrcv(msgqueue_id, &qbuffer, MAX_COLA, SERVIR, 0);
      char comidas[] = qbuffer.mtext;
      msgrcv(msgqueue_id, &qbuffer, MAX_COLA, SERVIR, 0);
      char postres[] = qbuffer.mtext;
      
      char *lista_bebidas[10];
      convertToArray(lista_bebidas, bebidas, 10); 
      char *lista_comidas[10];
      convertToArray(lista_comidas, comidas, 10); 
      char *lista_postres[10];
      convertToArray(lista_postres, postres, 10);


      //TODO: Recibir menú y mostrar para selección.
      
      printf("=-=-=-=-=-=-=-=-=-=-=\nEl menú de hoy es:\n");
      printf("1. Bebida\n2. Entrante\n3. Plato\n=-=-=-=-=-=-=-=-=-=-=\n");
      printf("Para pedir vaya presionando introduciendo el número del producto y un 0 cuando haya terminado la comanda.\n");
      int producto = -1;
      while(producto != 0) {
         printf("Introduzca un número: ");
         scanf("%d", &producto);
         switch(producto) {
            case 1:
               printf("Producto seleccionado: 1x BEBIDA\n");
               qbuffer.mtype=PEDIR;
               strncpy(qbuffer.mtext, "BEBIDA", MAX_COLA);
               msgsnd(msgqueue_id, &qbuffer, MAX_COLA, 0);
               break; 
            case 2:
               printf("Producto seleccionado: 1x COMIDA\n");
               qbuffer.mtype=PEDIR;
               strncpy(qbuffer.mtext, "COMIDA", MAX_COLA);
               msgsnd(msgqueue_id, &qbuffer, MAX_COLA, 0);
               break;
            case 3:
               printf("Producto seleccionado: 1x POSTRE\n");
               qbuffer.mtype=PEDIR;
               strncpy(qbuffer.mtext, "POSTRE", MAX_COLA);
               msgsnd(msgqueue_id, &qbuffer, MAX_COLA, 0);
               break;
            case 0:
               printf("Finalizando servicio.\n");
               qbuffer.mtype=PEDIR;
               strncpy(qbuffer.mtext, "FIN", MAX_COLA);
               msgsnd(msgqueue_id, &qbuffer, MAX_COLA, 0);
               break;
         }
      }
   } else {
      printf("La entrada introducida es incorrecta.\n");
   }
}
