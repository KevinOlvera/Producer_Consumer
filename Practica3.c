#include "Practica3.h"

int create_memory (){

	/**
    * Obtener el identificador IPC (LLAVE).
	*
    * @param const char *		Ruta de la memoria
    * @param int 				Identificador
    *
    * @return key_t				Llave de la memoria
    */
	key = ftok ("/bin/ls", 1111);

	if ( key == -1)
		return -1;

    /**
    * Crear el segmento de memoria compartida
	*
    * @param key_t				Llave de la memoria
    * @param size_t				Tamaño de la memoria (bytes)
    * @param int 				Macros para el modo de uso ( IPC_CREAT:Crea un nuevo segmento
    *														 IPC_EXCL: se combina con IPC_CREAT y si el segmento ya existía, 
    *																   entonces la creación falla. )
    *
    * @return int				-1:ERROR, ID del segmento de memoria.
    */
	id_memory = shmget(key, sizeof(zone)*5, 0777 | IPC_EXCL | IPC_CREAT);
	
	if ( id_memory == -1 ){
		id_memory = shmget(key, sizeof(zone)*5, 0777);
		printf("Ligando a la memoria compartida \n");
		buffer = (zone *)shmat(id_memory, (void*)0, 0);
		printf("Ligando la zona de datos a la memoria compartida\n");
		return 0;
	}
	else 
	printf("Se creo la memoria compartida\n");
	
	/**
	* Ligar datos al segmento de memoria compartida.
	*
	* @param int 				Id del segmento de memoria
	* @param ?
	* @param ?
	*
	* @return ?
	**/
	buffer = (zone *)shmat(id_memory, (void*)0, 0);
	printf("Ligando la zona de datos a la memoria compartida\n");

	return 1;
}


void delete_memory(){

	/**
	* Desasocia la zona de datos con la memoria compartida.
	*
	* @param size_t *				Tipo de dato de la zona de datos
	*
	* @return ?
	**/
	shmdt (buffer);
	printf("Desligando la zona de datos de la memoria compartida.\n");
	
	/**
	* Recibe informacion de la memoria compartida, establece permisos o la elimina. 
	*
	* @param imt					Id del segmento de memoria
	* @param int 					Macros para la operacion ( IPC_RMID:Elimina el segmento de memoria )
	* @param ?
	*
	* @return ?
	**/
	shmctl (id_memory, IPC_RMID, (struct shmid_ds *)NULL);
	printf("Eliminando el segmento de memoria compartida.\n");
}


void create_semaphore (){
	
	/**
	* Crea un conjunto de semaforos. 
	*
	* @param key_t					Id del conjunto
	* @param int 					Numero de semaforos del conjunto
	* @param int 					Macros para el modo de uso ( IPC_CREAT:Crea un nuevo semaforo
    *														 IPC_EXCL: se combina con IPC_CREAT y si el semaforo ya existía, 
    *																   entonces la creación falla. )
    *
	* @return ?
	**/
	semaphore = semget (key, 7, 0777 | IPC_EXCL | IPC_CREAT);

	if ( semaphore == -1 ){
		semaphore = semget (key, 7, 0777);
	
	} else {
		
		printf("Creando el conjunto de semaforos\n");

		/**
		* Recibe informacion de la memoria compartida, establece permisos o la elimina. 
		*
		* @param int					Id del semaforo
		* @param int 					Numero de semaforo
		* @param int 					Operacion a realizar
		* @param int 					Valor de la operacion
	    *
		* @return ?
		**/			//[P,C,1,2,3,4,5]
		semctl(semaphore, 0, SETVAL, 5);
		semctl(semaphore, 1, SETVAL, 0);
		semctl(semaphore, 2, SETVAL, 1);
		semctl(semaphore, 3, SETVAL, 1);
		semctl(semaphore, 4, SETVAL, 1);
		semctl(semaphore, 5, SETVAL, 1);
		semctl(semaphore, 6, SETVAL, 1);

		printf("Inicializando los semaforos.\n");
	}
}

void delete_semaphore (){
	
	/**
	* Recibe informacion de la memoria compartida, establece permisos o la elimina. 
	*
	* @param int					Id del semaforo
	* @param int 					Numero de semaforo
	* @param int 					Operacion a realizar
    *
	* @return int 					0:EXITO, -1:ERROR
	**/

	//for(int i=0 ; i<7; i++)
	semctl(semaphore, 0, IPC_RMID);
	semctl(semaphore, 1, IPC_RMID);
	semctl(semaphore, 2, IPC_RMID);
	semctl(semaphore, 3, IPC_RMID);
	semctl(semaphore, 4, IPC_RMID);
	semctl(semaphore, 5, IPC_RMID);
	semctl(semaphore, 6, IPC_RMID);
	//delete_memory();
	printf("Eliminando los semaphores.\n");

}

