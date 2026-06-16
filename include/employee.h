#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "utils.h"

typedef enum {
    EDU_HIGH_SCHOOL = 1,
    EDU_COLLEGE,
    EDU_BACHELOR,
    EDU_MASTER,
    EDU_PHD
} Education;

typedef struct Employee {
    char id[MAX_STR];
    char name[MAX_STR];
    char gender[4];
    int age;
    char hometown[MAX_STR];
    Education education;
    struct Employee *next;
} Employee;

extern Employee *emp_head;

void employee_menu(void);
void emp_add(void);
void emp_delete(void);
void emp_modify(void);
void emp_query(void);
void emp_list_all(void);
void emp_save(void);
void emp_load(void);
Employee *emp_find(const char *id);
const char *edu_to_str(Education e);

#endif
