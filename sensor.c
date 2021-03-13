/* Monitor sensor, no arquivo sensor.c */ 
#include <math.h>
#include <pthread.h>
#include "sensor.h"
#include <stdio.h>

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
static pthread_cond_t alarme = PTHREAD_COND_INITIALIZER; 

static double sensor_lidoH = 0; 
static double sensor_lidoT = 0; 
static double sensor_lidoTi = 0; 
static double sensor_lidoNo = 0; 
static double sensor_lidoTa = 0; 

static double limite_atual = HUGE_VAL;


/* SENSOR H */
/* Chamado pela thread que le o sensor e atualizar o valor do monitor referente ao sensor lido */
 void sensor_putH( double lido) {
	 pthread_mutex_lock( &lock); 
	 sensor_lidoH = lido; 
	 if( sensor_lidoH >= limite_atual) 
	 	pthread_cond_signal( &alarme); 
	 pthread_mutex_unlock(&lock); 
 }
 
 /* Ler o valor do monitor referente ao sensor */ 
 double sensor_getH( void) {
	 double aux; 
	 pthread_mutex_lock( &lock); 
	 aux = sensor_lidoH; 
	 pthread_mutex_unlock( &lock); 
	 return aux;
 }



/* SENSOR T */
/* Chamado pela thread que le o sensor e atualizar o valor do monitor referente ao sensor lido */
 void sensor_putT( double lido) {
	 pthread_mutex_lock( &lock); 
	 sensor_lidoT = lido; 
	 if( sensor_lidoT >= limite_atual) 
	 	pthread_cond_signal( &alarme); 
	 pthread_mutex_unlock(&lock); 
 }
 
 /* Ler o valor do monitor referente ao sensor */ 
 double sensor_getT( void) {
	 double aux; 
	 pthread_mutex_lock( &lock); 
	 aux = sensor_lidoT; 
	 pthread_mutex_unlock( &lock); 
	 return aux;
 }

/* Thread fica bloqueada até o valor do sensor chegar em limite */ 
void sensor_alarmeT( double limite) {
	pthread_mutex_lock( &lock); 
	limite_atual = limite; 
	while( sensor_lidoT < limite_atual) 
		pthread_cond_wait( &alarme, &lock); 
	limite_atual = HUGE_VAL; 
	pthread_mutex_unlock( &lock); 
}

/* SENSOR Ti */
/* Chamado pela thread que le o sensor e atualizar o valor do monitor referente ao sensor lido */
 void sensor_putTi( double lido) {
	 pthread_mutex_lock( &lock); 
	 sensor_lidoTi = lido; 
	 if( sensor_lidoTi >= limite_atual) 
	 	pthread_cond_signal( &alarme); 
	 pthread_mutex_unlock(&lock); 
 }
 
 /* Ler o valor do monitor referente ao sensor */ 
 double sensor_getTi( void) {
	 double aux; 
	 pthread_mutex_lock( &lock); 
	 aux = sensor_lidoTi; 
	 pthread_mutex_unlock( &lock); 
	 return aux;
 }



/* SENSOR No */
/* Chamado pela thread que le o sensor e atualizar o valor do monitor referente ao sensor lido */
 void sensor_putNo( double lido) {
	 pthread_mutex_lock( &lock); 
	 sensor_lidoNo = lido; 
	 if( sensor_lidoNo >= limite_atual) 
	 	pthread_cond_signal( &alarme); 
	 pthread_mutex_unlock(&lock); 
 }
 
 /* Ler o valor do monitor referente ao sensor */ 
 double sensor_getNo( void) {
	 double aux; 
	 pthread_mutex_lock( &lock); 
	 aux = sensor_lidoNo; 
	 pthread_mutex_unlock( &lock); 
	 return aux;
 }

/* SENSOR Ta */
/* Chamado pela thread que le o sensor e atualizar o valor do monitor referente ao sensor lido */
 void sensor_putTa( double lido) {
	 pthread_mutex_lock( &lock); 
	 sensor_lidoTa = lido; 
	 if( sensor_lidoTa >= limite_atual) 
	 	pthread_cond_signal( &alarme); 
	 pthread_mutex_unlock(&lock); 
 }
 
 /* Ler o valor do monitor referente ao sensor */ 
 double sensor_getTa( void) {
	 double aux; 
	 pthread_mutex_lock( &lock); 
	 aux = sensor_lidoTa; 
	 pthread_mutex_unlock( &lock); 
	 return aux;
 }




