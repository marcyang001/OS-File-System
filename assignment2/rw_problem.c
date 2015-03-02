#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>


static int glob = 0;
static sem_t sem;

sem_t rw_mutex; //global variable shared with reader and writer
sem_t mutex;  // mutual exclusion variable (how many readers)
int read_count = 0;
int readerIteration;
int writerIteration;



void * Writer(void *arg) {

	int i = 0;
	while (i < writerIteration) {
	//do{
		
		int randomTime = rand() % 101000;
		int WriterNumber = (int)arg;
		wait(rw_mutex);

		//start of critical section
		int temp = glob; 
		glob = glob + 10; 

		printf("Writer %d increments the value from %d to %d\n", WriterNumber,temp, glob);
		// end of critical section

		//sleep(1);
		usleep(101000);
		signal(rw_mutex);
		
		i++;		
	}
}

void *Reader (void *arg) {

	int i = 0; 
	while (i < readerIteration) {
	//do {

		int randomTime = rand() % 101000;

		int ReaderNumber = (int) arg;
		wait(mutex); 
		read_count++;
		if (read_count ==1) 
			wait(rw_mutex);
		signal(mutex);

		//start of critical section
		printf("Reader %d is reading the number: %d\n", ReaderNumber,glob);
		//end of critical section		

		//sleep(1);
		usleep(101000);
		wait(mutex);
		read_count--; 

		if(read_count ==0) 
			signal(rw_mutex);
		signal(mutex);

		i++;
	}
	//while(1);


}




int main(int argc, char *argv[]) {

	int nbReader = 10;
	int nbWriter = 6;
	pthread_t t_read[nbReader], t_write[nbWriter];
	pthread_t r1, w1;
	int s, i, j; 
	
	readerIteration = atoi(argv[1]);
	writerIteration = atoi(argv[2]);


	int nbIteration = 10;

	//initialize semaphore rw_mutex = 1 
  	if (sem_init(&rw_mutex, 0, 1) == -1) {
    	printf("Error, init semaphore\n");
    	exit(1);
  	}

  	//initialize semaphore mutex = 1 
  	if (sem_init(&mutex, 0, 1) == -1) {
    	printf("Error, init semaphore\n");
    	exit(1);
  	}

  	

  	//creation of 100 reader threads 
	for (i = 0; i < nbReader; i++) {
		if (i < nbReader) {
			s = pthread_create(&t_read[i], NULL, Reader, (void *)i);
		}

	}
	// creation of 10 writer threads
	
	for (j = 0; j < nbWriter; j++){
		
		s = pthread_create(&t_write[j], NULL, Writer, (void *)j);
	}


	for (i = 0; i<nbReader; i++) {
		if( i < nbReader)
			s = pthread_join(t_read[i], NULL);
	}

	for (j = 0; j<nbWriter; j++) {
		if(j < nbWriter)
			s = pthread_join(t_write[j], NULL);
	}



	return 0;

}