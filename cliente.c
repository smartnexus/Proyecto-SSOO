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
		
   //TODO: Hacer bucle para que no se salga del programa cuando se pulsa la tecla incorrecta.
   //Idea: Podemos poner que el menu sea numerado(es decir pulso el numero de lo que quiero) y que pulsando el 0 se acabe de pedir porque el bucle asi es while(0)
   //Menu Ejemplo de lo anterior
   //1.CocaCola
   //0.Salir
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
      printf("Texto: %s \n", qbuffer.mtext);
   } else {
      printf("La entrada introducida es incorrecta.\n");
   }
}
