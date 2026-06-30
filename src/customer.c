#include "customer.h"

Customer *cust_head = NULL;
static Customer *cust_tail = NULL;
static int cust_id_counter = 0; /* 自增编号计数器，生成 C0001, C0002... */

/*
 * 客户信息模块
 * ============
 * 数据结构字段：编号(id,自动生成)、名称(name)、联系方式(phone)、
 *              地址(address)、业务联系记录(contact_log)
 * 存储方式：单向链表，头指针 cust_head，头插法插入
 * 文件存储：data/customers.dat（二进制）
 *
 * 特殊功能：业务联系记录可追加，格式 [YYYY-MM-DD] 内容;
 */

/* ---- 查询接口 ---- */

/**
 * cust_find - 按编号精确查找客户
 * @id: 客户编号（如 "C0001"）
 *
 * 遍历链表 strcmp 比较 id
 * 返回值：找到返回节点指针，未找到返回 NULL
 */
Customer *cust_find(const char *id)
{
    for (Customer *p = cust_head; p != NULL; p = p->next) {
        if (strcmp(p->id, id) == 0) {
            return p;
        }
    }
    return NULL;
}

static void cust_append(Customer *node)
{
    node->next = NULL;
    if (cust_head == NULL) {
        cust_head = node;
        cust_tail = node;
    } else {
        cust_tail->next = node;
        cust_tail = node;
    }
}

static void cust_print_header(void)
{
    printf("%-10s %-16s %-16s %-30s\n",
           "编号", "客户名称", "联系方式", "地址");
    printf("--------------------------------------------------------------------------\n");
}

static void cust_print_one(const Customer *p)
{
    printf("%-10s %-16s %-16s %-30s\n",
           p->id, p->name, p->phone, p->address);
}

static void cust_print_full(const Customer *p)
{
    printf("客户编号: %s\n", p->id);
    printf("客户名称: %s\n", p->name);
    printf("联系方式: %s\n", p->phone);
    printf("地址: %s\n", p->address);
    printf("业务联系记录: %s\n", p->contact_log[0] ? p->contact_log : "暂无");
}

static int cust_append_contact(Customer *p, const char *content)
{
    char date[MAX_STR] = {0};
    char entry[MAX_LONG] = {0};
    size_t current_len = strlen(p->contact_log);

    if (content[0] == '\0') {
        return 1;
    }

    get_current_date(date, sizeof(date));
    snprintf(entry, sizeof(entry), "[%s] %s; ", date, content);
    if (current_len + strlen(entry) >= sizeof(p->contact_log)) {
        return 0;
    }

    strcat(p->contact_log, entry);
    return 1;
}

/* ---- 增删改查 ---- */

/**
 * cust_add - 添加客户记录
 *
 * 实现步骤：
 *   1. 分配新节点
 *   2. 自动生成编号：snprintf(c->id, MAX_STR, "C%04d", ++cust_id_counter)
 *   3. 依次读取：名称、联系方式、地址、首次业务联系记录
 *   4. 头插法插入
 *
 * 注意：编号自动生成，不需要用户输入
 */
void cust_add(void)
{
    clear_screen();
    Customer *node = calloc(1, sizeof(Customer));
    char first_log[MAX_LONG] = {0};

    if (node == NULL) {
        printf("内存分配失败\n");
        pause_screen();
        return;
    }

    do {
        snprintf(node->id, sizeof(node->id), "C%04d", ++cust_id_counter);
    } while (cust_find(node->id) != NULL);

    printf("客户编号: %s\n", node->id);
    printf("客户名称: ");
    safe_gets(node->name, sizeof(node->name));
    printf("联系方式: ");
    safe_gets(node->phone, sizeof(node->phone));
    printf("地址: ");
    safe_gets(node->address, sizeof(node->address));
    printf("首次业务联系记录: ");
    safe_gets(first_log, sizeof(first_log));
    if (!cust_append_contact(node, first_log)) {
        printf("联系记录过长，已跳过首次记录\n");
    }

    cust_append(node);
    printf("添加成功\n");
    pause_screen();
}

