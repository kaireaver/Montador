#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "montador.h"

int main(int argc, char *argv[]) {
    //Abertura do arquivo
    FILE *arq;
    switch (argc) {
    case 1:
        printf("Passe um arquivo como argumento!\n");
        return 1;
    case 2:
        arq = fopen(argv[1], "rt");
        if (arq == NULL) {
            printf("Arquivo não encontrado!\n");
            return 1;
        }
    }
    
    // Passo 1 do montador, constrói a tabela de símbolos
    SymTable *head = NULL;
    head = pass_one(arq, head);
    fclose(arq);

    // Passo 2, traduz o código recebido
    arq = fopen(argv[1], "rt");
    pass_two(arq,head);
    fclose(arq);

    return 0;
}
