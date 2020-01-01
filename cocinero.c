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
  int j;
  int num_pedido;
  char *bebidas[] = {"Nada","Cerveza","Coca-Cola","Zumo","Nestea","Aquarius","Agua","bibaerVino"};
  char *comidas[] = {"Nada","Ensaladilla","Papas Bravas","Ensalada","Tortilla","Puntillitas","Calamares","Revuelto de setas"};
  char *postres[] = {"Nada","Flan de huevo","Arroz con leche","Tarta de la abuela","Brownie","Tarta de turron","Helado","Fruta del dia"};
  inicializar();
  while(1){
   for(i=BEBIDAS_PREPARAR;i<=POSTRES_PREPARAR;i++){
      if((msgrcv(qid,&qbuffer,MAX_COLA,i,IPC_NOWAIT))!=-1){
	if(i==BEBIDAS_PREPARAR){
	  num_pedido = atoi(qbuffer.mtext);
	  printf("Preparando el producto: %s, por favor espere %d segundos.\n",bebidas[num_pedido],i);
	}
	if(i==COMIDAS_PREPARAR){
	  num_pedido = atoi(qbuffer.mtext);
	  printf("Preparando el producto: %s, por favor espere %d segundos.\n",comidas[num_pedido],i);
	}
	if(i==POSTRES_PREPARAR){
	  num_pedido = atoi(qbuffer.mtext);
	  printf("Preparando el producto: %s, por favor espere %d segundos.\n",postres[num_pedido],i);
	}
         sleep(i);
         j=i;
         qbuffer.mtype=j+3; //Tipo de las cosas enviar es igual +3
         msgsnd(qid,&qbuffer,MAX_COLA,IPC_NOWAIT);
	 printf("Listo.\n");
        }
   }
  }
  return 0;
}
void inicializar() {
   key_t clave;
   clave=ftok(".",'m');
   if ((qid=msgget(clave,IPC_CREAT|0660))==-1) {
      printf("Error al iniciar la cola\n");
   }
}
