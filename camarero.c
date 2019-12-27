#include "<stdio.h>"
#include "<semaphore.h>"
#include "<fcntl.h>"
#include "<stdlib.h>"
#include "<unistd.h>"

key_t clave;
sem_t *llamar=NULL;
int msgqueue_id;

int inicializar() {
   //Abrir semaforo
   llamar = sem_open("llamar_camarero", 0);
   clave=ftok(".",'m');
   if ((msgqueue_id=msgget(clave,IPC_CREAT|0660))==-1) {
      printf("Error al iniciar la cola\n");
   }
}

int main() {
   inicializar();
   //TODO: Hacer bucle esperando mensaje de pedir de un cliente.

}

