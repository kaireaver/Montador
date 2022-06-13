#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "montador.h"

int main(int argc, char *argv[]) {
    FILE *arq;
    switch (argc) {
    case 1:
        printf("Forneça um arquivo!\n");
        return 1;
    case 2:
        arq = fopen(argv[1], "rt");
        if (arq == NULL) {
            printf("Arquivo não encontrado\n");
            return 1;
        }
    }
    
    // Realiza passo 1 do montador, construindo tabela de símbolos
    SymTable *head = NULL;
    head = pass_one(arq, head);
    fclose(arq);

    // Realiza passo 2, traduzindo o código recebido
    arq = fopen(argv[1], "rt");
    pass_two(arq,head);
    fclose(arq);

    return 0;
}
