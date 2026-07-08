#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define capacity 5


// STRUCTS DECLARATION
typedef struct{

} Consulta;

typedef struct{
	int id;
	char nome[50];
	int idade;
	char telefone[20];
	char email[50];
} Cliente;


// SCREEN PRINTING FUNCTIONS
void exibirBoasVindas(){
	printf("\n ===== SISTEMA DE CADASTRO DE CLIENTES =====\n");
	printf("Desenvolvido por Lucas Givisiez\n\n");
}

void exibirMenu(){
	printf("\nEscolha uma acao:\n");
	printf("(Digite o numero da opcao para selecionar)\n");
	printf("1 - Novo cliente\n");
	printf("2 - Modificar cliente\n");
	printf("3 - Ver cliente\n");
	printf("4 - Deletar cliente\n");

	printf("0 - Sair\n");
} 

void showAllClients(){
	char linha[50];
	int num_linha = 0;
	int countClientes = 1;
	FILE *arquivo;
	arquivo = fopen("allClients.txt", "r");
	if(arquivo == NULL) {
		printf("Erro ao manipular arquivo\n");
		return;
	}

	printf(" ===== TODOS OS CLIENTES ===== \n");
	while(fgets(linha, 50, arquivo) != NULL){
		if(num_linha%2 != 0){
			printf("%d) %s", countClientes, linha);
			countClientes++;
		}
		num_linha++;
	}
}


// AUX FUNCTIONS
void defaultCliente(Cliente *padrao){
	padrao->id = 000;
	strcpy(padrao->nome, "Nao informado");
	padrao->idade = 000;
	strcpy(padrao->telefone, "000");
	strcpy(padrao->email, "Nao informado");
}

int calcId(){
	int lastId, newId;
	
	FILE *arquivo;
	arquivo = fopen("lastId.txt", "r");
	if(arquivo == NULL){
		printf("Erro ao manipular arquivo de gerenciamento de ids\n");
		return -1;
	}

	fscanf(arquivo, " %d", &lastId);
	fclose(arquivo);

	newId = lastId+1;

	arquivo = fopen("lastId.txt", "w");
	if(arquivo == NULL){
		printf("Erro ao manipular arquivo de gerenciamento de ids\n");
		return -1;
	}

	fprintf(arquivo, "%d", newId);
	fclose(arquivo);
	return newId;
}

void selectClient(char idCliente[50]){
	char linha[50];
	int num_linha = 0;
	int opcao = -1;

	FILE *arquivo;
	arquivo = fopen("allClients.txt", "r");
	if(arquivo == NULL) {
		printf("Erro ao manipular arquivo\n");
		return;
	}


	printf("\nInforme o numero do cliente a ser consultado: ");
	scanf(" %d", &opcao);


	while(fgets(linha, 50, arquivo) != NULL){
		if(2*(opcao-1) == num_linha){
			strcpy(idCliente, linha);
			idCliente[strcspn(idCliente, "\n")] = '\0';
			break;
		}
		num_linha++;
	}
	fclose(arquivo);

}

void readClient(char*** data_vector, char dadosCliente[50]){
	char buffer[50];

	FILE* arquivo;
	arquivo = fopen(dadosCliente, "r");
	if(arquivo == NULL){
		printf("Erro ao manipular arquivo");
		return;
	}

	int i=0;
	*data_vector = (char**)malloc(capacity * sizeof(char*));
	while(fgets(buffer, 50, arquivo) != NULL){
		(*data_vector)[i] = malloc(50 * sizeof(char));
		strcpy((*data_vector)[i], buffer);
		i++;
	}

	fclose(arquivo);
}

void copyAllClients(FILE** registro, FILE** copia){
	char buffer[50];

	*registro = fopen("allClients.txt", "r");
	*copia = fopen("allClientsTEMP.txt", "w");
	if(*registro == NULL || *copia == NULL){
		printf("Erro ao manipular arquivos");
		return;
	}

	while(fgets(buffer, 50, *registro) != NULL){
		fprintf(*copia, "%s", buffer);
	}
	fclose(*registro);
	fclose(*copia);
}

// MENU FUNCTIONS
void novoCliente(Cliente *temp){
	defaultCliente(temp);

	int id = calcId();
	char idString[20];
	sprintf(idString, "%d", id);

	printf("Informe o nome do cliente: ");
	scanf(" %49[^\n]", temp->nome);

	printf("Informe a idade do cliente: ");
	scanf(" %d", &temp->idade);

	printf("Informe o telefone do cliente (sem espacos ou caracteres especiais): ");
	scanf(" %19[^\n]", temp->telefone);
	
	printf("Informe o email do cliente: ");
	scanf(" %49[^\n]", temp->email);

	FILE *arquivo;
	FILE *registro;
	arquivo = fopen(idString, "w");
	if(arquivo == NULL) {
		printf("Erro ao manipular arquivo\n");
		return;
	}
	registro = fopen("allClients.txt", "a");
	if(registro == NULL){
		printf("Erro ao manipular arquivo\n");
		return;
	}
	

	fprintf(arquivo, "%d\n", id);
	fprintf(arquivo, "%s\n", temp->nome);
	fprintf(arquivo, "%d\n", temp->idade);
	fprintf(arquivo, "%s\n", temp->telefone);
	fprintf(arquivo, "%s\n", temp->email);

	fprintf(registro, "%d\n", id);
	fprintf(registro, "%s\n", temp->nome);

	fclose(arquivo);
	fclose(registro);

	printf("Cliente registrado com sucesso\n");
}

