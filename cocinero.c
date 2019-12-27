# Proyecto-SSOO
#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <sys/msg.h> 
#include <string.h>
#include <unistd.h>

#define ERR_ABR 1
#define ERR_LEER 2
#define MAX_COLA 100
#define M_TYPES 5  //Numero de mesas maximas

struct mymsgbuf{ 
   long mtype; 
   char mtext[MAX_COLA];
};

int main(int argc, char** argv){
  struct mymsgbuf qbuffer_rx;
  struct mymsgbuf qbuffer_tx;
  int i;
  int error;
  int error=0;
  key_t clave;
  int msgcola_id_rx; //Cola para recibir los pedidos
  int msgcola_id_tx; //Cola para mandar los pedidos(hay que crearla en otro lado)
  
  clave=ftok(".",'m');
  
  if((msgcola_id_rx=msgget(clave,0))==-1){
    error=ERR_ABR;
    if((msgcola_id_tx=msgget(clave,0))==-1){
       error=ERR_ABR;
    }
  }
   
  while(1){
   for(i=1;i<=M_TYPES;i++){
      msgrcv(msgcola_id_rx,&qbuffer_rx,MAX_COLA,i,IPC_NOWAIT);
      printf("Haciendo el producto %s, por favor espere\n",qbuffer_rx.mtext);
      sleep(5);
      qbuffer_tx.mtype=i; //Tipo de la cola receptora, deberia ser igual al que he sacado
      strncpy(qbuffer_tx.mtext,qbuffer_rx.mtext,MAX_COLA); //Copio en la estructura de la cola transmisora el plato sacado de la cola receptora despues de prepararla
      msgsnd(msgcola_id_tx,&qbuffer_tx,MAX_COLA,0);
   }
  }



  return 0;
}
