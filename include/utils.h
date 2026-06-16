#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STR 64
#define MAX_LONG 256
#define DATA_DIR "data/"

void clear_screen(void);
void pause_screen(void);
void safe_gets(char *buf, int size);
int read_int(void);
void get_current_date(char *buf, int size);

#endif