void verCliente(){
	char idCliente[50];
	char linha[50];
	int num_linha = 0;
	int opcao = -1;
	
	showAllClients();

	selectClient(idCliente);

	char** data_vector;
	readClient(&data_vector, idCliente);

	printf("ID: %s\n", data_vector[0]);
	printf("Nome: %s\n", data_vector[1]);
	printf("Idade: %s\n", data_vector[2]);
	printf("Telefone: %s\n", data_vector[3]);
	printf("Email: %s\n", data_vector[4]);

}

void modificarCliente(){
	char idCliente[50];
	char buffer[50];
	int opcao = -1;
	Cliente temp;

	printf("\nQual cliente deseja modificar?\n");
	showAllClients();

	selectClient(idCliente);

	char** client_data;
	readClient(&client_data, idCliente);

	sscanf(client_data[0], "%d", &temp.id);
	sscanf(client_data[1], "%s", temp.nome);
	sscanf(client_data[2], "%d", &temp.idade);
	sscanf(client_data[3], "%s", temp.telefone);
	sscanf(client_data[4], "%s", temp.email);

	printf("\nO que deseja alterar?\n");
	printf("1) Nome\n");
	printf("2) Idade\n");
	printf("3) Telefone\n");
	printf("4) Email\n");
	scanf(" %d", &opcao);
	switch(opcao){
		case 1:
			printf("Informe o novo nome: ");
			scanf(" %49[^\n]", temp.nome);
			break;
		case 2:
			printf("Informe a nova idade: ");
			scanf(" %d", &temp.idade);
			break;
		case 3:
			printf("Informe o novo telefone: ");
			scanf(" %49[^\n]", temp.telefone);
			break;
		case 4:
			printf("Infome o novo email: ");
			scanf(" %49[^\n]", temp.email);
			break;
		default:
			printf("Opcao invalida\n");
			break;
	}

	if(opcao == 1){

		FILE* registro;
		FILE* copia;
		
		copyAllClients(&registro, &copia);
		// registro = fopen("allClients.txt", "r");
		// copia = fopen("allClientsTEMP.txt", "w");
		// if(registro == NULL || copia == NULL){
		// 	printf("Erro ao manipular arquivos");
		// 	return;
		// }

		// while(fgets(buffer, 50, registro) != NULL){
		// 	fprintf(copia, "%s", buffer);
		// }
		// fclose(registro);
		// fclose(copia);


		int num_linha = 0;
		registro = fopen("allClients.txt", "w");
		copia = fopen("allClientsTEMP.txt", "r");
		if(registro == NULL || copia == NULL){
			printf("Erro ao manipular arquivos");
			return;
		}

		int flag = 0;
		while(fgets(buffer, 50, copia) != NULL){
			if(num_linha%2 == 0){
				char temp_buffer[50];
				strcpy(temp_buffer, buffer);
				temp_buffer[strcspn(temp_buffer, "\n")] = '\0';
				if(strcmp(temp_buffer, idCliente) == 0){
					flag = 1;
				}
			}

			if(num_linha%2 != 0 && flag == 1){
				fprintf(registro, "%s\n", temp.nome);
				flag = 0;
			} else {
				fprintf(registro, "%s", buffer);
			}
			num_linha++;
		}

		fclose(registro);
		fclose(copia);

		if(remove("allClientsTEMP.txt") != 0){
			printf("Erro ao deletar arquivo temporario");
			return;
		}
	}

	FILE* arquivo;
	arquivo = fopen(idCliente, "w");
	if(arquivo == NULL){
		printf("Erro ao manipular arquivo\n");
		return;
	}

	fprintf(arquivo, "%d\n", temp.id);
	fprintf(arquivo, "%s\n", temp.nome);
	fprintf(arquivo, "%d\n", temp.idade);
	fprintf(arquivo, "%s\n", temp.telefone);
	fprintf(arquivo, "%s\n", temp.email);
	
	fclose(arquivo);
}

void deletarCliente(){
	char idCliente[50];
	char buffer[50];
	int num_linha = 0;

	printf("Qual cliente deseja deletar: ");
	showAllClients();
	selectClient(idCliente);

	if(remove(idCliente) != 0){
		printf("Erro ao deletar arquivo");
		return;
	} 
	printf("Cliente deletado com sucesso\n");

	FILE* arquivo;
	FILE* copia;
	copyAllClients(&arquivo, &copia);

	arquivo = fopen("allClients.txt", "w");
	copia = fopen("allClientsTEMP.txt", "r");
	if(arquivo == NULL || copia == NULL){
		printf("Erro ao manipular arquivos");
		return;
	}

	int flag = 0;
	while(fgets(buffer, 50, copia) != NULL){
		if(num_linha%2 == 0){
			char temp_buffer[50];
			strcpy(temp_buffer, buffer);
			temp_buffer[strcspn(temp_buffer, "\n")] = '\0';
			if(strcmp(temp_buffer, idCliente) == 0){
				flag = 1;
			} else {
				fprintf(arquivo, "%s", buffer);
			}
		} else if(num_linha%2 != 0 && flag == 1){
			flag = 0;
		} else {
			fprintf(arquivo, "%s", buffer);
		}

		num_linha++;
	}


	fclose(arquivo);
	fclose(copia);
	remove("allClientsTEMP.txt");
}

int main(){

	FILE *arquivo;
	int opcao = -1;
	Cliente temp;
	int continuar = -1;

	exibirBoasVindas();

	while(opcao != 0){
		exibirMenu();

		scanf("%d", &opcao);

		switch (opcao) {
			case 0:
				printf("Obrigado por utilizar!\n");
				break;

			case 1:
				novoCliente(&temp);
				break;

			case 2:
				modificarCliente();
				break;

			case 3:
				verCliente();
				break;

			case 4:
				deletarCliente();
				printf("Em implementacao\n");
				break;
	
			default:
				printf("Opcao invalida\n");
				break;
		}
	}
	

	return 0;
}