/**
 * cust_delete - 删除客户记录
 *
 * 实现步骤：
 *   1. 读取客户编号
 *   2. 遍历查找（记录前驱 prev）
 *   3. 确认后摘链、free
 */
void cust_delete(void)
{
    char id[MAX_STR] = {0};
    Customer *p = cust_head;
    Customer *prev = NULL;

    printf("需要删除的客户编号: ");
    safe_gets(id, sizeof(id));
    if (id[0] == '\0') {
        printf("请输入客户编号\n");
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
        printf("未找到此客户\n");
        pause_screen();
        return;
    }

    cust_print_header();
    cust_print_one(p);
    printf("确认删除此客户吗？ (y/n): ");
    char confirm[4] = {0};
    safe_gets(confirm, sizeof(confirm));
    if (confirm[0] != 'y' && confirm[0] != 'Y') {
        printf("已取消\n");
        pause_screen();
        return;
    }

    if (prev == NULL) {
        cust_head = p->next;
    } else {
        prev->next = p->next;
    }
    if (cust_tail == p) {
        cust_tail = prev;
    }
    free(p);
    printf("已删除\n");
    pause_screen();
}

/**
 * cust_modify - 修改客户记录
 *
 * 实现步骤：
 *   1. 按编号查找，未找到提示返回
 *   2. 可修改字段：名称、联系方式、地址
 *   3. 联系记录通过 cust_add_contact_log 单独追加，此处不修改
 *   4. 回车保留原值
 */
void cust_modify(void)
{
    char id[MAX_STR] = {0};
    char short_buf[MAX_STR] = {0};
    char long_buf[MAX_LONG] = {0};

    printf("需要修改的客户编号: ");
    safe_gets(id, sizeof(id));
    if (id[0] == '\0') {
        printf("请输入客户编号\n");
        pause_screen();
        return;
    }

    Customer *p = cust_find(id);
    if (p == NULL) {
        printf("未找到此客户\n");
        pause_screen();
        return;
    }

    cust_print_full(p);

    printf("客户名称(当前: %s，回车保留): ", p->name);
    safe_gets(short_buf, sizeof(short_buf));
    if (short_buf[0] != '\0') {
        strcpy(p->name, short_buf);
    }

    printf("联系方式(当前: %s，回车保留): ", p->phone);
    safe_gets(short_buf, sizeof(short_buf));
    if (short_buf[0] != '\0') {
        strcpy(p->phone, short_buf);
    }

    printf("地址(当前: %s，回车保留): ", p->address);
    safe_gets(long_buf, sizeof(long_buf));
    if (long_buf[0] != '\0') {
        strcpy(p->address, long_buf);
    }

    printf("修改成功\n");
    pause_screen();
}

/**
 * cust_query - 查询客户记录（子菜单）
 *
 * 支持三种查询方式：
 *   1. 按编号查询 — cust_find 精确匹配
 *   2. 按名称查询 — 遍历链表 strstr 模糊匹配
 *   3. 查看完整信息 — 显示全部字段，包括业务联系记录
 */
