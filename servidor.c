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
#define SIG_GET 100
#define PEDIR 1
#define SERVIR 2
#define BEBIDAS_PEDIR 3
#define COMIDAS_PEDIR 4
#define POSTRES_PEDIR 5
#define RECOGER_SERVIR 6
#define SINCRO 7

key_t clave;
sem_t *llamar=NULL;
int qid;
struct mymsgbuf qbuffer;
struct mymsgbuf{
   long mtype;
   char mtext[MAX_COLA];
};
void inicializar();
void recoger();

int main(){
   int fin=-1;
   struct sigaction act;
   inicializar();

   act.sa_handler = recoger; /* función a ejecutar */ 
   act.sa_flags = 0; 
   sigemptyset(&act.sa_mask);   
   //sigaction(SIG_GET, &act, NULL);

   
   sigaction(SIGALRM, &act, NULL);

   while(1) pause();

   return 0;
}
void recoger() {
   printf("Señal para recoger recibida.\n");
   while(msgrcv(qid,&qbuffer,MAX_COLA,RECOGER_SERVIR,IPC_NOWAIT)!=-1) {
      printf("Producto: %s, listo para servir\n",qbuffer.mtext);
   }
}
void inicializar() {
   //Abrir semaforo
   llamar = sem_open("llamar_camarero", 0);
   clave=ftok(".",'m');
   if ((qid=msgget(clave,IPC_CREAT|0660))==-1) {
      printf("Error al iniciar la cola\n");
   }
   int pid = getpid();
   char pid_t[10];
   sprintf(pid_t, "%d", pid);
   qbuffer.mtype=SINCRO;
   strncpy(qbuffer.mtext, pid_t, MAX_COLA);
   msgsnd(qid,&qbuffer,MAX_COLA,IPC_NOWAIT);
}
