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
#include "monitorcontrole.h"

#define NSEC_PER_SEC    (1000000000)

float temperatura_desejada = 0;
float nivel_agua = 0;


void thread_mostra_status (void){
	double te, h, ti, ta, no,na,ni,nf,q,temperatura_desejada,nivel_agua;
	struct timespec t;
	long int periodo = 40e6; 	// 40ms
	double temp;
	
	// Le a hora atual, coloca em t
	clock_gettime(CLOCK_MONOTONIC ,&t);

	// Tarefa periodica iniciará em 1 segundo
	t.tv_sec++;

	while(1) {
		// Espera ateh inicio do proximo periodo
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		
		
    	te = sensor_getT();
		bufduplo_insereLeitura_sensores(te);
		
		h = sensor_getH();
		bufduplo_insereLeitura_sensores(h);
		
		ti = sensor_getTi();
		
		ta = sensor_getTa();
		
		no = sensor_getNo();
		bufduplo_insereLeitura_sensores(no);
		
		na = atuador_getNa();
		nf = atuador_getNf();
		ni = atuador_getNi();
		q = atuador_getQ();
		
		
		temperatura_desejada = temperatura_getH();
		nivel_agua = nivel_getH();
		
		aloca_tela();
		printf("\33[H\33[2J");		
		printf("---------------------------------------\n");
		printf("Temperatura escolhida pelo usuario--> %.4lf\n", temperatura_desejada);
		printf("Nivel de agua escolhido pelo usuario--> %.4lf\n", nivel_agua);
		printf("Temperatura (T)--> %.4lf\n", te);
		printf("Temperatura do ambiente ao redor do recipiente (Ta)--> %.4lf\n", ta);
		printf("Temperatura da agua de entrada (Ti)--> %.4lf\n", ti);
		printf("Fluxo de agua de saisa (No)--> %.4lf\n", no);
		printf("Altura (H)--> %.4lf\n", h);
		printf("Fluxo de água aquecida (Na)--> %.4lf\n", na);
		printf("Fluxo de água de saida (Nf)--> %.4lf\n", nf);
		printf("Fluxo de água de entrada (Ni)--> %.4lf\n", ni);
		printf("Fluxo de calor (Q)--> %.4lf\n", q);
		printf("---------------------------------------\n");
		libera_tela();
		sleep(1);
		
		// Calcula inicio do proximo periodo
		t.tv_nsec += periodo;
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;
			t.tv_sec++;
		}
	}
	
	
}


