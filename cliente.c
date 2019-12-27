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
struct mymsgbuf qbuffer;

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
      //TODO: Enviar mensaje de pedir por cola y recibir menú.
      tipo=1;//El tipo habria que incrementarlo de alguna manera dentro del bucle para cada cliente ser uno diferente
      qbuffer.mtype=tipo;
      strncpy(qbuffer.mtext,"pedir",MAX_COLA);//Aqui se tiene que pasar el menu al cliente?
      msgsnd(msgqueue_id,&qbuffer,MAX_COLA,0); //No se va a quedar bloqueado aqui(nunca debe estar llena la cola aqui)
      
      msgrcv(msgqueue_id,&qbuffer,MAX_COLA,tipo,0);
   } else {
      printf("La entrada introducida es incorrecta.\n");
   }
}
