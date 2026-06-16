#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "utils.h"

typedef struct Customer {
    char id[MAX_STR];
    char name[MAX_STR];
    char phone[MAX_STR];
    char address[MAX_LONG];
    char contact_log[MAX_LONG];
    struct Customer *next;
} Customer;

extern Customer *cust_head;

void customer_menu(void);
void cust_add(void);
void cust_delete(void);
void cust_modify(void);
void cust_query(void);
void cust_list_all(void);
void cust_add_contact_log(void);
void cust_save(void);
void cust_load(void);
Customer *cust_find(const char *id);

#endif
