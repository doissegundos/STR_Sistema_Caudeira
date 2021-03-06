/* Monitor sensor, no arquivo sensor.c */ 
#include <math.h>
#include <pthread.h>
#include "atuadores.h"

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
static pthread_cond_t alarme = PTHREAD_COND_INITIALIZER; 

static double atuador_Ni = 0; 
static double atuador_Q = 0;
static double atuador_Na = 0; 
static double atuador_Nf = 0;   

static double limite_atual = HUGE_VAL;

/* SENSOR Ni */
/* Chamado pela thread que le o sensor e atualizar o valor do monitor referente ao sensor lido */
 void atuador_putNi( double lido) {
	 pthread_mutex_lock( &lock); 
	 atuador_Ni = lido; 
	 if( atuador_Ni >= limite_atual) 
	 	pthread_cond_signal( &alarme); 
	 pthread_mutex_unlock(&lock); 
 }
 
 /* Ler o valor do monitor referente ao sensor */ 
 double atuador_getNi( void) {
	 double aux; 
	 pthread_mutex_lock( &lock); 
	 aux = atuador_Ni; 
	 pthread_mutex_unlock( &lock); 
	 return aux;
 }
 
  void atuador_putNa( double lido) {
	 pthread_mutex_lock( &lock); 
	 atuador_Na = lido; 
	 if( atuador_Na >= limite_atual) 
	 	pthread_cond_signal( &alarme); 
	 pthread_mutex_unlock(&lock); 
 }
 
 /* Ler o valor do monitor referente ao sensor */ 
 double atuador_getNa( void) {
	 double aux; 
	 pthread_mutex_lock( &lock); 
	 aux = atuador_Na; 
	 pthread_mutex_unlock( &lock); 
	 return aux;
 }
 
  void atuador_putNf( double lido) {
	 pthread_mutex_lock( &lock); 
	 atuador_Nf = lido; 
	 if( atuador_Nf >= limite_atual) 
	 	pthread_cond_signal( &alarme); 
	 pthread_mutex_unlock(&lock); 
 }
 
 /* Ler o valor do monitor referente ao sensor */ 
 double atuador_getNf( void) {
	 double aux; 
	 pthread_mutex_lock( &lock); 
	 aux = atuador_Nf; 
	 pthread_mutex_unlock( &lock); 
	 return aux;
 }
 
  void atuador_putQ( double lido) {
	 pthread_mutex_lock( &lock); 
	 atuador_Q = lido; 
	 if( atuador_Q >= limite_atual) 
	 	pthread_cond_signal( &alarme); 
	 pthread_mutex_unlock(&lock); 
 }
 
 /* Ler o valor do monitor referente ao sensor */ 
 double atuador_getQ( void) {
	 double aux; 
	 pthread_mutex_lock( &lock); 
	 aux = atuador_Q; 
	 pthread_mutex_unlock( &lock); 
	 return aux;
 }
