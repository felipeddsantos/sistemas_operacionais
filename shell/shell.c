/*

Sistemas Operacionais - Shell  
Felipe Daniel Dias dos Santos - 11711ECP004
Graduação em Engenharia de Computação - Faculdade de Engenharia Elétrica - Universidade Federal de Uberlândia

*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>  

//Função tokenizer: dividir a entrada em um conjunto de tokens (separar comando de parâmetros)
int tokenizer(char* entrada, char** comm_args){

	int index;

	for(index = 0; index < 128; index++){
	
		comm_args[index] = strsep(&entrada, " ");

		if(comm_args[index] == NULL)
		
			return 0;

		if(strlen(comm_args[index]) == 0)
		
			index--;
	}

	return 1;
}  

//Função principal
int main(){

	char diretorio[1024], entrada[1024], *comm_args[128];
	char* buffer;
	char* p_entrada = entrada;
	char** p_comm_args = comm_args;
	int cd_flag = 0;

	//Loop infinito: shell recebe e executa comandos até que um erro fatal ocorra ou o comando exit seja inserido na entrada
	while(1){

		//Iniciar shell e mostrar o diretório atual
		if(getcwd(diretorio, sizeof(diretorio)) != NULL)
		
			printf("\nMyShell: %s", diretorio);
			
		else
		
			printf("\nErro ao obter diretorio atual");
 
		//Ler a entrada do usuário
		buffer = readline("\n~$ ");

		if(strlen(buffer) != 0)
		
			strcpy(p_entrada, buffer); 
			 
		else  
		
			continue;
    
		//Obter o conjunto de tokens da entrada
		if(tokenizer(p_entrada, p_comm_args))
		
			printf("\nEntrada muito longa");

		//Verificar se o comando inserido é exit e executá-lo
		if(strcmp(p_comm_args[0], "exit") == 0)
		
			return 0;
   	 
		//Verificar se o comando inserido é cd e executá-lo
		cd_flag = 0;

		if(strcmp(p_comm_args[0], "cd") == 0){
		
			if(chdir(p_comm_args[1]) != 0)
			
				printf("\nErro ao mudar de diretorio");
				
			cd_flag = 1;
		}   	 

		//Executar comando diferente de exit e cd
		if(cd_flag == 0){

			//Criar processo
			pid_t pid_comando = fork();
    
			//Erro ao executar fork
			if(pid_comando == -1){
			
				printf("\nErro ao criar processo");
				
				return -1;
			}

			//Fork executado com sucesso, processo filho criado e executando. Executar entrada
			else if(pid_comando == 0){

				//Erro ao executar o comando inserido na entrada devido inexistência do comando ou erro inesperado
				if(execvp(p_comm_args[0], p_comm_args) < 0)
				
					printf("\nErro ao executar o comando");
			}

			//Fork executado com sucesso, processo filho criado. Aguardar finalizar processo pai antes de executar entrada
			else{
			
				//Erro ao finalizar o processo pai
				if(wait(NULL) < 0){
				
					printf("\nErro inesperado");
					
					return -1;
				}
			}
		}
	}

	return 0;
}
