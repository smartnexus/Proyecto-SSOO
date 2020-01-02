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

int main(){
   key_t key; 
   int msgqueue_id;
   sem_t *semID=NULL;
    
   //Crear semaforo
   semID = sem_open("llamar_camarero", O_CREAT|IPC_EXCL, 0600,1);

   //Crear cola
   key=ftok(".",'m');
   //Crea nueva cola  
   msgqueue_id=msgget(key,IPC_CREAT|0660);
    
   return 0;
}
