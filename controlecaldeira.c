//Definição de Bibliotecas
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "socket.h"
#include "sensor.h"
#include "tela.h"
#include "bufsensor.h"
#include "buf_tempo_resposta.h"
#include "atuadores.h"

#define NSEC_PER_SEC    (1000000000)

float temperatura_desejada = 0;
int nivel_agua = 0;


void thread_mostra_status (void){
	double t, h, ti, ta, no,na,ni,nf,q;
	while(1){
		t = sensor_getT();
		bufduplo_insereLeitura_sensores(t);
		
		h = sensor_getH();
		bufduplo_insereLeitura_sensores(h);
		
		ti = sensor_getTi();
		bufduplo_insereLeitura_sensores(ti);
		
		ta = sensor_getTa();
		bufduplo_insereLeitura_sensores(ta);
		
		no = sensor_getNo();
		bufduplo_insereLeitura_sensores(no);
		
		na = atuador_getNa();
		nf = atuador_getNf();
		ni = atuador_getNi();
		q = atuador_getQ();
		
		bufduplo_insereLeitura_tempo_resposta(10);
		bufduplo_insereLeitura_tempo_resposta(10);
		
		aloca_tela();
		printf("\33[H\33[2J");		
		printf("---------------------------------------\n");
		printf("Temperatura (T)--> %.2lf\n", t);
		printf("Temperatura do ambiente ao redor do recipiente (Ta)--> %.2lf\n", ta);
		printf("Temperatura da agua de entrada (Ti)--> %.2lf\n", ti);
		printf("Fluxo de agua de saisa (No)--> %.2lf\n", no);
		printf("Altura (H)--> %.2lf\n", h);
		printf("Fluxo de água aquecida (Na)--> %.2lf\n", na);
		printf("Fluxo de água de saida (Nf)--> %.2lf\n", nf);
		printf("Fluxo de água de entrada (Ni)--> %.2lf\n", ni);
		printf("Fluxo de calor (Q)--> %.2lf\n", q);
		printf("---------------------------------------\n");
		libera_tela();
		sleep(1);
		//					
	}	
		
}


void thread_le_sensor (void){
	char msg_enviada[1000];
	while(1){
		strcpy( msg_enviada, "st-0");
		sensor_putT(msg_socket(msg_enviada));
		
		strcpy( msg_enviada, "sh-0");
	    sensor_putH(msg_socket(msg_enviada));
	    
		strcpy( msg_enviada, "sti0");
		sensor_putTi(msg_socket(msg_enviada));
		
		strcpy( msg_enviada, "sno0");
	    sensor_putNo(msg_socket(msg_enviada));	
	    
	    strcpy( msg_enviada, "sta0");
	    sensor_putTa(msg_socket(msg_enviada));	
	    
	    //strcpy( msg_enviada, "ani20.0");
	    //atuador_putNi(msg_socket(msg_enviada));
	    
	    //strcpy( msg_enviada, "ana5.0");
	    //atuador_putNa(msg_socket(msg_enviada));
	    
	    //strcpy( msg_enviada, "anf20.0");
	    //atuador_putNf(msg_socket(msg_enviada));
		
		//strcpy( msg_enviada, "aq-20.0");
	    //atuador_putQ(msg_socket(msg_enviada));
			
		//		
		//		
	}
		
}
void controleTemperatura()
{
	struct timespec t;
	long int periodo = 50000000; 	// 50ms
	double temp;
	char msg_enviada[1000];
	
	// Le a hora atual, coloca em t
	clock_gettime(CLOCK_MONOTONIC ,&t);

	// Tarefa periodica iniciará em 1 segundo
	t.tv_sec++;

	while(1) {
		// Espera ateh inicio do proximo periodo
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		
		
    	// Realiza seu trabalho    	
    	temp = sensor_getT();
    	if (temp<temperatura_desejada+0.5){ //aumentar temp
    		printf("temperatura menor do q a escolhida pelo usuario %.2lf\n",temperatura_desejada);
    		strcpy( msg_enviada, "ana10.0");
    		atuador_putNa(msg_socket(msg_enviada));
    		
    		strcpy( msg_enviada, "ani0.0");
	    	atuador_putNi(msg_socket(msg_enviada));
	    	
	    	strcpy( msg_enviada, "aq-1000000.0");
	    	atuador_putQ(msg_socket(msg_enviada));
	    	
	    	strcpy( msg_enviada, "anf0.0");
	    	atuador_putNf(msg_socket(msg_enviada));
    		
    		
		}
		if (temp>temperatura_desejada+0.5){ //diminuir temp
    		printf("temperatura maior do q a escolhida pelo usuario %.2lf\n",temperatura_desejada);
	    	strcpy( msg_enviada, "ana0.0");
    		atuador_putNa(msg_socket(msg_enviada));
    		
    		strcpy( msg_enviada, "ani100.0");
	    	atuador_putNi(msg_socket(msg_enviada));
	    	
	    	strcpy( msg_enviada, "aq-1000000.0");
	    	atuador_putQ(msg_socket(msg_enviada));
	    	
	    	strcpy( msg_enviada, "anf100.0");
	    	atuador_putNf(msg_socket(msg_enviada));
		}
		if (temp==temperatura_desejada+0.5){ //estabilizar o sistema
    		printf("temperatura igual a escolhida pelo usuario %.2lf\n",temperatura_desejada);
	    	strcpy( msg_enviada, "ana0.0");
    		atuador_putNa(msg_socket(msg_enviada));
    		
    		strcpy( msg_enviada, "ani0.0");
	    	atuador_putNi(msg_socket(msg_enviada));
	    	
	    	strcpy( msg_enviada, "aq-0.0");
	    	atuador_putQ(msg_socket(msg_enviada));
	    	
	    	strcpy( msg_enviada, "anf0.0");
	    	atuador_putNf(msg_socket(msg_enviada));
		}
		
    	
		printf("Passou um periodo !\n");	
			
		
		
		// Calcula inicio do proximo periodo
		t.tv_nsec += periodo;
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;
			t.tv_sec++;
		}
	}
}

