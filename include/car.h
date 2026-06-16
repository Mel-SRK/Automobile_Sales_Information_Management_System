#ifndef CAR_H
#define CAR_H

#include "utils.h"

typedef struct Car {
    char id[MAX_STR];
    char model[MAX_STR];
    char color[MAX_STR];
    char manufacturer[MAX_STR];
    char date[MAX_STR];
    double price;
    struct Car *next;
} Car;

extern Car *car_head;

void car_menu(void);
void car_add(void);
void car_delete(void);
void car_modify(void);
void car_query(void);
void car_list_all(void);
void car_save(void);
void car_load(void);
Car *car_find(const char *id);

#endif
