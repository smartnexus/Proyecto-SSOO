# Proyecto-SSOO
#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <sys/msg.h> 
#include <string.h>
#include <unistd.h>

#define ERR_ABR 1
#define MAX_COLA 100
#define M_TYPES 5

struct mymsgbuf{ 
   long mtype; 
   char mtext[MAX_SEND_SIZE];
};

int inicializar(){
  int error=0;
  key_t clave;
  int msgcola_id;
  clave=ftok(".",'m');
  if((msgcola_id=msgget(clave,0))==-1){
    error=ERR_ABR;
  }
  return error;
}
int main(int argc, char** argv){
  struct mymsgbuf qbuffer;
  int error;
  if((error=inicializar())!=0){
    printf("No se ha podido acceder a la cola correctamente\n");
  }
  for(i=1;i<=M_TYPES;i++){
   msgrcv(msgcola_id,&qbuffer,MAX_COLA,i,IPC_NOWAIT);
  }  



  return 0;
}
