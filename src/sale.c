#include "sale.h"
#include "car.h"
#include "employee.h"
#include "customer.h"

Sale *sale_head = NULL;
static Sale *sale_tail = NULL;
static int sale_id_counter = 0; /* 自增编号计数器，生成 S0001, S0002... */

typedef struct {
    char name[MAX_STR];
    int qty;
    double amount;
} SaleStat;

/*
 * 销售信息模块
 * ============
 * 数据结构字段：记录编号(id,自动生成)、销售日期(date)、
 *              汽车编号(car_id)、型号(car_model,冗余)、颜色(color,冗余)、
 *              数量(quantity)、经手人编号(emp_id)、经手人姓名(emp_name,冗余)、
 *              客户编号(cust_id)、客户名称(cust_name,冗余)、总价(total_price)
 *
 * 冗余存储说明：car_model/color/emp_name/cust_name 是从关联表查出后
 *              复制过来的，目的是报表显示时不依赖其他模块的数据完整性
 *
 * 关联关系：
 *   car_id     -> Car.id     （查型号、颜色、单价）
 *   emp_id     -> Employee.id（查经手人姓名）
 *   cust_id    -> Customer.id（查客户名称）
 *
 * 总价计算：total_price = 单价(car->price) × 数量(quantity)
 * 存储方式：单向链表，头指针 sale_head，头插法插入
 * 文件存储：data/sales.dat（二进制）
 */

/* ---- 查询接口 ---- */

/**
 * sale_find - 按记录编号精确查找销售记录
 * @id: 销售记录编号（如 "S0001"）
 *
 * 遍历链表 strcmp 比较 id
 * 返回值：找到返回节点指针，未找到返回 NULL
 */
Sale *sale_find(const char *id)
{
    for (Sale *p = sale_head; p != NULL; p = p->next) {
        if (strcmp(p->id, id) == 0) {
            return p;
        }
    }
    return NULL;
}

static void sale_append(Sale *node)
{
    node->next = NULL;
    if (sale_head == NULL) {
        sale_head = node;
        sale_tail = node;
    } else {
        sale_tail->next = node;
        sale_tail = node;
    }
}

static void sale_print_header(void)
{
    printf("%-10s %-12s %-14s %-8s %-6s %-12s %-16s %12s\n",
           "记录号", "销售日期", "型号", "颜色", "数量", "经手人", "客户", "总价");
    printf("--------------------------------------------------------------------------------\n");
}

static void sale_print_one(const Sale *p)
{
    printf("%-10s %-12s %-14s %-8s %-6d %-12s %-16s %12.2f\n",
           p->id, p->date, p->car_model, p->color, p->quantity,
           p->emp_name, p->cust_name, p->total_price);
}

static int read_positive_int(const char *prompt)
{
    int value = 0;
    while (value <= 0) {
        printf("%s", prompt);
        value = read_int();
        if (value <= 0) {
            printf("请输入大于 0 的整数。\n");
        }
    }
    return value;
}

static double read_nonnegative_double(const char *prompt)
{
    char buf[MAX_STR] = {0};
    double value = -1.0;
    while (value < 0.0) {
        printf("%s", prompt);
        safe_gets(buf, sizeof(buf));
        value = atof(buf);
        if (value < 0.0) {
            printf("请输入非负数字。\n");
        }
    }
    return value;
}

static void sale_add_stat(SaleStat stats[], int *count, const char *name,
                          int qty, double amount)
{
    const char *key = name[0] ? name : "未填写";
    for (int i = 0; i < *count; i++) {
        if (strcmp(stats[i].name, key) == 0) {
            stats[i].qty += qty;
            stats[i].amount += amount;
            return;
        }
    }
    if (*count >= 100) {
        return;
    }
    snprintf(stats[*count].name, sizeof(stats[*count].name), "%s", key);
    stats[*count].qty = qty;
    stats[*count].amount = amount;
    (*count)++;
}

static void sale_print_report(FILE *out,
                              SaleStat model_stats[], int model_count,
                              SaleStat emp_stats[], int emp_count,
                              int total_qty, double total_amount)
{
    fprintf(out, "【按型号统计】\n");
    fprintf(out, "%-20s %10s %14s\n", "型号", "销售数量", "销售金额");
    fprintf(out, "-----------------------------------------------\n");
    for (int i = 0; i < model_count; i++) {
        fprintf(out, "%-20s %10d %14.2f\n",
                model_stats[i].name, model_stats[i].qty, model_stats[i].amount);
    }
    fprintf(out, "-----------------------------------------------\n");
    fprintf(out, "%-20s %10d %14.2f\n\n", "合计", total_qty, total_amount);

    fprintf(out, "【按经手人统计】\n");
    fprintf(out, "%-20s %10s %14s\n", "经手人", "销售数量", "销售金额");
    fprintf(out, "-----------------------------------------------\n");
    for (int i = 0; i < emp_count; i++) {
        fprintf(out, "%-20s %10d %14.2f\n",
                emp_stats[i].name, emp_stats[i].qty, emp_stats[i].amount);
    }
    fprintf(out, "-----------------------------------------------\n");
    fprintf(out, "%-20s %10d %14.2f\n", "合计", total_qty, total_amount);
}

