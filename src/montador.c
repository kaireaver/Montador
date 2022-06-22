#include "montador.h"
#include <string.h>

int retorna_instrucao(char str[]) { //Retorna o opcode da instrução fornecida
    if (strcmp(str, "LOAD") == 0) {
        return 1;
    } else if (strcmp(str, "STORE") == 0) {
        return 2;
    } else if (strcmp(str, "ADD") == 0) {
        return 3;
    } else if (strcmp(str, "SUB") == 0) {
        return 4;
    } else if (strcmp(str, "JMP") == 0) {
        return 5;
    } else if (strcmp(str, "JPG") == 0) {
        return 6;
    } else if (strcmp(str, "JPL") == 0) {
        return 7;
    } else if (strcmp(str, "JPE") == 0) {
        return 8;
    } else if (strcmp(str, "JPNE") == 0) {
        return 9;
    } else if (strcmp(str, "PUSH") == 0) {
        return 10;
    } else if (strcmp(str, "POP") == 0) {
        return 11;
    } else if (strcmp(str, "READ") == 0) {
        return 12;
    } else if (strcmp(str, "WRITE") == 0) {
        return 13;
    } else if (strcmp(str, "CALL") == 0) {
        return 14;
    } else if (strcmp(str, "RET") == 0) {
        return 15;
    } else if (strcmp(str, "HALT") == 0) {
        return 16;
    } else if (strcmp(str, "WORD") == 0) {
        return 17;
    } else if (strcmp(str, "END") == 0) {
        return 18;
    } else {
        return -1;
    }
}

// FUNÇÕES CONSTRUTORAS DA TABELA
// Retorna endereço inteiro de um dado símbolo
int get_address(char *symbol, SymTable *head){
    SymTable *aux = head;
    int addr;
    while(aux != NULL)
    {
        if(strcmp(symbol,aux->symbol) == 0)
        {
            addr = aux->mem_addr;
            break;
        }
        aux = aux->next;
    }
    return addr;
}

// Retorna 1 se tem o símbolo e 0 caso contrário.
int has_symbol(char symbol[], SymTable *head){
    SymTable *aux = head;
    while (aux != NULL){
        if(strcmp(symbol,aux->symbol) == 0) return 1;
        aux = aux->next;
    }
    return 0;
}

// Adiciona símbolo se ele não estiver na tabela.
SymTable * add_symbol(char symbol[], SymTable *head){
    if(has_symbol(symbol, head) == 0){
        SymTable *aux = head;
        SymTable *new;
        while (aux != NULL && aux->next != NULL) {
            aux = aux->next;
        }
        new = (SymTable *) malloc(sizeof(SymTable));
        strcpy(new->symbol, symbol);
        new->mem_addr = -1;
        new->next= NULL;
        if (aux != NULL)
            aux->next = new;
        else
            head = new;
    }
    return head;
}

// Adiciona endereço a um símbolo na tabela. Se não acha, cria novo símbolo
SymTable * add_address(char symbol[], int addr, SymTable *head){
    SymTable *aux = head;
    SymTable *new;
    if(has_symbol(symbol, head) == 1){
        while(aux != NULL){
            if(strcmp(symbol,aux->symbol) == 0){
                aux->mem_addr = addr;
            }
            aux = aux->next;
        }
    }
    else {
        new = (SymTable *) malloc(sizeof(SymTable));
        while (aux != NULL && aux->next != NULL) {
            aux = aux->next;
        }
        strcpy(new->symbol, symbol);
        new->mem_addr = addr;
        new->next= NULL;
        if (aux != NULL)
            aux->next = new;
        else
            head = new;
    }
    return head;
}

// Passo 1 - Construção da tabela.
SymTable * pass_one(FILE *arq, SymTable *head){
    char line[100];
    char *word;
    int size, prog_size = 0;
    /*
    Lê linha por linha e a separa em tokens com "strtok()"
    Caso encontre um label ou uma palavra não declarada pela MV,
    adiciona à tabela de símbolos.
    */
    while(fgets(line, 100, arq) != NULL){
        line[strcspn(line, "\r\n")] = 0;
        word = strtok(line, " ");
        if (word != NULL && strncmp(word, ";", 1) != 0 && strncmp(word, "\n", 1) != 0){
            // Se termina com ":", label encontrado.
            size = strlen(word);
            if (strncmp(&word[size-1], ":", 1) == 0){
                word[size-1] = '\0';
                head = add_address(word, prog_size, head);
                word = strtok(NULL, " ");
            }
            int i = retorna_instrucao(word);
            //Se leu instrução sem ser HALT, RET ou WORD
            if (i >= 1 && i <= 14){
                word = strtok(NULL, " ");
                prog_size += 2;
                head = add_symbol(word, head);
            }
            // HALT, RET ou WORD (aumenta prog_size em 1)
            else if (i == 15 || i == 16 || i == 17){
                prog_size ++;
            }
            // Se é END, sai do loop
            else if (i == 18){
                break;
            }
        }
    }
    printf("\n\nMV1 0 999 %d ", prog_size);
    return head;
}

//Passo 2 - Traduz o programa
void pass_two(FILE *arq, SymTable *head) {
    char line[100];
    char *word;
    int size, address_or_code, mem_addr = 0;

    while(fgets(line, 100, arq) != NULL) {
        line[strcspn(line, "\r\n")] = 0;
        word = strtok(line, " ");
        while (word != NULL){
            if (strncmp(word, ";", 1) == 0) { //Ignora linha se é um comentário
                break;
            } else if (strncmp(word, "\n", 1) != 0) {
                size = strlen(word);
                if (strncmp(&word[size-1], ":", 1) != 0) { //Ignora declaração de labels
                    if (has_symbol(word, head) == 1) { //Referência a um label
                    } else { //Instruções
                        address_or_code = retorna_instrucao(word);
                        if (((address_or_code >= 1) && (address_or_code <= 14))) { // Instruções com registador, exceto WORD e END
                            printf("%d ", address_or_code);
                            word = strtok(NULL, " "); //Próxima palavra
                            mem_addr += 2;
                            printf("%d ", get_address(word, head) - mem_addr);
                        } else if (address_or_code == 17) { //WORD
                            word = strtok(NULL, " "); //Próxima palavra
                            printf("%s ", word);
                            mem_addr++;

                        } else if ((address_or_code == 18) || (address_or_code == -1)){ //END ou inválido
                            printf("\b"); //Apaga espaço extra
                            return;

                        } else if (address_or_code == 15 || address_or_code == 16){ // HALT ou RET
                            printf("%d ", address_or_code);
                            mem_addr++;
                        }
                    }
                }
            }
            word = strtok(NULL, " "); //Próxima palavra da linha
        }
    }
}
