#include <sys/wait.h> 
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <ctype.h> 
#include <sys/msg.h> 
#include <string.h> 


#define MAX_COLA_TEXT

struct mymsgbuf{ 
  long mtype; 
  char mtext[MAX_COLA_TEXT]; 
};

int solicitar(char *lista);
int crear_cola ();
int add_cola(int qid,int num_mesa, int pedido);

int main(){
  char *bebidas[8]={"Nada","Cerveza", "Coca-Cola", "Zumo", "Nestea", "Aquarius", "Agua", "Vino"};
  char *entrantes[8]={"Nada","Ensaladilla","Papas Bravas","Ensalada","Tortilla","Puntillitas","Calamares","Revuelto de setas"};
  char *platos[5]={"Nada","Paella","Solomillo","Entrecot","Bacalao"};
  int num_mesa;
  int pedido;
  pid_t pid;
  int msgId;

  //Crear cola
  msgId=crear_cola();  

  //Crear hijo (cliente)
  pid = fork();
  
  switch (pid){
  case -1:
    printf ("Error al crear el proceso hijo");
    break;
  case 0: //hijo=cliente
    


    break;
  default: //padre=camarero
    printf("Bienvenido,\n Introduzca su numero de mesa: \n");
    scanf("%d",&num_mesa);
    printf("¿Que desea tomar?\nComencemos con la bebida: ");
    while (pedido!=0){
      pedido=solicitar(*bebidas);
      //Comprobar disponibilidad (memoria compartida con cocina)

      
      //Añadir pedido a la cola tipo numero de la mesa
      if (pedido!=0)
      add_cola(msgId, num_mesa, pedido);      
    }
    printf("Vamos a por los entrantes\n");
    while (pedido!=0){
      pedido=solicitar(*entrantes);
      //Comprobar disponibilidad (memoria compartida con cocina)
      
      
      //Añadir pedido a la cola tipo numero de la mesa
      if (pedido!=0)
      add_cola(msgId, num_mesa, pedido);      
    }
    printf("Para finalizar, ¿desean algun plato?\n");
    while (pedido!=0){
      pedido=solicitar(*platos);
      //Comprobar disponibilidad (memoria compartida con cocina)
      
      //Añadir pedido a la cola tipo numero de la mesa    
      if (pedido!=0)
      add_cola(msgId, num_mesa, pedido);      
    }  
  }
  return 0;
}



  
int solicitar(char *lista){
  int pedido;
  for (int i=0; i<sizeof(lista);i++){
    printf("%d   ----     %s\n", i,lista[i]);
  }
  printf ("Seleccionar numero: \n");
  scanf ("%d", pedido);  
  return pedido;
}

int crear_cola (){
  int msgId;
  key_t key;
  //Creo clave
  key=ftok(".",'m');
  //Crea nueva cola (si no existste)
  if ((msgId=msgget(key,IPC_CREAT|0660))==-1) 
    { 
      printf("Error al crear la cola\n"); 
    } 
  else 
    {    
      return msgId; 
    }
}
 

int add_cola(int qid,int num_mesa,int pedido) 
{ 
  int resultado;
  struct mymsgbuf *qbuf;

  qbuf->mtype=num_mesa;
  qbuf->mtext=(char *)pedido;
 
   
  resultado=msgsnd(qid,qbuf,MAX_COLA_TEXT,0);
      
  return (resultado);

} 