/* ---- 增删改查 ---- */

/**
 * sale_add - 添加销售记录
 *
 * 实现步骤：
 *   1. 分配新节点，生成编号 snprintf(s->id, "S%04d", ++sale_id_counter)
 *   2. 销售日期：默认今天（get_current_date），用户可覆盖输入
 *   3. 关联轿车：
 *      - 读取汽车编号，调用 car_find(car_id)
 *      - 找到则自动填充 car_model、color，打印提示
 *      - 未找到则手动输入型号和颜色
 *   4. 读取数量 quantity
 *   5. 关联员工：
 *      - 读取员工编号，调用 emp_find(emp_id)
 *      - 找到则自动填充 emp_name
 *      - 未找到则手动输入姓名
 *   6. 关联客户：
 *      - 读取客户编号，调用 cust_find(cust_id)
 *      - 找到则自动填充 cust_name
 *      - 未找到则手动输入名称
 *   7. 计算总价：
 *      - 如果 car_find 成功：total_price = car->price * quantity
 *      - 否则：提示用户手动输入单价，total_price = 单价 * quantity
 *   8. 头插法插入链表
 */
void sale_add(void)
{
    clear_screen();
    Sale *node = calloc(1, sizeof(Sale));
    double unit_price = 0.0;

    if (node == NULL) {
        printf("内存分配失败\n");
        pause_screen();
        return;
    }

    do {
        snprintf(node->id, sizeof(node->id), "S%04d", ++sale_id_counter);
    } while (sale_find(node->id) != NULL);

    printf("销售记录编号: %s\n", node->id);
    printf("销售日期(默认当日): ");
    safe_gets(node->date, sizeof(node->date));
    if (node->date[0] == '\0') {
        get_current_date(node->date, sizeof(node->date));
    }

    printf("汽车编号: ");
    safe_gets(node->car_id, sizeof(node->car_id));
    Car *car = car_find(node->car_id);
    if (car != NULL) {
        strcpy(node->car_model, car->model);
        strcpy(node->color, car->color);
        unit_price = car->price;
        printf("已关联车辆: %s / %s / 单价 %.2f\n",
               node->car_model, node->color, unit_price);
    } else {
        printf("未找到车辆信息，请手动输入型号和颜色。\n");
        printf("汽车型号: ");
        safe_gets(node->car_model, sizeof(node->car_model));
        printf("颜色: ");
        safe_gets(node->color, sizeof(node->color));
        unit_price = read_nonnegative_double("单价: ");
    }

    node->quantity = read_positive_int("销售数量: ");

    printf("经手人员工编号: ");
    safe_gets(node->emp_id, sizeof(node->emp_id));
    Employee *emp = emp_find(node->emp_id);
    if (emp != NULL) {
        strcpy(node->emp_name, emp->name);
        printf("已关联经手人: %s\n", node->emp_name);
    } else {
        printf("未找到员工信息，请手动输入经手人姓名: ");
        safe_gets(node->emp_name, sizeof(node->emp_name));
    }

    printf("客户编号: ");
    safe_gets(node->cust_id, sizeof(node->cust_id));
    Customer *cust = cust_find(node->cust_id);
    if (cust != NULL) {
        strcpy(node->cust_name, cust->name);
        printf("已关联客户: %s\n", node->cust_name);
    } else {
        printf("未找到客户信息，请手动输入客户名称: ");
        safe_gets(node->cust_name, sizeof(node->cust_name));
    }

    node->total_price = unit_price * node->quantity;
    printf("销售总价: %.2f\n", node->total_price);

    sale_append(node);
    printf("添加成功\n");
    pause_screen();
}

/**
 * sale_delete - 删除销售记录
 *
 * 实现步骤：
 *   1. 读取记录编号
 *   2. 遍历查找（记录前驱 prev）
 *   3. 确认后摘链、free
 */
