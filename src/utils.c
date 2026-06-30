#include "utils.h"

void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause_screen(void) {
    printf("\n按回车键继续...");
    getchar();
}

void safe_gets(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    } else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }
}

int read_int(void) {
    char buf[MAX_STR];
    safe_gets(buf, sizeof(buf));
    return atoi(buf);
}

void get_current_date(char *buf, int size) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    snprintf(buf, size, "%04d-%02d-%02d",
             tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
}
