#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <sys/msg.h> 
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define ERR_ABR 1
#define ERR_LEER 2
#define MAX_COLA 200
#define BEBIDAS_PREPARAR 3  //Numero de cosas diferentes a servir de mas prioritarias a menos(bebidas,comidas y postres)
#define COMIDAS_PREPARAR 4
#define POSTRES_PREPARAR 5
#define PEDIDOS_SERVIR 6
#define SINCRO 7

struct mymsgbuf{ 
   long mtype; 
   char mtext[MAX_COLA];
};
int qid;
struct mymsgbuf qbuffer;
int pid;

void inicializar(); 
void convertToArray(char * arr[], char list[], int size);
void componer_msg (char pedido_c[10], int pedido, int pid);

int main(){
   int i;
   int num_pedido;
   char pedido[5];
   char *bebidas[] = {"Nada","Cerveza","Coca-Cola","Zumo","Nestea","Aquarius","Agua","Vino"};
   char *comidas[] = {"Nada","Ensaladilla","Papas Bravas","Ensalada","Tortilla","Puntillitas","Calamares","Revuelto de setas"};
   char *postres[] = {"Nada","Flan de huevo","Arroz con leche","Tarta de la abuela","Brownie","Tarta de turrazos matematicos","Helado","Fruta del dia"};
   inicializar();
   while(1) {
      for(i=BEBIDAS_PREPARAR;i<=POSTRES_PREPARAR;i++){
         if((msgrcv(qid,&qbuffer,MAX_COLA,i,IPC_NOWAIT))!=-1){
            char *arr[2];
            char **ptr = arr;
            convertToArray(arr, qbuffer.mtext, 2);
            num_pedido = atoi(ptr[0]);
	    if(i==BEBIDAS_PREPARAR){
	       printf("[Mesa %s] Preparando el producto: %s. (Tiempo de preparación %d segundos)\n", ptr[1], bebidas[num_pedido], i);
	    }
 	    if(i==COMIDAS_PREPARAR){
	       printf("[Mesa %s] Preparando el producto: %s, (Tiempo de preparación %d segundos)\n", ptr[1], comidas[num_pedido], i);
 	    }
	    if(i==POSTRES_PREPARAR){
	       printf("[Mesa %s] Preparando el producto: %s, (Tiempo de preparación %d segundos)\n", ptr[1], postres[num_pedido], i);
	    }
            sleep(i);
            qbuffer.mtype=PEDIDOS_SERVIR;
            char pedido_c[10];
            componer_msg(pedido_c, num_pedido, atoi(ptr[1]));
            strncpy(qbuffer.mtext, pedido_c, MAX_COLA);
            msgsnd(qid,&qbuffer,MAX_COLA,IPC_NOWAIT);
	    printf("[Mesa %s] ¡Listo!\n", ptr[1]);
            kill(pid, SIGALRM);
         }
      }
   }
   return 0;
}
void convertToArray(char * arr[], char list[], int size) {
   char *item = strtok(list, "-");
   char **ptr = arr;

   int i = 0;
   while(item != NULL) {
      ptr[i] = item;
      item = strtok(NULL, ",");
      i++;
   }
}
void componer_msg (char pedido_c[10], int pedido, int pid) {
   char pid_c[5];
   sprintf(pedido_c, "%d", pedido);
   sprintf(pid_c, "%d", pid);
   strcat(pedido_c, "-");
   strcat(pedido_c, pid_c);
}
void inicializar() {
   key_t clave;
   clave=ftok(".",'m');
   if ((qid=msgget(clave,IPC_CREAT|0660))==-1) {
      printf("Error al iniciar la cola\n");
   }
   printf("Esperando sicronismo con servidor...\n");
   msgrcv(qid,&qbuffer,MAX_COLA,SINCRO,0);
   pid = atoi(qbuffer.mtext);
   printf("Cocina lista. (PID Servidor: %d)\n", pid);
}