void sale_delete(void)
{
    char id[MAX_STR] = {0};
    Sale *p = sale_head;
    Sale *prev = NULL;

    printf("需要删除的销售记录编号: ");
    safe_gets(id, sizeof(id));
    if (id[0] == '\0') {
        printf("请输入销售记录编号\n");
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
        printf("未找到此销售记录\n");
        pause_screen();
        return;
    }

    sale_print_header();
    sale_print_one(p);
    printf("确认删除此销售记录吗？ (y/n): ");
    char confirm[4] = {0};
    safe_gets(confirm, sizeof(confirm));
    if (confirm[0] != 'y' && confirm[0] != 'Y') {
        printf("已取消\n");
        pause_screen();
        return;
    }

    if (prev == NULL) {
        sale_head = p->next;
    } else {
        prev->next = p->next;
    }
    if (sale_tail == p) {
        sale_tail = prev;
    }
    free(p);
    printf("已删除\n");
    pause_screen();
}

/**
 * sale_query - 查询销售记录（子菜单）
 *
 * 支持四种查询方式：
 *   1. 按记录编号查询 — sale_find 精确匹配
 *   2. 按汽车型号查询 — 遍历链表 strstr 匹配 car_model
 *   3. 按经手人查询   — 遍历链表 strstr 匹配 emp_name
 *   4. 按日期范围查询 — 读取起止日期，strcmp(p->date, start) >= 0 && <= end
 *
 * 日期格式 YYYY-MM-DD，字符串比较即可满足日期范围判断（字典序 = 时间序）
 */
