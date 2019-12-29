# Proyecto-SSOO
#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <sys/msg.h> 
#include <string.h>
#include <unistd.h>

#define ERR_ABR 1
#define ERR_LEER 2
#define MAX_COLA 200
#define BEBIDAS_PREPARAR 3  //Numero de cosas diferentes a servir de mas prioritarias a menos(bebidas,comidas y postres)
#define COMIDAS_PREPARAR 4
#define POSTRES_PREPARAR 5
#define BEBIDAS_SERVIR 6
#define COMIDAS_SERVIR 7
#define POSTRES_SERVIR 8

struct mymsgbuf{ 
   long mtype; 
   char mtext[MAX_COLA];
};
int qid;
struct mymsgbuf qbuffer;

void inicializar(); 

int main(int argc, char** argv){
  int i;
  
  while(1){
   for(i=3;i<=5;i++){
      msgrcv(qid,&qbuffer,MAX_COLA,i,IPC_NOWAIT);
      printf("Haciendo el producto: %s, por favor espere\n",qbuffer.mtext);
      sleep(i);
      qbuffer.mtype=i+3; //Tipo de la cola receptora, deberia ser igual al que he sacado
      msgsnd(qid,&qbuffer,MAX_COLA,0);
   }
  }
  return 0;
}
void inicializar() {
   //Abrir semaforo
   clave=ftok(".",'m');
   if ((qid=msgget(clave,IPC_CREAT|0660))==-1) {
      printf("Error al iniciar la cola\n");
   }
}
