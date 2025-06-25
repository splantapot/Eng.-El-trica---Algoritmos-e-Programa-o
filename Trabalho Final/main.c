/* Trabalho Final *//*
> Curso: Eng. Elétrica, 2025.1
> Disciplina: Algoritmos e Programação - Prof.: Weslley Emmanuel
> Aluno: João Victor Carvalho Rocha
*/

// CONFIGURAÇÕES ==============================================================

// NOTA: Sistema desenvolvido em Windows
// Em windows, system("cls"); Em Linux, system("clear")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EQUIP 100

#define CELL_SIZE_ID (1+4)
#define CELL_SIZE_NOME (1+50)
#define CELL_SIZE_QNT (1+15)
#define CELL_SIZE_STATUS (1+15)
#define CELL_LIMITADOR '|'
#define CELL_PREENCHEDOR ' '
#define LINE_SIZE (CELL_SIZE_ID + CELL_SIZE_NOME + CELL_SIZE_QNT*2 + CELL_SIZE_STATUS + 6)

#define DATA_HEADER '$'

#define UNDEFINED_INT -1704
#define QUIT_INT -2501
#define QUIT_STR "/c"

#define INTERACTION_SIZE 100

typedef struct {
	//ID, Nome, Quantidade Máx., Quantidade Disp., Status
	int id;
	char nome[CELL_SIZE_NOME];
	int qnt_max;
	int qnt_disp;
	int status;
} Equipamento;


const char status[3][CELL_SIZE_STATUS] = {"Disponivel", "Emprestado", "Manutencao"};

// FUNÇÕES ====================================================================

/*Para manipulação de respostas */
void fgetS(char* str, int tam);
void fgetI(int* num, int tam);
int rmvn(char* destino);

/* Exibição do menu e tabelas */
void menu();
void tabelaLinha(const char* id, const char* nome, const char* qnt_max, const char* qnt_disp, const char* status, const int pos);
void tabelaTitulo();
void tabelaLinhaEqp(Equipamento equipamento);

/* Funções para os dados */
int interromperS(const char* str);
int interromperI(const int num);
void esperarResposta();
int buscar_pos_id(int* pos, Equipamento* equipamentos, int n); //função auxiliar para empréstimos e devoluções

int cadastrarEquipamento(Equipamento *equipamentos, int *n);
int emprestarEquipamento(Equipamento *equipamentos, int n);
int devolverEquipamento(Equipamento *equipamentos, int n);
int manutencaoEquipamento(Equipamento *equipamentos, int n);

void listarEquipamentos(Equipamento *equipamentos, int n);
void salvarDados(Equipamento *equipamentos, int n, const char *nome_arquivo);
void carregarDados(Equipamento *equipamentos, int *n, const char *nome_arquivo);


// MAIN =======================================================================

int main() {
	printf("Carregando...\n");
	
	const char nome_arquivo[] = "equipamentos.txt"; 
    Equipamento equipamentos[MAX_EQUIP];
    int n = 0;
    
    int opcao;
    int realizou_operacoes = 0;
    int resposta; //variável para operacoes de escrita
    char interacao[INTERACTION_SIZE];

    carregarDados(equipamentos, &n, nome_arquivo);

    do {
        menu(interacao);
        fgetI(&opcao, 3);
        
        switch(opcao) {
            case 1: 
            	resposta = cadastrarEquipamento(equipamentos, &n);
				if (resposta == 1) { //se houve erro
					strcpy(interacao, "Cadastro cancelado.");
				} else {
					strcpy(interacao, "Cadastro realizado com sucesso!");
					realizou_operacoes = 1;
				}
				break;
			
            case 2: listarEquipamentos(equipamentos, n); break;
            
            case 3: 
				resposta = emprestarEquipamento(equipamentos, n);
				if (resposta == 1) { //se houve erro
					strcpy(interacao, "Emprestimo cancelado.");
				} else {
					strcpy(interacao, "Emprestimo realizado com sucesso!");
					realizou_operacoes = 1;
				}
				break;
			
            case 4:
				resposta = devolverEquipamento(equipamentos, n);
				if (resposta == 1) { //se houve erro
					strcpy(interacao, "Devolucao cancelada.");
				} else {
					strcpy(interacao, "Devolucao realizada com sucesso!");
					realizou_operacoes = 1;
				}
				break;
            
            case 5:
            	resposta = manutencaoEquipamento(equipamentos, n);
				if (resposta == 1) { //se houve erro
					strcpy(interacao, "Registro de Manutencao cancelado.");
				} else {
					strcpy(interacao, "Registro de Manutencao realizado com sucesso!");
					realizou_operacoes = 1;
				}
				break;
			
			case 6:
				salvarDados(equipamentos, n, nome_arquivo);
				strcpy(interacao, "Dados salvos com sucesso!");
				realizou_operacoes = 0;
				break;
			
            case 0:
            	if (realizou_operacoes) {
            		printf("\n***Tecle 0 para confirmar, ou qualquer outro numero para cancelar.\n");
					printf("*Voce possui operacoes nao salvas. Deseja realmente sair?\n");
					fgetI(&opcao, 3);
					strcpy(interacao, "*Nao deixe de salvar seus dados!");
				}
            	if (opcao == 0) {
					printf("Saindo...");
				}
				break;
				
            default:
				strcpy(interacao, "Opcao Invalida");
				break;
        }
    } while(opcao != 0);
    
    printf("\n");

    return 0;
}

