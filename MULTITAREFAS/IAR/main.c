#include <stdint.h>
#include <stdio.h>
#include "multitarefas.h"
#define tamanho 5

/*
 * Prototipos das tarefas
 */
void tarefa_produtor(void);
void tarefa_consumidor(void);

int buffer[tamanho];
int produtos;

/*
 * Configuracao dos tamanhos das pilhas
 */
#define TAM_PILHA_PRODUTOR	(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_CONSUMIDOR	(TAM_MINIMO_PILHA + 24)
#define TAM_PILHA_OCIOSA	(TAM_MINIMO_PILHA + 24)

semaforo_t vazio = {0, 0};
semaforo_t cheio = {1, 0};

uint8_t count = 0;

/*
 * Declaracao das pilhas das tarefas
 */
uint32_t PILHA_TAREFA_PRODUTOR[TAM_PILHA_PRODUTOR];
uint32_t PILHA_TAREFA_CONSUMIDOR[TAM_PILHA_CONSUMIDOR];
uint32_t PILHA_TAREFA_OCIOSA[TAM_PILHA_OCIOSA];

/*
 * Funcao principal de entrada do sistema
 */
int main(void)
{
	
	/* Criacao das tarefas */
	/* Parametros: ponteiro, nome, ponteiro da pilha, tamanho da pilha, prioridade da tarefa */
	
	CriaTarefa(tarefa_produtor, "Produtor", PILHA_TAREFA_PRODUTOR, TAM_PILHA_PRODUTOR, 1);
	
	CriaTarefa(tarefa_consumidor, "Consumidor", PILHA_TAREFA_CONSUMIDOR, TAM_PILHA_CONSUMIDOR, 2);
	
	/* Cria tarefa ociosa do sistema */
	CriaTarefa(tarefa_ociosa,"Tarefa ociosa", PILHA_TAREFA_OCIOSA, TAM_PILHA_OCIOSA, 0);
	
	/* Configura marca de tempo */
	ConfiguraMarcaTempo();   
	
	/* Inicia sistema multitarefas */
	IniciaMultitarefas();
	
	/* Nunca chega aqui */
	while (1)
	{
	}
}

/* Tarefas de exemplo que usam funcoes para suspender/continuar as tarefas */
void tarefa_produtor(void)
{
        int produz;
	SemaforoAguarda(&vazio);
        
        if(buffer[tamanho] == tamanho) {
          SemaforoLibera(&cheio);
        } else {
          buffer[tamanho]= 1 + (rand()%100);
          produz = (produz+1) % tamanho;
        }  
        SemaforoLibera(&cheio);
}

void tarefa_consumidor(void)
{
        int consome;
	SemaforoAguarda(&cheio);
        
        if(produtos == 0 || produtos != tamanho) {
          SemaforoAguarda(&vazio);
        }
        consome = (consome+1)%tamanho;
        produtos--;
        buffer[tamanho-1] = -1;
        SemaforoLibera(&vazio);
}