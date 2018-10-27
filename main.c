#include "Practica3.c"

int main(int argc, char const *argv[]){
	int pipe1, inserciones;
	int *espera;
	
	mkfifo(myfifo1, 0666);

	buffer = malloc(sizeof(zone));
	buffer->flag = 0;
	if ( create_memory() != -1 ){
		create_semaphore();
	} 
	else {
		printf("ERROR al crear la llave IPC.\n");
		exit(-1);
	}

	printf("Introduce el numero de inserciones de cada productor: ");
	scanf("%d",&inserciones);
	
	//CREANDO HIJOS
	for( int j = 0 ; j < inserciones ; j++ ){	
		for ( int i = 0 ; i < 20 ; i++ ){
			switch (fork()){
				case -1:
					printf("ERROR al crear procesos.\n");
					exit(-1);
				break;

				case 0:
					if ( i%2 == 0 ){
						productor(i);
					}else{
						consumer(i);
					}
					exit(0);
				break;
				
				default:
					if ( i%2 != 0 ){

						//Inicializar el pipe
						pipe1 = open(myfifo1, O_RDONLY);
						read(pipe1, &(buffer->content), sizeof(buffer->content));

						switch(i){
							case 1:
								File_1 = fopen ( "File_1.txt", "a" );
								fputs((buffer->content), File_1);
								fputc('\n', File_1);
								fclose(File_1);
							break;
							case 3:
								File_2 = fopen ( "File_2.txt", "a" );
								fputs((buffer->content), File_2);
								fputc('\n', File_2);
								fclose(File_2);
							break;
							case 5:
								File_3 = fopen ( "File_3.txt", "a" );
								fputs((buffer->content), File_3);
								fputc('\n', File_3);
								fclose(File_3);
							break;
							case 7:
								File_4 = fopen ( "File_4.txt", "a" );
								fputs((buffer->content), File_4);
								fputc('\n', File_4);
								fclose(File_4);
							break;
							case 9:
								File_5 = fopen ( "File_5.txt", "a" );
								fputs((buffer->content), File_5);
								fputc('\n', File_5);
								fclose(File_5);
							break;
							case 11:
								File_6 = fopen ( "File_6.txt", "a" );
								fputs((buffer->content), File_6);
								fputc('\n', File_6);
								fclose(File_6);
							break;
							case 13:
								File_7 = fopen ( "File_7.txt", "a" );
								fputs((buffer->content), File_7);
								fputc('\n', File_7);
								fclose(File_7);
							break;
							case 15:
								File_8 = fopen ( "File_8.txt", "a" );
								fputs((buffer->content), File_8);
								fputc('\n', File_8);
								fclose(File_8);
							break;
							case 17:
								File_9 = fopen ( "File_9.txt", "a" );
								fputs((buffer->content), File_9);
								fputc('\n', File_9);
								fclose(File_9);
							break;
							case 19:
								File_10 = fopen ( "File_10.txt", "a" );
								fputs((buffer->content), File_10);
								fputc('\n', File_10);
								fclose(File_10);
							break;
						}

						close (pipe1);
					}

				break;
			}
		}

		for (int i = 0; i < 20; i++) wait(espera);
	}

	delete_semaphore();
	delete_memory();
	return 0;
}