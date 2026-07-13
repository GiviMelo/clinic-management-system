#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define patients_fields 5
#define consults_fields 2

// STRUCTS DECLARATION

// defines the struct data
// typedef struct{
// 	int dia;
// 	int mes;
// 	int ano;
// } Data;

// defines the struct consulta
typedef struct{
	//Data data;
	char data[50];
	char descricao[256];
	int id_paciente;
} Consulta;


// defines the struct paciente
typedef struct{
	int id;
	char nome[50];
	int idade;
	char telefone[20];
	char email[50];
} Paciente;


/*
	SCREEN PRINTING FUNCTIONS
*/

// print the welcome message
void exibirBoasVindas(){
	system("clear");
	printf("\n ===== SISTEMA DE CADASTRO DE PACIENTES =====\n");
	printf("Desenvolvido por Lucas Givisiez\n\n");
}

void pausar(){
	printf("\nPressione enter para continuar...");
	getchar(); getchar();
}

// printf the options menu
void exibirMenu(){
	printf("============ MENU PRINCIPAL ============");
	printf("\nEscolha uma acao:\n");
	printf("(Digite o numero da opcao para selecionar)\n");
	printf("1 - Novo paciente\n");
	printf("2 - Modificar paciente\n");
	printf("3 - Ver paciente\n");
	printf("4 - Deletar paciente\n");
	printf("5 - Adicionar consulta\n");
	printf("6 - Ver consultas\n");
	printf("0 - Sair\n");
	printf("========================================\n");
} 


// show every single patient registered
void showAllPatients(){
	char linha[50];
	int num_linha = 0;
	int countPatients = 1;
	FILE *arquivo;
	arquivo = fopen("patients_data/allPatients.txt", "r");
	if(arquivo == NULL) {
		printf("Erro ao manipular arquivo\n");
		return;
	}

	// as the patients names are stored on every odd line on 'allPatients.txt', it checks if the line read is even, if so, it prints on the screen and increment the 			number of patients registered (to show before each patient)
	printf(" ===== TODOS OS PACIENTES ===== \n");
	while(fgets(linha, 50, arquivo) != NULL){
		if(num_linha%2 != 0){
			printf("%d) %s", countPatients, linha);
			countPatients++;
		}
		num_linha++;
	}
}

/*
	AUX FUNCTIONS
*/

// like a c++ constructor, if defines 'default' values each time a new patient file is created
void defaultPaciente(Paciente *padrao){
	padrao->id = 000;
	strcpy(padrao->nome, "Nao informado");
	padrao->idade = 000;
	strcpy(padrao->telefone, "000");
	strcpy(padrao->email, "Nao informado");
}

void defaultConsulta(Consulta *padrao){
	// padrao->data.dia = 00;
	// padrao->data.mes == 00;
	// padrao->data.ano = 0000;
	strcpy(padrao->data, "00/00/0000");

	strcpy(padrao->descricao, "Nao informado\n");

	padrao->id_paciente = 000;
}

// calculates the new patient's id
// each patient have an exclusive id
int calcId(){
	int lastId, newId;
	
	//opens the file that stores the last patient's registered id
	FILE *arquivo;
	arquivo = fopen("patients_data/lastId.txt", "r");
	if(arquivo == NULL){
		printf("Erro ao manipular arquivo de gerenciamento de ids\n");
		return -1;
	}

	//store the read id on the variable 'lastId'
	fscanf(arquivo, " %d", &lastId);
	fclose(arquivo);

	//the new id is always the successor of the last id used
	newId = lastId+1;

	//rewrite the file that stores the last id used with the new one
	arquivo = fopen("patients_data/lastId.txt", "w");
	if(arquivo == NULL){
		printf("Erro ao manipular arquivo de gerenciamento de ids\n");
		return -1;
	}

	fprintf(arquivo, "%d", newId);
	fclose(arquivo);

	//returns the value of the new id
	return newId;
}


// builds the full path for a file inside the 'patients_data' folder
// filename: the name of the file ("1", "allPatients.txt")
// result: the full path that can be passed to fopen ("patients_data/1")
void buildPath(char result[100], char filename[100]){
	sprintf(result, "patients_data/%s", filename);
}

