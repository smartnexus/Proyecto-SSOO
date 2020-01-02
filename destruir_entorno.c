#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>

int main (int argc, char *argv[]) {

   sem_unlink("llamar_camarero");

   return 0;

}
