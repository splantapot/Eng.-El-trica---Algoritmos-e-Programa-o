#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM 2

typedef struct Livros{ 
char nome[100]; 
char autor[100];
char categoria[100];
int id;
} livro;

int main(void){
	livro acervo[NUM];
	int i;
	printf("Cadastro de livros\n\n");
	for(i=0;i<NUM;i++){
		acervo[i].id=i;
		printf("Digite o nome do livro %d: ",i);
		gets(acervo[i].nome);
		printf("Digite o autor do livro %d: ",i);
		gets(acervo[i].autor);
		printf("Digite a categoria do livro %d: ",i);
		gets(acervo[i].categoria);
		printf("--------------------------------------\n");
	}
	for (i=0;i<NUM;i++){
		printf("***Livro %d***\n",acervo[i].id);
		printf("- Nome: %s\n",acervo[i].nome);
		printf("- Autor: %s\n",acervo[i].autor);
	    printf("- Categoria: %s\n\n",acervo[i].categoria);
	}
	return 0;
}