void buildConsultPath(char result[100], char filename[100]){
	sprintf(result, "consults_data/consults_%s", filename);
}

// search and store the id of the selected patient through 'allPatients.txt' file
void selectPatient(char idPaciente[50]){
	char linha[50];
	int num_linha = 0;
	int opcao = -1;

	//open the register of every single patient
	FILE *arquivo;
	arquivo = fopen("patients_data/allPatients.txt", "r");
	if(arquivo == NULL) {
		printf("Erro ao manipular arquivo\n");
		return;
	}

	//ask and store user's selection
	printf("\nInforme o numero do paciente a ser consultado: ");
	scanf(" %d", &opcao);

	//every patient's id is stored on even lines ('allPatients.txt'), so it checks if the user's selection matches with the id on that specific line
	while(fgets(linha, 50, arquivo) != NULL){
		if(2*(opcao-1) == num_linha){		//if it matches, copy the id on the line to the variable 'idPaciente'
			strcpy(idPaciente, linha);
			idPaciente[strcspn(idPaciente, "\n")] = '\0';			//replace the "\n" on the end of the string with '\0'
			break;
		}
		num_linha++;
	}
	fclose(arquivo);

}

// read and stores the data of a specific patient
// data_vector is where every information of the patient is gonna be stored (each index willl hold one field)
// dadosPaciente is the patient's file name
void readPatient(char*** data_vector, char dadosPaciente[50]){
	char buffer[50];

	//builds the path for the file inside 'patients_data'
	char path[100];
	buildPath(path, dadosPaciente);

	//open the file in read mode
	FILE* arquivo;
	arquivo = fopen(path, "r");
	if(arquivo == NULL){
		printf("Erro ao manipular arquivo");
		return;
	}


	//allocates memory for the vector that will hold all patient fields
	int i=0;
	*data_vector = (char**)malloc(patients_fields * sizeof(char*));	//allocates memory dinamically for every data type each patient has (ex: id, name, age, phone, email)
	while(fgets(buffer, 50, arquivo) != NULL){			//read every line of the patient's file
		(*data_vector)[i] = malloc(50 * sizeof(char));		// allocates memory dinamically for individual data types (50 char for id, 50 char for name...)
		strcpy((*data_vector)[i], buffer);		//copies the line to the data vector
		i++;
	}

	fclose(arquivo);
}