const char * get_string(int i)
{
	//printf("Obtuvimos cadena %d.\n", i);
	
	if(i == 0) return "1";
	if(i == 2) return "2";
	if(i == 4) return "3";
	if(i == 6) return "4";
	if(i == 8) return "5";
	if(i == 10) return "6";
	if(i == 12) return "7";
	if(i == 14) return "8";
	if(i == 16) return "9";
	else return "10";
}

/* Codigo del productor */
void productor(int i){
	int x = 1;
	//printf("Productor %d\n",i);
	int semaphore_value;
	
	while (1){
		//obtenemos el valor del semaforo
		semaphore_value = semctl(semaphore, 0, GETVAL);
		if(semaphore_value >0){
			semctl(semaphore, 0, SETVAL, semaphore_value-1);
			//printf("Se redujo el valor del semaforo 0 en el productor %d.\n", i);

			break;
		}
	}

	while(x){
		for(int j = 0; j<5; j++){
			//sem_wait(j+2);
			semaphore_value = semctl(semaphore, (j+2), GETVAL);
			//printf("%d", semaphore_value);
			
			if(semaphore_value > 0){
				semctl(semaphore, (j+2), SETVAL, semaphore_value-1);
				//printf("Se redujo el valor del semaforo %d en el productor %d para que nadie entre al campo a analizar.\n", (j+2), i);
				if(buffer -> flag == 0){
					//Copiar al buffer el contenido que le corresponde al productor
					strcpy(buffer -> content, get_string(i));
					buffer -> flag = 1;
					//sem_signal(j+2);
					semaphore_value = semctl(semaphore, j+2, GETVAL);
					semctl(semaphore, j+2, SETVAL, semaphore_value+1);
					//printf("Se aumento el valor del semaforo %d en el productor %d.\n", j+2, i);
					x = 0;
					j = 5;
					break;
				}

				//sem_signal(j+2);
				semaphore_value = semctl(semaphore, j+2, GETVAL);
				semctl(semaphore, j+2, SETVAL, semaphore_value+1);
				//printf("Se aumento el valor del semaforo %d en el productor  %d porque no pudo entrar a un campo.\n", j+2, i);
			}
				
		}
	}
	//sem_signal(c);
	semaphore_value = semctl(semaphore, 1, GETVAL);
	semctl(semaphore, 1, SETVAL, semaphore_value+1);
	//printf("Se aumento el valor del semaforo 1 en el productor %d para que se pueda consumir.\n", i);
}


/* Codigo del consumidor */
void consumer (){
	//printf("consumer %d\n",i);
	int x = 1;
	int  semaphore_value, pipe1;
	pipe1 = open(myfifo1,O_WRONLY);

	//sem_wait(c);
	while (1){
		semaphore_value = semctl(semaphore, 1, GETVAL);
		if(semaphore_value >0){
			semctl(semaphore, 1, SETVAL, semaphore_value-1);
			//printf("Se redujo el valor del semaforo 1 en el consumer %d.\n", i);
			break;
		}
	}

	while(x){
		for(int j = 0; j<5; j++ ){
			//sem_wait((j+2));

			semaphore_value = semctl(semaphore, j+2, GETVAL);
			if(semaphore_value >0){
				semctl(semaphore, j+2, SETVAL, semaphore_value-1);
				//printf("Se redujo el valor del semaforo %d en el consumer %d.\n", j+2, i);
				if(buffer -> flag == 1){
					
					write(pipe1, &(buffer->content), sizeof(buffer->content));
					
					buffer->flag = 0;
					//sem_signal(j+2);
					semaphore_value = semctl(semaphore, j+2, GETVAL);
					semctl(semaphore, j+2, SETVAL, semaphore_value+1);
					//printf("Se aumento el valor del semaforo %d en el consumer %d.\n", j+2, i);
					x = 0;
					j = 5;
					
					break;
				}

				semctl(semaphore, j+2, SETVAL, semaphore_value+1);
				//printf("Se aumento el valor del semaforo %d en el consumer %d.\n", j+2, i);
			}
	
		}	
	}

	//sem_signal(p);
	semaphore_value = semctl(semaphore, 0, GETVAL);
	semctl(semaphore, 0, SETVAL, semaphore_value+1);
	//printf("Se aumento el valor del semaforo 0 en el consumer %d.\n", i);
	close(pipe1);					
}