void controleNivelAgua()
{
	struct timespec t;
	long int periodo = 70000000; 	// 50ms
	double h;
	
	// Le a hora atual, coloca em t
	clock_gettime(CLOCK_MONOTONIC ,&t);

	// Tarefa periodica iniciará em 1 segundo
	t.tv_sec++;

	while(1) {
		// Espera ateh inicio do proximo periodo
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		
		
		
    	// Realiza seu trabalho    	
    	h = sensor_getH();
    	if (h<=nivel_agua){
    		printf("nivel de agua menor do q a escolhida pelo usuario");
		}
		if (h>nivel_agua){
    		printf("nivel de agua maior do q a escolhida pelo usuario");
		}
		
    	
		printf("Passou um periodo !\n");	
		
		
		
		
		// Calcula inicio do proximo periodo
		t.tv_nsec += periodo;
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;
			t.tv_sec++;
		}
	}
}

void verificaTemperatura()
{
	struct timespec t;
	long int periodo = 10000000; 	// 10ms
	double temp;
	
	// Le a hora atual, coloca em t
	clock_gettime(CLOCK_MONOTONIC ,&t);

	// Tarefa periodica iniciará em 1 segundo
	t.tv_sec++;

	while(1) {
		// Espera ateh inicio do proximo periodo
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		
		
    	// Realiza seu trabalho    	
    	temp = sensor_getT();
		if (temp>30){
    		printf("ALARME DISPARADO");
		}
		    		
			
		
		
		// Calcula inicio do proximo periodo
		t.tv_nsec += periodo;
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;
			t.tv_sec++;
		}
	}
}



void thread_alarme (void){
	while(1){
		//	
	}
		
}

///Controle


int main( int argc, char *argv[]) {
    
	cria_socket(argv[1], atoi(argv[2]) );
	
	printf("Digite o valor desejado da temperatura:\n");
	scanf("%f", &temperatura_desejada);
	
	printf("Digite o valor desejado do nivel maximo de agua:\n");
	scanf("%f", &nivel_agua);
    
	pthread_t t1, t2, t3;	
	pthread_t buffer_sensores;
	pthread_t buffer_tempo_resposta;
	pthread_t verifica_temperatura;
	pthread_t controle_temperatura;
	
    
    pthread_create(&t1, NULL, (void *) thread_mostra_status, NULL);
    pthread_create(&t2, NULL, (void *) thread_le_sensor, NULL);
    pthread_create(&t3, NULL, (void *) thread_alarme, NULL);
    pthread_create(&buffer_sensores, NULL, (void *) bufduplo_esperaBufferCheioSensores, NULL);
    pthread_create(&buffer_tempo_resposta, NULL, (void *) bufduplo_esperaBufferCheio_tempo_resposta, NULL);
    pthread_create(&verifica_temperatura, NULL, (void *) verificaTemperatura, NULL);
    pthread_create(&controle_temperatura, NULL, (void *) controleTemperatura, NULL);
	
    
	pthread_join( t1, NULL);
	pthread_join( t2, NULL);
	pthread_join( t3, NULL);
	pthread_join( buffer_sensores, NULL); 
	pthread_join( buffer_tempo_resposta, NULL);
	pthread_join( verifica_temperatura, NULL);
	pthread_join( controle_temperatura, NULL);
	    
}

