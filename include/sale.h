#ifndef SALE_H
#define SALE_H

#include "utils.h"

typedef struct Sale {
    char id[MAX_STR];
    char date[MAX_STR];
    char car_id[MAX_STR];
    char car_model[MAX_STR];
    char color[MAX_STR];
    int quantity;
    char emp_id[MAX_STR];
    char emp_name[MAX_STR];
    char cust_id[MAX_STR];
    char cust_name[MAX_STR];
    double total_price;
    struct Sale *next;
} Sale;

extern Sale *sale_head;

void sale_menu(void);
void sale_add(void);
void sale_delete(void);
void sale_query(void);
void sale_list_all(void);
void sale_report(void);
void sale_save(void);
void sale_load(void);
Sale *sale_find(const char *id);

#endif