void cust_query(void)
{
    while (1) {
        clear_screen();
        printf("========== 查询客户信息 ==========\n");
        printf("   1. 按编号查询\n");
        printf("   2. 按名称模糊查询\n");
        printf("   3. 查看完整信息\n");
        printf("   0. 返回\n");
        printf("==================================\n");
        printf("请选择: ");
        int choice = read_int();

        if (choice == 0) {
            return;
        }

        if (choice == 1 || choice == 3) {
            char id[MAX_STR] = {0};
            printf("请输入客户编号: ");
            safe_gets(id, sizeof(id));
            Customer *p = cust_find(id);
            if (p == NULL) {
                printf("未找到匹配记录\n");
            } else if (choice == 1) {
                cust_print_header();
                cust_print_one(p);
            } else {
                cust_print_full(p);
            }
            pause_screen();
        } else if (choice == 2) {
            char keyword[MAX_STR] = {0};
            int count = 0;
            printf("请输入客户名称关键词: ");
            safe_gets(keyword, sizeof(keyword));
            cust_print_header();
            for (Customer *p = cust_head; p != NULL; p = p->next) {
                if (strstr(p->name, keyword) != NULL) {
                    cust_print_one(p);
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
 * cust_list_all - 显示全部客户记录
 *
 * 统计数量 → 打印表头(编号/名称/联系方式/地址) → 遍历打印
 */
void cust_list_all(void)
{
    clear_screen();
    if (cust_head == NULL) {
        printf("暂无客户数据\n");
        pause_screen();
        return;
    }

    int count = 0;
    cust_print_header();
    for (Customer *p = cust_head; p != NULL; p = p->next) {
        cust_print_one(p);
        count++;
    }
    printf("共 %d 条记录\n", count);
    pause_screen();
}

/**
 * cust_add_contact_log - 追加业务联系记录
 *
 * 实现步骤：
 *   1. 读取客户编号，cust_find 查找
 *   2. 打印当前已有联系记录
 *   3. 读取新的联系内容
 *   4. 拼接日期前缀：snprintf(entry, "[YYYY-MM-DD] %s; ", 新记录)
 *   5. 检查总长度不超过 MAX_LONG，strcat 追加到 contact_log
 */
void cust_add_contact_log(void)
{
    char id[MAX_STR] = {0};
    char content[MAX_LONG] = {0};

    printf("客户编号: ");
    safe_gets(id, sizeof(id));
    Customer *p = cust_find(id);
    if (p == NULL) {
        printf("未找到此客户\n");
        pause_screen();
        return;
    }

    printf("当前联系记录: %s\n", p->contact_log[0] ? p->contact_log : "暂无");
    printf("新的联系内容: ");
    safe_gets(content, sizeof(content));
    if (!cust_append_contact(p, content)) {
        printf("联系记录空间不足，追加失败\n");
        pause_screen();
        return;
    }

    printf("追加成功\n");
    pause_screen();
}

/* ---- 菜单 ---- */

/**
 * customer_menu - 客户信息管理子菜单
 *
 * 菜单项：1-添加 2-删除 3-修改 4-查询 5-显示全部 6-追加联系记录 0-返回
 */
void customer_menu(void)
{
    while (1) {
        clear_screen();
        printf("==========================================\n");
        printf("             客户信息管理系统\n");
        printf("==========================================\n");
        printf("   1. 添加客户信息\n");
        printf("   2. 删除客户信息\n");
        printf("   3. 修改客户信息\n");
        printf("   4. 查询客户信息\n");
        printf("   5. 显示全部客户\n");
        printf("   6. 追加联系记录\n");
        printf("   0. 返回\n");
        printf("==========================================\n");
        printf("请选择: ");

        int choice = read_int();
        switch (choice) {
        case 1:
            cust_add();
            break;
        case 2:
            cust_delete();
            break;
        case 3:
            cust_modify();
            break;
        case 4:
            cust_query();
            break;
        case 5:
            cust_list_all();
            break;
        case 6:
            cust_add_contact_log();
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
 * cust_save - 保存客户数据到 data/customers.dat（wb）
 * cust_load - 从 data/customers.dat 加载（rb），头插法重建链表
 *
 * 注意：加载时需同步更新 cust_id_counter 为当前最大编号，
 *       避免自增编号冲突
 */
void cust_save(void)
{
    FILE *fp = fopen(DATA_DIR "customers.dat", "wb");
    if (fp == NULL) {
        printf("客户数据保存失败\n");
        return;
    }

    for (Customer *p = cust_head; p != NULL; p = p->next) {
        Customer tmp = *p;
        tmp.next = NULL;
        fwrite(&tmp, sizeof(Customer), 1, fp);
    }
    fclose(fp);
}

void cust_load(void)
{
    FILE *fp = fopen(DATA_DIR "customers.dat", "rb");
    if (fp == NULL) {
        return;
    }

    Customer tmp;
    while (fread(&tmp, sizeof(Customer), 1, fp) == 1) {
        Customer *node = calloc(1, sizeof(Customer));
        if (node == NULL) {
            break;
        }
        *node = tmp;
        cust_append(node);
        if (node->id[0] == 'C') {
            int number = atoi(node->id + 1);
            if (number > cust_id_counter) {
                cust_id_counter = number;
            }
        }
    }
    fclose(fp);
}
