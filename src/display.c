#include "display.h"

void display_symbol_data(t_list *symbol_data) {
    while (symbol_data) {
        symbol_t *symbol = symbol_data->content;
        printf("%016lx ---%c-- %s\n", symbol->value, symbol->type, symbol->name);
        symbol_data = symbol_data->next;
    }
}