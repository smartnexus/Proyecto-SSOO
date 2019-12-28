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

key_t clave;
sem_t *llamar=NULL;
int qid;
struct mymsgbuf qbuffer;
struct mymsgbuf rbuffer;



struct mymsgbuf{
   long mtype;
   char mtext[MAX_COLA];
};


void inicializar() {
   //Abrir semaforo
   llamar = sem_open("llamar_camarero", 0);
   clave=ftok(".",'m');
   if ((qid=msgget(clave,IPC_CREAT|0660))==-1) {
      printf("Error al iniciar la cola\n");
   }
}

int main() {
  int msg_rcv;
  int msg_send;
   inicializar();
   //TODO: Hacer bucle esperando mensaje de pedir de un cliente.
   while(1){////
     //Recibir
     printf("Esperando ");
     msg_rcv = msgrcv(qid,&rbuffer,MAX_COLA,PEDIR,0);
     printf("Nuevo cliente %s", rbuffer.mtext);
     





     // msg_send = msgsnd(qid,qbuf,MAX_SEND_SIZE,0);



   }
}

