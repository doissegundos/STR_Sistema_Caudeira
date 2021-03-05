//Definição de Bibliotecas
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "socket.h"
#include "sensor.h"
#include "tela.h"

void thread_mostra_status (void){
	double t, h, ti, ta, no;
	while(1){
		t = sensor_getT();
		h = sensor_getH();
		ti = sensor_getTi();
		ta = sensor_getTa();
		no = sensor_getNo();
		aloca_tela();
		printf("\33[H\33[2J");		
		printf("---------------------------------------\n");
		printf("Temperatura (T)--> %.2lf\n", t);
		printf("Temperatura do ambiente ao redor do recipiente (Ta)--> %.2lf\n", ta);
		printf("Temperatura da agua de entrada (Ti)--> %.2lf\n", ti);
		printf("Fluxo de agua de saisa (No)--> %.2lf\n", no);
		printf("Altura (H)--> %.2lf\n", h);
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
		//		
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
    
	pthread_t t1, t2, t3;
    
    pthread_create(&t1, NULL, (void *) thread_mostra_status, NULL);
    pthread_create(&t2, NULL, (void *) thread_le_sensor, NULL);
    pthread_create(&t3, NULL, (void *) thread_alarme, NULL);
    
	pthread_join( t1, NULL);
	pthread_join( t2, NULL);
	pthread_join( t3, NULL);
	    
}

