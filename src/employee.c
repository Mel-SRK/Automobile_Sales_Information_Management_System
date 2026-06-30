#include "employee.h"

Employee *emp_head = NULL;
static Employee *emp_tail = NULL;

/*
 * 员工信息模块
 * ============
 * 数据结构字段：编号(id)、姓名(name)、性别(gender)、
 *              年龄(age)、籍贯(hometown)、学历(education)
 * 学历枚举：1-高中 2-大专 3-本科 4-硕士 5-博士
 * 存储方式：单向链表，头指针 emp_head，头插法插入
 * 文件存储：data/employees.dat（二进制）
 */

/* ---- 工具函数 ---- */

/**
 * edu_to_str - 学历枚举转中文字符串
 * @e: Education 枚举值
 *
 * 返回值：对应的中文字符串指针，如 "本科"、"硕士"
 * 可用静态字符串数组实现：const char *str[] = {"", "高中", "大专", ...};
 */
const char *edu_to_str(Education e)
{
    switch (e) {
    case EDU_HIGH_SCHOOL:
        return "高中";
    case EDU_COLLEGE:
        return "大专";
    case EDU_BACHELOR:
        return "本科";
    case EDU_MASTER:
        return "硕士";
    case EDU_PHD:
        return "博士";
    default:
        return "未知";
    }
}

/* ---- 查询接口 ---- */

/**
 * emp_find - 按编号精确查找员工
 * @id: 员工编号
 *
 * 遍历链表 strcmp 比较 id
 * 返回值：找到返回节点指针，未找到返回 NULL
 */
Employee *emp_find(const char *id)
{
    for (Employee *p = emp_head; p != NULL; p = p->next) {
        if (strcmp(p->id, id) == 0) {
            return p;
        }
    }
    return NULL;
}

static void emp_append(Employee *node)
{
    node->next = NULL;
    if (emp_head == NULL) {
        emp_head = node;
        emp_tail = node;
    } else {
        emp_tail->next = node;
        emp_tail = node;
    }
}

static void emp_print_header(void)
{
    printf("%-10s %-12s %-6s %-6s %-14s %-8s\n",
           "编号", "姓名", "性别", "年龄", "籍贯", "学历");
    printf("--------------------------------------------------------\n");
}

static void emp_print_one(const Employee *p)
{
    printf("%-10s %-12s %-6s %-6d %-14s %-8s\n",
           p->id, p->name, p->gender, p->age, p->hometown,
           edu_to_str(p->education));
}

static Education read_education_choice(Education current, int allow_keep)
{
    char buf[MAX_STR] = {0};
    while (1) {
        if (allow_keep) {
            printf("学历(1-高中 2-大专 3-本科 4-硕士 5-博士，当前: %s，回车保留): ",
                   edu_to_str(current));
        } else {
            printf("学历(1-高中 2-大专 3-本科 4-硕士 5-博士): ");
        }
        safe_gets(buf, sizeof(buf));
        if (allow_keep && buf[0] == '\0') {
            return current;
        }
        int choice = atoi(buf);
        if (choice >= EDU_HIGH_SCHOOL && choice <= EDU_PHD) {
            return (Education)choice;
        }
        printf("学历输入无效，请重新输入。\n");
    }
}

/* ---- 增删改查 ---- */

/**
 * emp_add - 添加员工记录
 *
 * 实现步骤：
 *   1. 分配新节点 (calloc)
 *   2. 依次读取：编号、姓名、性别(男/女)、年龄、籍贯、学历
 *   3. 学历输入 1-5 的数字，转为 Education 枚举
 *   4. 编号查重
 *   5. 头插法插入
 */