// MANIPULAÇÃO DE STRINGS =====================================================

int rmvn(char* destino) {
	char temp[strlen(destino)+1];
	int i;
	
	strcpy(temp, destino);
	for (i = 0; i < sizeof(temp); i++) { //percorre temp, se encontra \n, troca para \0.
		if (temp[i] == '\n') {
			temp[i] = '\0';
			i = UNDEFINED_INT;
			break;
		}
	}
	strcpy(destino, temp);
	
	return i == UNDEFINED_INT? 1 : 0;
}

void fgetS(char* str, int tam) {
	char temp[tam];
	fgets(temp, tam, stdin);
	
	int find = rmvn(temp);
	strcpy(str, temp);
	
	// Se !find == caso nao tenha encontrado o \n da resposta dada, a palavra inserida foi maior que 'tam'
	if (!find) { // limpa o buffer
		char dump = getchar();
		while (dump != '\n' && dump != EOF) {
			dump = getchar();
		}
	}
}

void fgetI(int* num, int tam) {
	char temp[tam];
	int num_sf, num_atoi;
	fgetS(temp, tam);
	
	if (!strcmp(temp, QUIT_STR)) { //caso o texto digitado seja um '/q', numero deve ser QUIT_STR
		*num = QUIT_INT;
		
	} else {
		sscanf(temp, "%d", &num_sf);
		num_atoi = atoi(temp);
		
		*num = num_atoi;
		if (num_atoi == 0 && num_sf != num_atoi) { //testa se as duas conversões não convergem
			*num = UNDEFINED_INT;
		}
	}
}

// MENU E LISTAS ==============================================================

