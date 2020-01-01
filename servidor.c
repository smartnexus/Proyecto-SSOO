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

key_t clave;
sem_t *llamar=NULL;
int qid;
struct mymsgbuf qbuffer;
struct mymsgbuf{
   long mtype;
   char mtext[MAX_COLA];
};
void inicializar();
int main(){
  int fin=-1;
  inicializar();
 while(fin!=0) {
    if(msgrcv(qid,&qbuffer,MAX_COLA,RECOGER_SERVIR,IPC_NOWAIT)!=-1) {
       printf("Producto: %s, listo para sevir\n",qbuffer.mtext);
    }
  } 


  return 0;
}
void inicializar() {
   //Abrir semaforo
   llamar = sem_open("llamar_camarero", 0);
   clave=ftok(".",'m');
   if ((qid=msgget(clave,IPC_CREAT|0660))==-1) {
      printf("Error al iniciar la cola\n");
   }
}
