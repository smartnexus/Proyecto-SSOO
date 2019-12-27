#include "<stdio.h>"
#include "<semaphore.h>"
#include "<fcntl.h>"
#include "<stdlib.h>"
#include "<unistd.h>"

int inicializar() {
   //Abrir semaforo
   sem_t *semID=NULL;
   semID = sem_open("llamar_camarero", 0);
}

int main(){
   inicializar();



}
