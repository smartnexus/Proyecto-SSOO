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

key_t clave;
sem_t *llamar=NULL;
int msgqueue_id;

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
   printf("Pulse Enter para llamar al camarero:");
   char ch=fgetc(stdin);
   if(ch==0x0A) {
      printf("Se ha llamado al camarero, espere por favor.\n");
      sem_wait(llamar);
      //TODO: Enviar mensaje de pedir por cola y recibir menú.
   } else {
      printf("La entrada introducida es incorrecta.\n");
   }
}
