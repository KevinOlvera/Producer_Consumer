#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/types.h>

typedef struct {
	int flag;
	char content[20];
} zone;

zone *buffer;
int id_memory;
int semaphore;
key_t key;

FILE *File_1;
FILE *File_2;
FILE *File_3;
FILE *File_4;
FILE *File_5;
FILE *File_6;
FILE *File_7;
FILE *File_8;
FILE *File_9;
FILE *File_10;

int create_memory ();
void delete_memory ();
void create_semaphore ();
void delete_semaphore ();

void productor(int i);
void consumer ();
const char * get_string(int);
char * myfifo1 = "/tmp/myfifo1";