void sale_query(void)
{
    while (1) {
        clear_screen();
        printf("========== 查询销售记录 ==========\n");
        printf("   1. 按记录编号查询\n");
        printf("   2. 按汽车型号查询\n");
        printf("   3. 按经手人查询\n");
        printf("   4. 按日期范围查询\n");
        printf("   0. 返回\n");
        printf("==================================\n");
        printf("请选择: ");
        int choice = read_int();

        if (choice == 0) {
            return;
        }

        if (choice == 1) {
            char id[MAX_STR] = {0};
            printf("请输入记录编号: ");
            safe_gets(id, sizeof(id));
            Sale *p = sale_find(id);
            if (p == NULL) {
                printf("未找到匹配记录\n");
            } else {
                sale_print_header();
                sale_print_one(p);
            }
            pause_screen();
        } else if (choice == 2) {
            char keyword[MAX_STR] = {0};
            int count = 0;
            printf("请输入汽车型号关键词: ");
            safe_gets(keyword, sizeof(keyword));
            sale_print_header();
            for (Sale *p = sale_head; p != NULL; p = p->next) {
                if (strstr(p->car_model, keyword) != NULL) {
                    sale_print_one(p);
                    count++;
                }
            }
            printf("共 %d 条记录\n", count);
            pause_screen();
        } else if (choice == 3) {
            char keyword[MAX_STR] = {0};
            int count = 0;
            printf("请输入经手人姓名关键词: ");
            safe_gets(keyword, sizeof(keyword));
            sale_print_header();
            for (Sale *p = sale_head; p != NULL; p = p->next) {
                if (strstr(p->emp_name, keyword) != NULL) {
                    sale_print_one(p);
                    count++;
                }
            }
            printf("共 %d 条记录\n", count);
            pause_screen();
        } else if (choice == 4) {
            char start[MAX_STR] = {0};
            char end[MAX_STR] = {0};
            int count = 0;
            printf("开始日期(YYYY-MM-DD，留空表示不限): ");
            safe_gets(start, sizeof(start));
            printf("结束日期(YYYY-MM-DD，留空表示不限): ");
            safe_gets(end, sizeof(end));
            if (start[0] == '\0') {
                strcpy(start, "0000-00-00");
            }
            if (end[0] == '\0') {
                strcpy(end, "9999-99-99");
            }
            sale_print_header();
            for (Sale *p = sale_head; p != NULL; p = p->next) {
                if (strcmp(p->date, start) >= 0 && strcmp(p->date, end) <= 0) {
                    sale_print_one(p);
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
 * sale_list_all - 显示全部销售记录
 *
 * 统计数量 → 打印表头 → 遍历打印
 * 表头：记录号 | 销售日期 | 型号 | 颜色 | 数量 | 经手人 | 客户 | 总价
 */
void sale_list_all(void)
{
    clear_screen();
    if (sale_head == NULL) {
        printf("暂无销售数据\n");
        pause_screen();
        return;
    }

    int count = 0;
    sale_print_header();
    for (Sale *p = sale_head; p != NULL; p = p->next) {
        sale_print_one(p);
        count++;
    }
    printf("共 %d 条记录\n", count);
    pause_screen();
}

/**
 * sale_report - 打印销售报表（题目核心需求）
 *
 * 实现步骤：
 *   1. 遍历链表，汇总统计
 *
 *   2. 按型号统计（去重合并）：
 *      - 用数组记录每种型号的 qty 和 amount
 *      - 遍历时已存在则累加，否则新增一条
 *      - 打印：型号 | 销售数量 | 销售金额
 *      - 最后打印合计行
 *
 *   3. 按经手人统计（同上逻辑）：
 *      - 打印：经手人 | 销售数量 | 销售金额
 *
 *   4. 导出到文件 data/report.txt（fprintf 文本格式）
 *      - 与屏幕显示内容一致
 *      - 打开失败则提示警告
 *
 * 注意：typedef struct { char name[MAX_STR]; int qty; double amount; } Stat;
 *       用 Stat stats[100] 做简单去重统计即可，不需要复杂数据结构
 */
void sale_report(void)
{
    SaleStat model_stats[100] = {0};
    SaleStat emp_stats[100] = {0};
    int model_count = 0;
    int emp_count = 0;
    int total_qty = 0;
    double total_amount = 0.0;

    clear_screen();
    if (sale_head == NULL) {
        printf("暂无销售数据，无法生成报表\n");
        pause_screen();
        return;
    }

    for (Sale *p = sale_head; p != NULL; p = p->next) {
        sale_add_stat(model_stats, &model_count, p->car_model, p->quantity, p->total_price);
        sale_add_stat(emp_stats, &emp_count, p->emp_name, p->quantity, p->total_price);
        total_qty += p->quantity;
        total_amount += p->total_price;
    }

    sale_print_report(stdout, model_stats, model_count, emp_stats, emp_count,
                      total_qty, total_amount);

    FILE *fp = fopen(DATA_DIR "report.txt", "w");
    if (fp == NULL) {
        printf("\n报表文件导出失败\n");
    } else {
        sale_print_report(fp, model_stats, model_count, emp_stats, emp_count,
                          total_qty, total_amount);
        fclose(fp);
        printf("\n报表已导出到 %sreport.txt\n", DATA_DIR);
    }
    pause_screen();
}

/* ---- 菜单 ---- */

/**
 * sale_menu - 销售信息管理子菜单
 *
 * 菜单项：
 *   1. 添加销售记录 -> sale_add
 *   2. 删除销售记录 -> sale_delete
 *   3. 查询销售记录 -> sale_query
 *   4. 显示全部记录 -> sale_list_all
 *   5. 打印销售报表 -> sale_report
 *   0. 返回主菜单   -> return
 */
void sale_menu(void)
{
    while (1) {
        clear_screen();
        printf("==========================================\n");
        printf("             销售信息管理系统\n");
        printf("==========================================\n");
        printf("   1. 添加销售记录\n");
        printf("   2. 删除销售记录\n");
        printf("   3. 查询销售记录\n");
        printf("   4. 显示全部记录\n");
        printf("   5. 打印销售报表\n");
        printf("   0. 返回\n");
        printf("==========================================\n");
        printf("请选择: ");

        int choice = read_int();
        switch (choice) {
        case 1:
            sale_add();
            break;
        case 2:
            sale_delete();
            break;
        case 3:
            sale_query();
            break;
        case 4:
            sale_list_all();
            break;
        case 5:
            sale_report();
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
 * sale_save - 保存销售数据到 data/sales.dat（wb）
 * sale_load - 从 data/sales.dat 加载（rb），头插法重建链表
 *
 * 注意：加载时需同步更新 sale_id_counter 为当前最大编号
 */
void sale_save(void)
{
    FILE *fp = fopen(DATA_DIR "sales.dat", "wb");
    if (fp == NULL) {
        printf("销售数据保存失败\n");
        return;
    }

    for (Sale *p = sale_head; p != NULL; p = p->next) {
        Sale tmp = *p;
        tmp.next = NULL;
        fwrite(&tmp, sizeof(Sale), 1, fp);
    }
    fclose(fp);
}

void sale_load(void)
{
    FILE *fp = fopen(DATA_DIR "sales.dat", "rb");
    if (fp == NULL) {
        return;
    }

    Sale tmp;
    while (fread(&tmp, sizeof(Sale), 1, fp) == 1) {
        Sale *node = calloc(1, sizeof(Sale));
        if (node == NULL) {
            break;
        }
        *node = tmp;
        sale_append(node);
        if (node->id[0] == 'S') {
            int number = atoi(node->id + 1);
            if (number > sale_id_counter) {
                sale_id_counter = number;
            }
        }
    }
    fclose(fp);
}