void emp_add(void)
{
    clear_screen();
    Employee *node = calloc(1, sizeof(Employee));
    char buf[MAX_STR] = {0};

    if (node == NULL) {
        printf("内存分配失败\n");
        pause_screen();
        return;
    }

    printf("员工编号: ");
    safe_gets(node->id, sizeof(node->id));
    if (node->id[0] == '\0') {
        printf("员工编号不能为空\n");
        free(node);
        pause_screen();
        return;
    }
    if (emp_find(node->id) != NULL) {
        printf("员工编号已存在\n");
        free(node);
        pause_screen();
        return;
    }

    printf("姓名: ");
    safe_gets(node->name, sizeof(node->name));
    printf("性别: ");
    safe_gets(node->gender, sizeof(node->gender));
    printf("年龄: ");
    safe_gets(buf, sizeof(buf));
    node->age = atoi(buf);
    printf("籍贯: ");
    safe_gets(node->hometown, sizeof(node->hometown));
    node->education = read_education_choice(EDU_BACHELOR, 0);

    emp_append(node);
    printf("添加成功\n");
    pause_screen();
}

/**
 * emp_delete - 删除员工记录
 *
 * 实现步骤：
 *   1. 读取编号
 *   2. 遍历查找（记录前驱 prev）
 *   3. 确认后摘链、free
 */
void emp_delete(void)
{
    char id[MAX_STR] = {0};
    Employee *p = emp_head;
    Employee *prev = NULL;

    printf("需要删除的员工编号: ");
    safe_gets(id, sizeof(id));
    if (id[0] == '\0') {
        printf("请输入员工编号\n");
        pause_screen();
        return;
    }

    while (p != NULL) {
        if (strcmp(p->id, id) == 0) {
            break;
        }
        prev = p;
        p = p->next;
    }
    if (p == NULL) {
        printf("未找到此员工\n");
        pause_screen();
        return;
    }

    emp_print_header();
    emp_print_one(p);
    printf("确认删除此员工吗？ (y/n): ");
    char confirm[4] = {0};
    safe_gets(confirm, sizeof(confirm));
    if (confirm[0] != 'y' && confirm[0] != 'Y') {
        printf("已取消\n");
        pause_screen();
        return;
    }

    if (prev == NULL) {
        emp_head = p->next;
    } else {
        prev->next = p->next;
    }
    if (emp_tail == p) {
        emp_tail = prev;
    }
    free(p);
    printf("已删除\n");
    pause_screen();
}

/**
 * emp_modify - 修改员工记录
 *
 * 实现步骤：
 *   1. 按编号查找，未找到提示返回
 *   2. 逐字段提示输入，回车保留原值
 *   3. 学历修改显示当前值和选项提示
 */
void emp_modify(void)
{
    char id[MAX_STR] = {0};
    char buf[MAX_STR] = {0};

    printf("需要修改的员工编号: ");
    safe_gets(id, sizeof(id));
    if (id[0] == '\0') {
        printf("请输入员工编号\n");
        pause_screen();
        return;
    }

    Employee *p = emp_find(id);
    if (p == NULL) {
        printf("未找到此员工\n");
        pause_screen();
        return;
    }

    emp_print_header();
    emp_print_one(p);

    printf("姓名(当前: %s，回车保留): ", p->name);
    safe_gets(buf, sizeof(buf));
    if (buf[0] != '\0') {
        strcpy(p->name, buf);
    }

    printf("性别(当前: %s，回车保留): ", p->gender);
    safe_gets(buf, sizeof(buf));
    if (buf[0] != '\0') {
        strcpy(p->gender, buf);
    }

    printf("年龄(当前: %d，回车保留): ", p->age);
    safe_gets(buf, sizeof(buf));
    if (buf[0] != '\0') {
        p->age = atoi(buf);
    }

    printf("籍贯(当前: %s，回车保留): ", p->hometown);
    safe_gets(buf, sizeof(buf));
    if (buf[0] != '\0') {
        strcpy(p->hometown, buf);
    }

    p->education = read_education_choice(p->education, 1);
    printf("修改成功\n");
    pause_screen();
}

/**
 * emp_query - 查询员工记录（子菜单）
 *
 * 支持三种查询方式：
 *   1. 按编号查询 — emp_find 精确匹配
 *   2. 按姓名查询 — 遍历链表 strstr 模糊匹配
 *   3. 按学历查询 — 输入 1-5，遍历比较 p->education
 */
