#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EQUIP 100

typedef struct {
    int id;
    char nome[50];
    int quantidade;
    char status[20];
} Equipamento;

void menu();
void cadastrarEquipamento(Equipamento *equipamentos, int *n){}
void listarEquipamentos(Equipamento *equipamentos, int n){}
void emprestarEquipamento(Equipamento *equipamentos, int n){}
void devolverEquipamento(Equipamento *equipamentos, int n){}
void salvarDados(Equipamento *equipamentos, int n, const char *nome_arquivo){}
void carregarDados(Equipamento *equipamentos, int *n, const char *nome_arquivo){}

int main() {
    Equipamento equipamentos[MAX_EQUIP];
    int n = 0;
    int opcao;

    carregarDados(equipamentos, &n, "equipamentos.txt");

    do {
        menu();
        scanf("%d", &opcao);
        switch(opcao) {
            case 1: cadastrarEquipamento(equipamentos, &n); break;
            case 2: listarEquipamentos(equipamentos, n); break;
            case 3: emprestarEquipamento(equipamentos, n); break;
            case 4: devolverEquipamento(equipamentos, n); break;
            case 5: salvarDados(equipamentos, n, "equipamentos.txt"); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida.\n");
        }
    } while(opcao != 0);

    return 0;
}

void menu() {
	system("cls");
    printf("\n===== MENU =====\n");
    printf("1. Cadastrar Equipamento\n");
    printf("2. Listar Equipamentos\n");
    printf("3. Emprestar Equipamento\n");
    printf("4. Devolver Equipamento\n");
    printf("5. Salvar Dados\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}