void menu(char* interacao) {
	system("cls");
	if (interacao) {
		printf("%s\n", interacao);
	}

	strcpy(interacao, "");
	
    printf("\n===== MENU =====\n");
    printf("1. Cadastrar Equipamento\n");
    printf("2. Listar Equipamentos\n");
    printf("3. Emprestar Equipamentos\n");
    printf("4. Devolver Equipamentos\n");
    printf("5. Realizar a Manutencao de Equipamentos\n");
    printf("6. Salvar Dados\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

// FUNÇÕES DA APLICAÇÃO =======================================================

int interromperS(const char* str) {
	return !strcmp(str, QUIT_STR);
}
int interromperI(const int num) {
	return num == QUIT_INT;
}

void esperarResposta() {
	int num;
	printf("\n[ insira qualquer valor para continuar ]\n");
	fgetI(&num, 1);
}

int buscar_pos_id(int* pos, Equipamento* equipamentos, int n) {
	int i;
	int id;
	*pos = UNDEFINED_INT;
	
	while (*pos == UNDEFINED_INT) { //o valor de entrada precisa ser int
		printf("> ID do equipamento (int): ");
		fgetI(&id, CELL_SIZE_ID);
		if (interromperI(id)) return 1;
		for (i = 0; i < n; i++) {
			if (id == equipamentos[i].id) {
				*pos = i;
				break;
			}
		}
		if (*pos == UNDEFINED_INT) {
			printf("*ID nao encontrado.\n");
		}
	}
	
	return 0;
}

int cadastrarEquipamento(Equipamento *equipamentos, int *n){
	
	void limpar() {
		system("cls");
		printf("[tecle '/c' para cancelar a operacao]\n");
		printf("\n=== CADASTRO ===\n");
	}
	
	int err = 0;
	int i, id = UNDEFINED_INT;
	while (id == UNDEFINED_INT) { //o valor de entrada precisa ser int
		limpar();
		printf("Informe os seguintes dados:\n");
		if (err == 1) printf("***Este ID ja foi registrado.\n");
		printf("> ID (int): ");
		fgetI(&id, CELL_SIZE_ID);
		for (i = 0; i < *n; i++) {
			if (id == equipamentos[i].id) {
				id = UNDEFINED_INT;
				err = 1;
				break;
			}
		}
	}
	if ( interromperI(id) ) return 1; //cancelar
	equipamentos[*n].id = id;
	
	printf("> Nome do Equipamento: ");
	fgetS(equipamentos[*n].nome, CELL_SIZE_NOME);
	if ( interromperS(equipamentos[*n].nome) ) return 1; //cancelar
	
	err = 0;	
		
	equipamentos[*n].qnt_max = UNDEFINED_INT;
	while (equipamentos[*n].qnt_max == UNDEFINED_INT || equipamentos[*n].qnt_max < 1) { //o valor de entrada precisa ser int
		limpar();
		printf("> ID: %d\n", id);
		printf("> Nome do Equipamento: %s\n", equipamentos[*n].nome);
		if (err == 1) printf("*** A quantidade precisa ser maior que 0.\n");
		printf("> Quantidade Total: ");
		fgetI(&equipamentos[*n].qnt_max, CELL_SIZE_ID);
		if ( interromperI(equipamentos[*n].qnt_max) ) return 1; //cancelar
		if (equipamentos[*n].qnt_max < 1) {
			err = 1;
		}
	}
	
	equipamentos[*n].status = UNDEFINED_INT;
	
	while (equipamentos[*n].status == UNDEFINED_INT || equipamentos[*n].status < 0 || equipamentos[*n].status > 2) {
		limpar();
		printf("> ID: %d\n", id);
		printf("> Nome do Equipamento: %s\n", equipamentos[*n].nome);
		printf("> Quantidade Total: %d\n", equipamentos[*n].qnt_max);
		printf("***Responda com um numero [0 = %s, 1 = %s, 2 = %s]\n", status[0], status[1], status[2]);
		printf("> Situacao: ");
		fgetI(&equipamentos[*n].status, CELL_SIZE_ID);
		if ( interromperI(equipamentos[*n].status) ) return 1; //cancelar
	}
	
	switch(equipamentos[*n].status) {
		case 0: equipamentos[*n].qnt_disp = equipamentos[*n].qnt_max; break;
		default: equipamentos[*n].qnt_disp = 0; break;
	}
	
	*n += 1; //Novo equipamento adicionado
	return 0;
}

void listarEquipamentos(Equipamento *equipamentos, int n){
	system("cls");
	tabelaTitulo();
	int i;
	for (i = 0; i < n; i++) {
		tabelaLinhaEqp(equipamentos[i]);
	}
	
	esperarResposta();
}

int emprestarEquipamento(Equipamento *equipamentos, int n){
	
	void limpar() {
		system("cls");
		printf("[tecle '/c' para cancelar a operacao]\n");
		printf("\n== EMPRESTIMO ==\n");
	}
	
	limpar();
	printf("Informe os seguintes dados:\n");
	int pos;
	if (buscar_pos_id(&pos, equipamentos, n)) return 1; //cancela
	
	if (equipamentos[pos].qnt_disp == 0) {
		limpar();
		printf("> Equipamento '%s' indisponivel para emprestimo.\n", equipamentos[pos].nome);
		esperarResposta();
		return 1;
		
	} else {
		int i = 0;
		int qnt = UNDEFINED_INT;
		const int qnt_disp = equipamentos[pos].qnt_disp;
		while (qnt == UNDEFINED_INT) {
			limpar();
			if (i) printf("*Insira um valor valido.\n");
			printf("> Quantas unidades de '%s' deseja pegar? (Max: %d)\n> ", equipamentos[pos].nome, qnt_disp);
			fgetI(&qnt, 5);
			if (interromperI(qnt)) return 1; //cancela
			if (qnt > qnt_disp || qnt < 1) {
				qnt = UNDEFINED_INT;
				i = 1;
			}
		}
		equipamentos[pos].qnt_disp -= qnt;
		if (equipamentos[pos].qnt_disp == 0) {
			equipamentos[pos].status = 1;
		}
	}
	
	return 0;
}

int devolverEquipamento(Equipamento *equipamentos, int n) {
	void limpar() {
		system("cls");
		printf("[tecle '/c' para cancelar a operacao]\n");
		printf("\n=== DEVOLUCAO ==\n");
	}
	
	limpar();
	printf("Informe os seguintes dados:\n");
	int pos;
	if (buscar_pos_id(&pos, equipamentos, n)) return 1; //cancela

	if (equipamentos[pos].status == 2) { //equipamento sob manutencao
		int x;
		limpar();
		printf("\n***Tecle 0 para confirmar, ou qualquer outro numero para cancelar.\n");
		printf("*Equipamento '%s' esta sob manutencao. Deseja confirmar a devolucao?\n", equipamentos[pos].nome);
		printf("Resposta: ");
		fgetI(&x, 3);
		
		if (x != 0) {
			return 1;
		}
		
		equipamentos[pos].qnt_disp = equipamentos[pos].qnt_max;
		equipamentos[pos].status = 0;
		return 0;
	}
	
	if (equipamentos[pos].qnt_disp == equipamentos[pos].qnt_max) { //eqp indisponivel para devolucao
		limpar();
		printf("> Equipamento '%s' indisponivel para devolucao. (todos os equipamentos estao disponiveis)\n", equipamentos[pos].nome);
		esperarResposta();
		
		return 1;
	}
	
	int i = 0;
	int qnt = UNDEFINED_INT;
	const int qnt_max = equipamentos[pos].qnt_max;
	const int qnt_disp = equipamentos[pos].qnt_disp;
	while (qnt == UNDEFINED_INT) {
		limpar();
		if (i) printf("*Insira um valor valido.\n");
		printf("> Quantas unidades de '%s' esta devolvendo? (Max: %d)\n> ", equipamentos[pos].nome, qnt_max-qnt_disp);
		fgetI(&qnt, 5);
		if (interromperI(qnt)) return 1; //cancela
		if (qnt < 1 || qnt > qnt_max - qnt_disp) {
			qnt = UNDEFINED_INT;
			i = 1;
		}
	}
	
	equipamentos[pos].qnt_disp += qnt;
	equipamentos[pos].status = 0;
	
	return 0;	
}

int manutencaoEquipamento(Equipamento *equipamentos, int n) {
	
	void limpar() {
		system("cls");
		printf("[tecle '/c' para cancelar a operacao]\n");
		printf("\n== MANUTENCAO ==\n");
	}
	
	limpar();
	printf("Informe os seguintes dados:\n");
	int pos;
	if (buscar_pos_id(&pos, equipamentos, n)) return 1; //cancela
	
	if (equipamentos[pos].qnt_disp < equipamentos[pos].qnt_max) {
		limpar();
		printf("> Equipamento '%s' indisponivel para manutencao.\n", equipamentos[pos].nome);
		printf("*A manutencao so pode ser realizada se todas as unidades do equipamento estiverem disponiveis.\n");
		printf("*Nome do equipamento: '%s'.\n", equipamentos[pos].nome);
		printf("*Quantidade emprestada: '%d'.\n", equipamentos[pos].qnt_max - equipamentos[pos].qnt_disp);
		esperarResposta();
		return 1;
		
	} else {	
		equipamentos[pos].qnt_disp = 0;
		equipamentos[pos].status = 2;
	}
	
	return 0;
}

void salvarDados(Equipamento *equipamentos, int n, const char *nome_arquivo){
	FILE *arquivo;
	arquivo = fopen(nome_arquivo, "w");
	if (!arquivo) {
		printf("[ERRO] Houve um erro ao salvar os dados.\n");
		exit(1);
	}
	
	fprintf(arquivo, "%c\n", DATA_HEADER);
	
	int i;
	for (i = 0; i < n; i++) {
		fprintf(arquivo, "%d\n", equipamentos[i].id);
		fprintf(arquivo, "%s\n", equipamentos[i].nome);
		fprintf(arquivo, "%d\n", equipamentos[i].qnt_max);
		fprintf(arquivo, "%d\n", equipamentos[i].qnt_disp);
		fprintf(arquivo, "%d\n", equipamentos[i].status);
		fprintf(arquivo, "%c\n", DATA_HEADER);
	}
	
	fclose(arquivo);
}

void carregarDados(Equipamento *equipamentos, int *n, const char *nome_arquivo) {
	//'n' é, nesse caso, o número de equipamentos lidos com sucesso.
	const char header[] = {DATA_HEADER, '\n', '\0'};
	
	void inicializar() { //função para inicializar um novo arquivo para os dados
		FILE* arquivo;
		arquivo = fopen(nome_arquivo, "w");
		if (!arquivo) {
			printf("[ERRO] Falha ao inicializar o arquivo de dados.\n");
			exit(1);
		}
		
		fputs(header, arquivo);
		fclose(arquivo);
	}
	
	FILE* arquivo;
	arquivo = fopen(nome_arquivo, "r");
	if (!arquivo) {
		inicializar(); //inicializa caso o arquivo não exista
		arquivo = fopen(nome_arquivo, "r");
	}
	
	char str[CELL_SIZE_NOME+1]; //Tamanho capaz de ler até o '/n' da maior palavra possível (um nome)
	int num; //str = buffer para o texto, num = buffer para o número
	int can_read = 0, i = 0;
	while (fgets(str, sizeof(str), arquivo) != NULL) {
		if (i >= 5) {
			can_read = i = 0;
		}
		
		if (can_read) {
			switch (i) {
				case 0:
					sscanf(str, "%d", &num);
					equipamentos[*n].id = num;
					//printf("id #%d (%d)\n", *n, num);
					break;
				
				case 1:
					rmvn(str);
					strcpy(equipamentos[*n].nome, str);
					//printf("N#%d: %s.\n", *n, equipamentos[*n].nome);
					
					break;
				
				case 2: 
					sscanf(str, "%d", &num);
					equipamentos[*n].qnt_max = num;
					//printf("qnt_max #%d (%d)\n", *n, num);
					break;
				case 3:
					sscanf(str, "%d", &num);
					equipamentos[*n].qnt_disp = num;
					//printf("qnt_disp #%d (%d)\n", *n, num);
					break;
				case 4:
					sscanf(str, "%d", &num);
					equipamentos[*n].status = num;
					//printf("status #%d (%d)\n", *n, num);
					*n += 1; //adicionar status == adicionou mais um eqp
					break;
			}
			i += 1;
		}
		
		
		if (!strcmp(str, header)) { //verifica se é o início de um registro
			can_read = 1;
		}
	}
	
	if (!feof(arquivo)) {
		printf("[ERRO] Houve um erro ao ler os dados do arquivo.");
		exit(1);
	}
	
	//printf("END\n");
	//printf("n == %d\n", *n);
}

// FUNÇÕES PARA TEXTO =========================================================

void alinharTextoE(char* destino, const char* fonte, const int tam_final, const char caractere) {
	// OBS: 'destino' deve conter o espaço para o \0. 'tam_final' deve incluir o \0.
	int tam_inicial = strlen(fonte);
	char novo_texto[tam_final];
	
	int i;
	for (i = 0; i < tam_inicial; i++) {
		novo_texto[i] = fonte[i];
	}
	for (; i < tam_final-1; i++) {
		// Reserva o último caractere para o '\0'
		novo_texto[i] = caractere;
	}
	
	novo_texto[i] = '\0';
	strcpy(destino, novo_texto);
}

void alinharTextoC(char* destino, const char* fonte, const int tam_final, const char caractere) {
	// OBS: 'destino' deve conter o espaço para o \0. 'tam_final' deve incluir o \0.
	int tam_inicial = strlen(fonte);
	int tam_pretxt = (tam_final-1-tam_inicial)/2;
	char novo_texto[tam_final];
	
	int i, j;
	for (i = 0; i < tam_pretxt; i++) {
		novo_texto[i] = caractere;
	}
	for (j = 0; j < tam_inicial; j++, i++) {
		novo_texto[i] = fonte[j];
	}
	for (; i<tam_final-1; i++) {
		// Reserva o último caractere para o '\0'
		novo_texto[i] = caractere;
	}
	
	novo_texto[i] = '\0';
	strcpy(destino, novo_texto);
}

void alinharTextoD(char* destino, const char* fonte, const int tam_final, const char caractere) {
	// OBS: 'destino' deve conter o espaço para o \0. 'tam_final' deve incluir o \0.
	int tam_inicial = strlen(fonte);
	char novo_texto[tam_final];
	
	int i, j;
	for (i = 0; i < tam_final-tam_inicial-1; i++) {
		novo_texto[i] = caractere;
	}
	for (j = 0; j < tam_inicial; j++, i++) {
		// Reserva o último caractere para o '\0'
		novo_texto[i] = fonte[j];
	}
	
	novo_texto[i] = '\0';
	strcpy(destino, novo_texto);
}

void tabelaLinha(const char* id, const char* nome, const char* qnt_max, const char* qnt_disp, const char* status, const int pos) {
	// A função 'tabelaLinha' permite decidir a posição onde o texto será alinhado, através do param 'pos'.
	// POS: 0 = Esq, 1 = Centro, 2 ou outro = Dir;
	void (*alinhar) (char* destino, const char* fonte, const int tam_final, const char caractere);
	switch(pos) {
		case 0:
			alinhar = alinharTextoE;
			break;
		
		case 1:
			alinhar = alinharTextoC;
			break;
		
		default:
			alinhar = alinharTextoD;
			break;
	}
	
	
	char txt_id[CELL_SIZE_ID];
	char txt_nome[CELL_SIZE_NOME];
	char txt_qnt_max[CELL_SIZE_QNT];
	char txt_qnt_disp[CELL_SIZE_QNT];
	char txt_status[CELL_SIZE_STATUS];
	
	alinhar(txt_id, id, CELL_SIZE_ID, CELL_PREENCHEDOR);
	alinhar(txt_nome, nome, CELL_SIZE_NOME, CELL_PREENCHEDOR);
	alinhar(txt_qnt_max, qnt_max, CELL_SIZE_QNT, CELL_PREENCHEDOR);
	alinhar(txt_qnt_disp, qnt_disp, CELL_SIZE_QNT, CELL_PREENCHEDOR);
	alinhar(txt_status, status, CELL_SIZE_STATUS, CELL_PREENCHEDOR);
	
	printf("%c%s%c%s%c%s%c%s%c%s%c\n", 
		CELL_LIMITADOR, txt_id,
		CELL_LIMITADOR, txt_nome,
		CELL_LIMITADOR, txt_qnt_max,
		CELL_LIMITADOR, txt_qnt_disp,
		CELL_LIMITADOR, txt_status,
		CELL_LIMITADOR
	);
}

void tabelaTitulo() {
	tabelaLinha("ID", "Nome do Equipamento", "Qnt. Max.", "Qnt. Disp.", "Situacao", 1);
}

void tabelaLinhaEqp(Equipamento equipamento) {
	char txt_id[CELL_SIZE_ID], txt_qnt_max[CELL_SIZE_QNT], txt_qnt_disp[CELL_SIZE_QNT];
	sprintf(txt_id, "%d", equipamento.id);
	sprintf(txt_qnt_max, "%d", equipamento.qnt_max);
	sprintf(txt_qnt_disp, "%d", equipamento.qnt_disp);
	
	tabelaLinha(txt_id, equipamento.nome, txt_qnt_max, txt_qnt_disp, status[equipamento.status], 2);
}