void emp_query(void)
{
    while (1) {
        clear_screen();
        printf("========== 查询员工信息 ==========\n");
        printf("   1. 按编号查询\n");
        printf("   2. 按姓名模糊查询\n");
        printf("   3. 按学历筛选\n");
        printf("   0. 返回\n");
        printf("==================================\n");
        printf("请选择: ");
        int choice = read_int();

        if (choice == 0) {
            return;
        }

        if (choice == 1) {
            char id[MAX_STR] = {0};
            printf("请输入员工编号: ");
            safe_gets(id, sizeof(id));
            Employee *p = emp_find(id);
            if (p == NULL) {
                printf("未找到匹配记录\n");
            } else {
                emp_print_header();
                emp_print_one(p);
            }
            pause_screen();
        } else if (choice == 2) {
            char keyword[MAX_STR] = {0};
            int count = 0;
            printf("请输入姓名关键词: ");
            safe_gets(keyword, sizeof(keyword));
            emp_print_header();
            for (Employee *p = emp_head; p != NULL; p = p->next) {
                if (strstr(p->name, keyword) != NULL) {
                    emp_print_one(p);
                    count++;
                }
            }
            printf("共 %d 条记录\n", count);
            pause_screen();
        } else if (choice == 3) {
            Education edu = read_education_choice(EDU_BACHELOR, 0);
            int count = 0;
            emp_print_header();
            for (Employee *p = emp_head; p != NULL; p = p->next) {
                if (p->education == edu) {
                    emp_print_one(p);
                    count++;
                }
            }
            printf("共 %d 条记录\n", count);
            pause_screen();
        } else {
            printf("无效选择\n");
            pause_screen();
        }
    }
}

/**
 * emp_list_all - 显示全部员工记录
 *
 * 统计数量 → 打印表头 → 遍历打印 → 空链表提示
 */
void emp_list_all(void)
{
    clear_screen();
    if (emp_head == NULL) {
        printf("暂无员工数据\n");
        pause_screen();
        return;
    }

    int count = 0;
    emp_print_header();
    for (Employee *p = emp_head; p != NULL; p = p->next) {
        emp_print_one(p);
        count++;
    }
    printf("共 %d 条记录\n", count);
    pause_screen();
}

/* ---- 菜单 ---- */

/**
 * employee_menu - 员工信息管理子菜单
 *
 * 菜单项：1-添加 2-删除 3-修改 4-查询 5-显示全部 0-返回
 */
void employee_menu(void)
{
    while (1) {
        clear_screen();
        printf("==========================================\n");
        printf("             员工信息管理系统\n");
        printf("==========================================\n");
        printf("   1. 添加员工信息\n");
        printf("   2. 删除员工信息\n");
        printf("   3. 修改员工信息\n");
        printf("   4. 查询员工信息\n");
        printf("   5. 显示全部员工\n");
        printf("   0. 返回\n");
        printf("==========================================\n");
        printf("请选择: ");

        int choice = read_int();
        switch (choice) {
        case 1:
            emp_add();
            break;
        case 2:
            emp_delete();
            break;
        case 3:
            emp_modify();
            break;
        case 4:
            emp_query();
            break;
        case 5:
            emp_list_all();
            break;
        case 0:
            return;
        default:
            printf("无效选择\n");
            pause_screen();
            break;
        }
    }
}

/* ---- 文件读写 ---- */

/**
 * emp_save - 保存员工数据到 data/employees.dat（wb）
 * emp_load - 从 data/employees.dat 加载（rb），头插法重建链表
 */
void emp_save(void)
{
    FILE *fp = fopen(DATA_DIR "employees.dat", "wb");
    if (fp == NULL) {
        printf("员工数据保存失败\n");
        return;
    }

    for (Employee *p = emp_head; p != NULL; p = p->next) {
        Employee tmp = *p;
        tmp.next = NULL;
        fwrite(&tmp, sizeof(Employee), 1, fp);
    }
    fclose(fp);
}

void emp_load(void)
{
    FILE *fp = fopen(DATA_DIR "employees.dat", "rb");
    if (fp == NULL) {
        return;
    }

    Employee tmp;
    while (fread(&tmp, sizeof(Employee), 1, fp) == 1) {
        Employee *node = calloc(1, sizeof(Employee));
        if (node == NULL) {
            break;
        }
        *node = tmp;
        emp_append(node);
    }
    fclose(fp);
}
