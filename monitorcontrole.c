/* Monitor sensor, no arquivo sensor.c */ 
#include <math.h>
#include <pthread.h>
#include "sensor.h"

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
static pthread_cond_t alarme = PTHREAD_COND_INITIALIZER; 

static double temperatura_desejada = 0; 
static double nivel_agua = 0; 

static double limite_atual = HUGE_VAL;


/* MONITOR DA TEMPERATURA DESEJADA PELO USUARIO */
/* Chamado pela thread que le o sensor e atualizar o valor do monitor referente ao sensor lido */
 void temperatura_putH( double lido) {
	 pthread_mutex_lock( &lock); 
	 temperatura_desejada = lido; 
	 if( temperatura_desejada >= limite_atual) 
	 	pthread_cond_signal( &alarme); 
	 pthread_mutex_unlock(&lock); 
 }
 
 /* Ler o valor do monitor referente a temperatura */ 
 double temperatura_getH( void) {
	 double aux; 
	 pthread_mutex_lock( &lock); 
	 aux = temperatura_desejada; 
	 pthread_mutex_unlock( &lock); 
	 return aux;
 }
 
 /* MONITOR DO NIVEL DE AGUA DESEJADO PELO USUARIO */
/* Chamado pela thread que le o sensor e atualizar o valor do monitor referente ao sensor lido */
 void nivel_putH( double lido) {
	 pthread_mutex_lock( &lock); 
	 nivel_agua = lido; 
	 if( nivel_agua >= limite_atual) 
	 	pthread_cond_signal( &alarme); 
	 pthread_mutex_unlock(&lock); 
 }
 
 /* Ler o valor do monitor referente ao nivel de agua */ 
 double nivel_getH( void) {
	 double aux; 
	 pthread_mutex_lock( &lock); 
	 aux = nivel_agua; 
	 pthread_mutex_unlock( &lock); 
	 return aux;
 }
