

// DEFINIÇÕES E IMPORTAÇÕES ===================================================
#define CELL_SIZE_ID 1+ 4
#define CELL_SIZE_NOME 1+ 50
#define CELL_SIZE_QNT 1+ 15
#define CELL_SIZE_STATUS 1+ 15
#define CELL_LIMITADOR '|'
#define CELL_PREENCHEDOR ' '
#define LINE_SIZE (CELL_SIZE_ID + CELL_SIZE_NOME + CELL_SIZE_QNT*2 + CELL_SIZE_STATUS + 6 - 5 + 1) // 6 char divisórios - 5 vezes o '\0' + 1 para o '\0' final

#define MAX_EQUIP 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int id;
	char nome[CELL_SIZE_NOME];
	int qnt_max;
	int qnt_disp;
	char status[CELL_SIZE_STATUS];
	//ID, Nome, Quantidade Máx., Quantidade Disp., Status
} Equipamento;
const char status[3][CELL_SIZE_STATUS] = {"Disponivel", "Emprestado", "Manutencao"};

// FUNÇÕES ====================================================================
void alinhaTextoE(char* destino, const char* fonte, const int tam_final, const char caractere);
void alinhaTextoC(char* destino, const char* fonte, const int tam_final, const char caractere);
void alinhaTextoD(char* destino, const char* fonte, const int tam_final, const char caractere);

void tabelaLinha(char* linha, const char* id, const char* nome, const char* qnt_max, const char* qnt_disp, const char* status, const int pos);
void tabelaTitulo();
void tabelaLinhaEqp(Equipamento equipamento);

void menu();

void carregarDados(Equipamento* equipamentos, int* n, const char* nome_arquivo);


// MAIN =======================================================================

int main(void) {
Equipamento equipamentos[MAX_EQUIP];
    int n = 0;
    //int opcao;

    carregarDados(equipamentos, &n, "equipamentos.txt");
    
	return 0;
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

// FUNÇÕES PARA TABELA ========================================================

void tabelaLinha(char* linha, const char* id, const char* nome, const char* qnt_max, const char* qnt_disp, const char* status, const int pos) {
	// A função 'tabelaLinha' permite decidir a posição onde o texto será alinhado, através do param 'pos'.
	// POS: 0 = Esq, 1 = Centro, 2 ou outro = Dir;
	void (*alinhar)(char* destino, const char* fonte, const int tam_final, const char caractere);
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
	
	sprintf(linha, "%c%s%c%s%c%s%c%s%c%s%c", 
		CELL_LIMITADOR, txt_id,
		CELL_LIMITADOR, txt_nome,
		CELL_LIMITADOR, txt_qnt_max,
		CELL_LIMITADOR, txt_qnt_disp,
		CELL_LIMITADOR, txt_status,
		CELL_LIMITADOR
	);
}

void tabelaTitulo() {
	char titulo[LINE_SIZE];
	tabelaLinha(titulo, "ID", "Nome do Equipamento", "Qnt. Max.", "Qnt. Disp.", "Situacao", 1);
	printf("%s\n", titulo);
}

void tabelaLinhaEqp(Equipamento equipamento) {
	char nova_linha[LINE_SIZE];
	char txt_id[CELL_SIZE_ID], txt_qnt_max[CELL_SIZE_QNT], txt_qnt_disp[CELL_SIZE_QNT];
	sprintf(txt_id, "%d", equipamento.id);
	sprintf(txt_qnt_max, "%d", equipamento.qnt_max);
	sprintf(txt_qnt_disp, "%d", equipamento.qnt_disp);
	
	tabelaLinha(nova_linha, txt_id, equipamento.nome, txt_qnt_max, txt_qnt_disp, equipamento.status, 2);
	printf("%s\n", nova_linha);
}

// FUNÇÕES PARA MENU ==========================================================

void menu() {
	system("cls");
	printf("\n===== MENU =====\n");
    printf("1. Cadastrar Equipamento\n");
    printf("2. Listar Equipamentos\n");
    printf("3. Emprestar Equipamento\n");
    printf("4. Devolver Equipamento\n");
    printf("5. Salvar Dados\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
}

// FUNÇÕES PARA MANIPULAÇÃO DE DADOS ==========================================

void inicializarArquivo(const char* nome_arquivo) {
	char titulo[LINE_SIZE];
	tabelaLinha(titulo, "ID", "Nome do Equipamento", "Qnt. Max.", "Qnt. Disp.", "Situacao", 1);
	
	FILE* arquivo;
	arquivo = fopen(nome_arquivo, "w");
	
	if (!arquivo) {
		printf("Erro ao inicializar arquivo.");
		exit(1);
	}
	
	fprintf(arquivo, "%s\n", titulo);
	
	fclose(arquivo);
}

void carregarDados(Equipamento* equipamentos, int* n, const char* nome_arquivo) {
	FILE* arquivo;
	arquivo = fopen(nome_arquivo, "r");
	if (!arquivo) {
		inicializarArquivo(nome_arquivo);
	}
	arquivo = fopen(nome_arquivo, "r");
	if (!arquivo) {
		printf("Erro Critico ao abrir arquivo: '%s'", nome_arquivo);
		exit(1);
	}
}