void thread_le_sensor (void){
	char msg_enviada[1000];
	
	struct timespec t;
	long int periodo = 30e6; 	// 30ms
	double temp;
	
	// Le a hora atual, coloca em t
	clock_gettime(CLOCK_MONOTONIC ,&t);

	// Tarefa periodica iniciará em 1 segundo
	t.tv_sec++;

	while(1) {
		// Espera ateh inicio do proximo periodo
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		
		
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
	    
	   	
		
		// Calcula inicio do proximo periodo
		t.tv_nsec += periodo;
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;
			t.tv_sec++;
		}
	}
			
}
void controleTemperatura()
{
	struct timespec t, t_fim;;
	long int periodo = 50e6; 	// 50ms
	double temp;
	char msg_enviada[1000];
	double temperatura_desejada = 0;
	// Le a hora atual, coloca em t
	clock_gettime(CLOCK_MONOTONIC ,&t);
	long atraso_fim;

	// Tarefa periodica iniciará em 1 segundo
	t.tv_sec++;

	while(1) {
		// Espera ateh inicio do proximo periodo
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		
		
    	// Realiza seu trabalho    	
    	temp = sensor_getT();
    	temperatura_desejada = temperatura_getH();
    	
		/*if (((temp-temperatura_desejada)<0.03 && (temp-temperatura_desejada)>0 )  || ((temperatura_desejada - temp)<0.03 && (temperatura_desejada - temp)>0 ))	
		{ //estabilizar o sistema
    		//printf("temperatura igual a escolhida pelo usuario %.2lf\n",temperatura_desejada);
	    	strcpy( msg_enviada, "ana0.0");
    		atuador_putNa(msg_socket(msg_enviada));
    		
    		strcpy( msg_enviada, "ani0.0");
	    	atuador_putNi(msg_socket(msg_enviada));
	    	
	    	strcpy( msg_enviada, "aq-0.0");
	    	atuador_putQ(msg_socket(msg_enviada));
	    	
	    	strcpy( msg_enviada, "anf0.0");
	    	atuador_putNf(msg_socket(msg_enviada));
		}*/
    	if (temp<temperatura_desejada){ //aumentar temp
    	    //printf("temperatura menor do q a escolhida pelo usuario %.2lf\n",temperatura_desejada);
    		strcpy( msg_enviada, "ana10.0");
    		atuador_putNa(msg_socket(msg_enviada));
    		
    		strcpy( msg_enviada, "ani0.0");
	    	atuador_putNi(msg_socket(msg_enviada));  
	    	
	    	strcpy( msg_enviada, "anf0.0");
	    	atuador_putNf(msg_socket(msg_enviada));
	    	

		}
		else if (temp>temperatura_desejada){ //diminuir temp
    		//printf("temperatura maior do q a escolhida pelo usuario %.2lf\n",temperatura_desejada);
	    	strcpy( msg_enviada, "ana10.0");
    		atuador_putNa(msg_socket(msg_enviada));
    		
    		strcpy( msg_enviada, "ani100.0");
	    	atuador_putNi(msg_socket(msg_enviada));
	    	
	    	strcpy( msg_enviada, "anf100.0");
	    	atuador_putNf(msg_socket(msg_enviada));
		}

		
    	
		// Le a hora atual, coloca em t_fim
		clock_gettime(CLOCK_MONOTONIC ,&t_fim);	
			
		// Calcula o tempo de resposta observado em microsegundos
		atraso_fim = 1000000*(t_fim.tv_sec - t.tv_sec)   +   (t_fim.tv_nsec - t.tv_nsec)/1000;
		bufduplo_insereLeitura_tempo_resposta(atraso_fim);
		
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
	struct timespec t, t_fim;;
	long int periodo = 70e6; 	// 70ms
	double h;
	char msg_enviada[1000];
	double nivel_agua = 0;
	long atraso_fim;
	
	
	// Le a hora atual, coloca em t
	clock_gettime(CLOCK_MONOTONIC ,&t);

	// Tarefa periodica iniciará em 1 segundo
	t.tv_sec++;

	while(1) {
		// Espera ateh inicio do proximo periodo
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		
		
		
    	// Realiza seu trabalho    	
    	h = sensor_getH();
    	nivel_agua = nivel_getH();
    	if (h<nivel_agua){
    		//printf("nivel de agua menor do q a escolhida pelo usuario");
    		strcpy( msg_enviada, "ani50.0");
	    	atuador_putNi(msg_socket(msg_enviada));
	    	strcpy( msg_enviada, "anf0.0");
	    	atuador_putNf(msg_socket(msg_enviada));

		}
		if (h>nivel_agua){
    		//printf("nivel de agua maior do q a escolhida pelo usuario");
    		strcpy( msg_enviada, "ani0.0");
	    	atuador_putNi(msg_socket(msg_enviada));
	    	strcpy( msg_enviada, "anf100.0");
	    	atuador_putNf(msg_socket(msg_enviada));
	    	strcpy( msg_enviada, "ana0.0");
	    	atuador_putNa(msg_socket(msg_enviada));
		}
		
		// Le a hora atual, coloca em t_fim
		clock_gettime(CLOCK_MONOTONIC ,&t_fim);	
		    	
		//printf("Passou um periodo !\n");	
		atraso_fim = 1000000*(t_fim.tv_sec - t.tv_sec)   +   (t_fim.tv_nsec - t.tv_nsec)/1000;
		bufduplo_insereLeitura_tempo_resposta(atraso_fim);
		
		
		
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
	long int periodo = 90e6; 	// 90ms
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
    		printf("ALARME DISPARADO\n");
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
	temperatura_putH(temperatura_desejada);
	
	printf("Digite o valor desejado do nivel maximo de agua:\n");
	scanf("%f", &nivel_agua);
	nivel_putH(nivel_agua);
    
	pthread_t t1, t2, t3;	
	pthread_t buffer_sensores;
	pthread_t buffer_tempo_resposta;
	pthread_t verifica_temperatura;
	pthread_t controle_temperatura;
	pthread_t controle_agua;
	

    pthread_create(&t1, NULL, (void *) thread_mostra_status, NULL);
    pthread_create(&t2, NULL, (void *) thread_le_sensor, NULL);
    pthread_create(&t3, NULL, (void *) thread_alarme, NULL);
    pthread_create(&buffer_sensores, NULL, (void *) bufduplo_esperaBufferCheioSensores, NULL);
    pthread_create(&buffer_tempo_resposta, NULL, (void *) bufduplo_esperaBufferCheio_tempo_resposta, NULL);
    pthread_create(&verifica_temperatura, NULL, (void *) verificaTemperatura, NULL);
    pthread_create(&controle_temperatura, NULL, (void *) controleTemperatura, NULL);
    pthread_create(&controle_agua, NULL, (void *) controleNivelAgua, NULL);
	
    
	pthread_join( t1, NULL);
	pthread_join( t2, NULL);
	pthread_join( t3, NULL);
	pthread_join( buffer_sensores, NULL); 
	pthread_join( buffer_tempo_resposta, NULL);
	pthread_join( verifica_temperatura, NULL);
	pthread_join( controle_temperatura, NULL);
	pthread_join( controle_agua, NULL);
	    
}

