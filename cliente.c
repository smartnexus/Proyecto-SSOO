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
struct mymsgbuf rbuffer;

int inicializar() {
   //Abrir semaforo
   llamar = sem_open("llamar_camarero", 0);
   clave=ftok(".",'m');
   if ((msgqueue_id=msgget(clave,IPC_CREAT|0660))==-1) {
      printf("Error al iniciar la cola\n");
   }
}

int main() {
   inicializar();
   printf("¡Bienvenido!\n");
   //TODO: Hacer bucle para que no se salga del programa cuando se pulsa la tecla incorrecta.
   //Idea: Podemos poner que el menu sea numerado(es decir pulso el numero de lo que quiero) y que pulsando el 0 se acabe de pedir porque el bucle asi es while(0)
   //Menu Ejemplo de lo anterior
   //1.CocaCola
   //0.Salir
   printf("Pulse Enter para llamar al camarero:");
   char ch=fgetc(stdin);
   if(ch==0x0A) {
      printf("Se ha llamado al camarero, espere por favor.\n");
      sem_wait(llamar);
      //TODO: Enviar mensaje de pedir por cola y recibir menú. Cómo solo hay una cola entre camarero y mesas el tipo es 1 para pedir y 2 para servir.
      qbuffer.mtype=PEDIR;
      strncpy(qbuffer.mtext,"pedir:1",MAX_COLA);
      msgsnd(msgqueue_id, &qbuffer, MAX_COLA, 0); 
      //TODO: Cliente envia texto pedir, queda esperar respuesta del camarero.
      //msgrcv(msgqueue_id, &rbuffer, MAX_COLA, PEDIR, 0);
      //printf("Tipo: %ld Texto:%s \n",qbuffer.mtype,qbuffer.mtext);
   } else {
      printf("La entrada introducida es incorrecta.\n");
   }
}
