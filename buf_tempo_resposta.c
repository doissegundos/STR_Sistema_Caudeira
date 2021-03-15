/* Monitor buffer duplo, no arquivo bufduplo.c */
#include	<pthread.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>

#define TAMBUF 100 
static double buffer_0[TAMBUF]; 
static double buffer_1[TAMBUF];

static int emuso = 0; 
static int prox_insercao = 0; 
static int gravar = -1;
static int contador = 0;
static FILE* arquivo;

static pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER; 
static pthread_cond_t buffer_cheio = PTHREAD_COND_INITIALIZER;

void bufduplo_insereLeitura_tempo_resposta( double leitura) {
	pthread_mutex_lock( &exclusao_mutua); 
	if( emuso == 0) 
		buffer_0[prox_insercao] = leitura; 
	else 
		buffer_1[prox_insercao] = leitura; 
	++prox_insercao; 
	if( prox_insercao == TAMBUF){ 
		gravar = emuso; 
		emuso = (emuso + 1)% 2; 
		prox_insercao = 0; 
		pthread_cond_signal( &buffer_cheio); 
	} 
	pthread_mutex_unlock( &exclusao_mutua);
}

void adiciona_dados_buffer_tempo_resposta(float valor_lido){ 
	
    arquivo = fopen("dados_buffer_tempo_resposta.txt", "a+");
    
    //se ocorrer erro na criação ou leitura do arquivo, então o programa deve exibir mensagem de erro e ser encerrado.
    if(arquivo == NULL){
        printf("Ocorreu um erro na criação do arquivo\n");
        exit(1);    
    }
    
	//Gravando os dados dos tempos de resposta no buffer
	if(contador==0 ){
		contador++;
		fprintf(arquivo, "%f\t",valor_lido);
	}else if(contador == 1){
    	contador = 0;
		fprintf(arquivo, "%f\n",valor_lido);		
	}
		
    fclose(arquivo); //fecha o arquivo
       
}

double *bufduplo_esperaBufferCheio_tempo_resposta( void) {
	double *buffer; 
	int contador1 = 0;
	pthread_mutex_lock( &exclusao_mutua); 
	while( gravar == -1) 
		pthread_cond_wait( &buffer_cheio, &exclusao_mutua);
	if(gravar==0) 
		buffer = buffer_0; 
	else 
		buffer = buffer_1; 
	gravar = -1; 
	pthread_mutex_unlock( &exclusao_mutua); 
	
	//Adiciona os valores dos sensores no buffer
	while(contador1<TAMBUF){
		adiciona_dados_buffer_tempo_resposta(buffer[contador1]);
		contador1++;
	}	
	contador1 = 0;
	
	pthread_t buffer_tempo_resposta;
	pthread_create(&buffer_tempo_resposta, NULL, (void *) bufduplo_esperaBufferCheio_tempo_resposta, NULL);
	pthread_join( buffer_tempo_resposta, NULL); 

}


