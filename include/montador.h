#ifndef MONTADOR_H
#define MONTADOR_H

#include <stdlib.h>
#include <stdio.h>

// TODO: structs, classes, e outras definições.
int retorna_instrucao(char str[]);

// Tabela de símbolos + funções construtoras
typedef struct SymTable {
    char symbol[50];
    int mem_addr;
    struct SymTable *next;
}SymTable;

int get_address(char *symbol, SymTable *head);

int has_symbol(char symbol[], SymTable *head);

SymTable * add_symbol(char symbol[], SymTable *head);

SymTable * add_address(char symbol[], int addr, SymTable *head);

// Passos do montador
SymTable * pass_one(FILE *arq, SymTable *head);
void pass_two(FILE *arq, SymTable *head);

#endif