// makes a copy of the register cointaining every patient to a temporary file
void copyAllPatients(FILE** registro, FILE** copia){
	char buffer[50];

	*registro = fopen("patients_data/allPatients.txt", "r");
	*copia = fopen("patients_data/allPatientsTEMP.txt", "w");
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

void copyAllConsults(char path[100], FILE** registro, FILE** copia){
	char buffer[256];

	*registro = fopen(path, "r");
	*copia = fopen("consultsTEMP.txt", "w");
	if(*registro == NULL || *copia == NULL){
		printf("Erro ao manipular arquivos");
		return;
	}

	while(fgets(buffer, 256, *registro) != NULL){
		fprintf(*copia, "%s", buffer);
	}

	fclose(*registro);
	fclose(*copia);
}

void showAllConsults(char idPaciente[50]){
	char buffer[256];
	int total_linhas = 0;
	int display_linhas = 1;
	
	char path[100];
	buildConsultPath(path, idPaciente);

	FILE* arquivo;
	arquivo = fopen(path, "r");
	if(arquivo == NULL){
		printf("Erro ao manipular arquivo");
		return;
	}

	if(fgets(buffer, 256, arquivo) == NULL){
		printf("Nenhuma consulta registrada ainda\n");
		fclose(arquivo);
		return;
	}

	while(fgets(buffer, 256, arquivo) != NULL){
		if(total_linhas%2 == 0){
			printf("\n ----- CONSULTA %d ----- \n", display_linhas);
			printf("Data: 		%s\n", buffer);
			display_linhas++;
		} else {
			printf("Descricao: 	%s\n", buffer);
		}
		total_linhas++;
	}
}

void selectConsult(char idPaciente[50], int *opcao){
	char buffer[256];
	int num_linha = 0;

	showAllConsults(idPaciente);

	printf("Deseja modificar qual consulta?");
	scanf(" %d", opcao);
}

void readConsult(char*** data_vector, char idPaciente[50]){
	char buffer[256];

	char path[100];
	buildConsultPath(path, idPaciente);

	FILE* arquivo;
	arquivo = fopen(path, "r");
	if(arquivo == NULL){
		printf("Erro ao maniuplar arquivo");
		return;
	}

	int i = 0;

}

/*
	CONSULTS FUNCTIONS
*/

void novaConsulta(Consulta* c){
	system("clear");
	defaultConsulta(c);

	char idPaciente[20];

	printf("Deseja adicionar uma consulta a qual cliente?\n");
	showAllPatients();
	selectPatient(idPaciente);

	printf("Informe a data da consulta: ");
	scanf(" %s", c->data);
	printf("Descricao da consulta: ");
	scanf(" %255[^\n]", c->descricao);

	char path[100];
	buildConsultPath(path, idPaciente);

	FILE* arquivo;
	arquivo = fopen(path, "a");
	if(arquivo == NULL){
		printf("Erro ao manipular arquivo");
		return;
	}

	fprintf(arquivo, "%s\n", c->data);
	fprintf(arquivo, "%s\n", c->descricao);


	fclose(arquivo);

	printf("Consulta adicionada com sucesso\n");
	pausar();
}

void verConsultas(){
	system("clear");

	char idPaciente[50];

	printf("Deseja ver as consultas de que paciente?\n");
	showAllPatients();
	selectPatient(idPaciente);
	
	showAllConsults(idPaciente);

	pausar();
}

void modificarConsulta(){
	char idPaciente[50];
	char buffer[256];
	int total_linhas = 0;
	int consulta_atual = 1;
	int opcao_num_consulta = -1;
	int opcao_campo_a_alterar = -1;

	char nova_data[256];
	char nova_descricao[256];

	printf("Deseja modificar uma consulta relacionada a qual paciente?\n");
	showAllPatients();
	selectPatient(idPaciente);

	selectConsult(idPaciente, &opcao_num_consulta);

	printf("Qual campo deseja alterar?\n");
	printf("1) Data\n");
	printf("2) Descricao\n");
	scanf(" %d", &opcao_campo_a_alterar);
	switch(opcao_campo_a_alterar){
		case 1:
			printf("Informe a nova data: ");
			scanf(" %s", nova_data);
			break;
		case 2:
			printf("Informe a nova descricao: ");
			scanf(" %255[^\n]", nova_descricao);
			break;
		default:
			printf("Opcao invalida\n");
			break;
	}

	FILE* registro;
	FILE* copia;
	char path[100];
	buildConsultPath(path, idPaciente);

	copyAllConsults(path, &registro, &copia);

	registro = fopen(path, "w");
	copia = fopen("consultsTEMP.txt", "r");

	int linha_alvo_data = (opcao_num_consulta-1) * 2;
	int linha_alvo_descricao = linha_alvo_data + 1;

	while(fgets(buffer, 256, copia) != NULL){
		if(total_linhas == linha_alvo_data){
			fprintf(registro, "%s\n", nova_data);
		} else if (total_linhas == linha_alvo_descricao){
			fprintf(registro, "%s\n", nova_descricao);
		} else {
			fprintf(registro, "%s", buffer);
		}
		total_linhas++;
	}

	fclose(registro);
	fclose(copia);
	remove("consultsTEMP.txt");

	pausar();
}

/*
	PATIENT MANAGEMENT FUNCTIONS
*/

// creates a new patient file
void novoPaciente(Paciente *temp){
	system("clear");
	defaultPaciente(temp);	//calls the 'constructor'

	int id = calcId(); //calculates the new patient's id
	char idString[20];
	sprintf(idString, "%d", id); //transform the id(int) into a string
	temp->id = id;

	//ask the user for the patient's information
	printf("Informe o nome do paciente: ");
	scanf(" %49[^\n]", temp->nome);

	printf("Informe a idade do paciente: ");
	scanf(" %d", &temp->idade);

	printf("Informe o telefone do paciente (sem espacos ou caracteres especiais): ");
	scanf(" %19[^\n]", temp->telefone);
	
	printf("Informe o email do paciente: ");
	scanf(" %49[^\n]", temp->email);

	//builds the path for the new file
	char path[100];
	buildPath(path, idString);

	FILE *arquivo;
	FILE *registro;
	arquivo = fopen(path, "w");		//creates the new file on 'patients_data'
	if(arquivo == NULL) {
		printf("Erro ao manipular arquivo\n");
		return;
	}
	registro = fopen("patients_data/allPatients.txt", "a");		//open the general register in append mode
	if(registro == NULL){
		printf("Erro ao manipular arquivo\n");
		return;
	}
	
	//add the patient's info into his file
	fprintf(arquivo, "%d\n", id);
	fprintf(arquivo, "%s\n", temp->nome);
	fprintf(arquivo, "%d\n", temp->idade);
	fprintf(arquivo, "%s\n", temp->telefone);
	fprintf(arquivo, "%s\n", temp->email);

	//add the new patient's id and name to the general register
	fprintf(registro, "%d\n", id);
	fprintf(registro, "%s\n", temp->nome);

	fclose(arquivo);
	fclose(registro);

	char consultPath[100];
	buildConsultPath(consultPath, idString);

	FILE* consult;
	consult = fopen(consultPath, "a");
	if(consult == NULL){
		printf("Erro ao manipular arquivo");
		return;
	}
	fclose(consult);

	printf("Paciente registrado com sucesso\n");
	pausar();
}

// display a specific patient data
void verPaciente(){
	system("clear");
	
	char idPaciente[50];
	char linha[50];
	
	//display and ask the user to select a patient to see his info
	printf("Qual paciente deseja consultar?\n");
	showAllPatients();
	selectPatient(idPaciente);

	//read and store the selected patient's info on a vector
	char** data_vector;
	readPatient(&data_vector, idPaciente);

	//goes through the vector and display every patient's info
	printf("=============================\n");
	printf("	DADOS DO PACIENTE	\n");
	printf("=============================\n");
	printf("ID: 		%s\n", data_vector[0]);
	printf("Nome: 		%s\n", data_vector[1]);
	printf("Idade: 		%s\n", data_vector[2]);
	printf("Telefone: 	%s\n", data_vector[3]);
	printf("Email: 		%s\n", data_vector[4]);
	printf("=============================\n");

	pausar();
}

// modify a information of a specific patient
void modificarPaciente(){
	system("clear");

	char idPaciente[50];
	char buffer[50];
	int opcao = -1;
	Paciente temp;

	//display and ask user to select a patient to modify
	printf("\nQual paciente deseja modificar?\n");
	showAllPatients();
	selectPatient(idPaciente);

	//read and store the selected patient's info on a vector
	char** patient_data;
	readPatient(&patient_data, idPaciente);

	//transform each field into the right variable type
	sscanf(patient_data[0], "%d", &temp.id);
	sscanf(patient_data[1], "%s", temp.nome);
	sscanf(patient_data[2], "%d", &temp.idade);
	sscanf(patient_data[3], "%s", temp.telefone);
	sscanf(patient_data[4], "%s", temp.email);

	//ask the user whats gonna be changed
	printf("\nO que deseja alterar?\n");
	printf("1) Nome\n");
	printf("2) Idade\n");
	printf("3) Telefone\n");
	printf("4) Email\n");
	scanf(" %d", &opcao);
	//based on the user's input, ask what field is gonna be replaced with (the new name, the new age, etc)
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

	//if the user modifies the patient's name, rewrite it on the general register
	if(opcao == 1){

		FILE* registro;
		FILE* copia;
		
		//make a copy of the general register
		copyAllPatients(&registro, &copia);

		//open the general register in write mode and the copy in read mode
		int num_linha = 0;
		registro = fopen("patients_data/allPatients.txt", "w");
		copia = fopen("patients_data/allPatientsTEMP.txt", "r");
		if(registro == NULL || copia == NULL){
			printf("Erro ao manipular arquivos");
			return;
		}

		//checks if the read line is an even line (contains the id's)
		int flag = 0;
		while(fgets(buffer, 50, copia) != NULL){
			//if so, copies the id into a temporary buffer and compares if it matches the id of the selected patient
			if(num_linha%2 == 0){
				char temp_buffer[50];
				strcpy(temp_buffer, buffer);
				temp_buffer[strcspn(temp_buffer, "\n")] = '\0';
				//if the id matches, set the flag equals to 1
				if(strcmp(temp_buffer, idPaciente) == 0){
					flag = 1;
				}
			}

			//checks if the read line is an odd line (contains the name's) and if the flag is set to 1 (the id's matches)
			if(num_linha%2 != 0 && flag == 1){
				//if so, write the new name
				fprintf(registro, "%s\n", temp.nome);
				flag = 0;
			} else {
				//if not, just copy from the temporary file
				fprintf(registro, "%s", buffer);
			}
			num_linha++;
		}

		fclose(registro);
		fclose(copia);

		//deletes the temporary file
		if(remove("patients_data/allPatientsTEMP.txt") != 0){
			printf("Erro ao deletar arquivo temporario");
			return;
		}
	}

	//rewrite every field on the patient's file
	char path[100];
	buildPath(path, idPaciente);

	FILE* arquivo;
	arquivo = fopen(path, "w");
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

	printf("Campo alterado com sucesso\n");
	pausar();
}

// deletes a patient from the system
void deletarPaciente(){
	system("clear");

	char idPaciente[50];
	char buffer[50];
	int num_linha = 0;

	//display and ask what patient the user wants to delete
	printf("Qual paciente deseja deletar?\n ");
	showAllPatients();
	selectPatient(idPaciente);

	char confirmar;
	printf("Tem certeza que deseja deletar? (S/N)\n");
	scanf(" %c", &confirmar);
	if(confirmar != 's' && confirmar != 'S'){
		printf("Cancelado\n");
		pausar();
		return;
	}

	//build the path for the selected patient's file
	char path[100];
	buildPath(path, idPaciente);

	//delete the patient's file
	if(remove(path) != 0){
		printf("Erro ao deletar arquivo");
		return;
	} 

	//delete the selected patient from the general register
	FILE* arquivo;
	FILE* copia;
	copyAllPatients(&arquivo, &copia); 	//makes a temporary copy of the general register

	arquivo = fopen("patients_data/allPatients.txt", "w");
	copia = fopen("patients_data/allPatientsTEMP.txt", "r");
	if(arquivo == NULL || copia == NULL){
		printf("Erro ao manipular arquivos");
		return;
	}

	//read every line from the copy
	int flag = 0;
	while(fgets(buffer, 50, copia) != NULL){
		//checks if the read line is even (contains the id's) and check if the id matches the selected patient
		if(num_linha%2 == 0){
			char temp_buffer[50];
			strcpy(temp_buffer, buffer);
			temp_buffer[strcspn(temp_buffer, "\n")] = '\0';
			if(strcmp(temp_buffer, idPaciente) == 0){	//if so, set the flag to 1
				flag = 1;
			} else {		//if not, copy the line from the temporary file
				fprintf(arquivo, "%s", buffer);
			}
		} else if(num_linha%2 != 0 && flag == 1){	//if its not an even line (contains the name's) and the flag is set to 1, reset the flag
			flag = 0;
		} else {		//if it is an odd line and the flag is set to 0, just copy the line from the temporary file
			fprintf(arquivo, "%s", buffer);
		}

		num_linha++;
	}

	fclose(arquivo);
	fclose(copia);
	remove("patients_data/allPatientsTEMP.txt");

	printf("Paciente deletado com sucesso\n");
	pausar();
}



int main(){

	system("mkdir -p consults_data");
	system("mkdir -p patients_data");

	int opcao = -1;
	int continuar = -1;
	Paciente temp;
	Consulta c;

	//if the user doesnt select the option 'sair'/0 -- the program will run on loop and display the options menu
	while(opcao != 0){
		system("clear");
		exibirBoasVindas();
		exibirMenu();

		scanf("%d", &opcao);

		switch (opcao) {
			case 0:
				system("clear");
				printf("Obrigado por utilizar!\n");
				break;

			case 1:
				novoPaciente(&temp);
				break;

			case 2:
				modificarPaciente();
				break;

			case 3:
				verPaciente();
				break;

			case 4:
				deletarPaciente();
				break;

			case 5:
				novaConsulta(&c);
				break;

			case 6:
				verConsultas();
				break;

			case 7:
				modificarConsulta();
				break;

			default:
				printf("Opcao invalida\n");
				break;
		}
	}
	

	return 0;
}
