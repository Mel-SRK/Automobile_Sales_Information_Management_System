#include "sale.h"
#include "car.h"
#include "employee.h"
#include "customer.h"

Sale *sale_head = NULL;
static int sale_id_counter = 0; /* 自增编号计数器，生成 S0001, S0002... */

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
    /* TODO: 遍历链表比较 id */
    return NULL;
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
    /* TODO: 实现添加销售记录（核心功能，需关联三个模块） */
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
    /* TODO: 实现删除销售记录 */
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
    /* TODO: 实现查询销售记录 */
}

/**
 * sale_list_all - 显示全部销售记录
 *
 * 统计数量 → 打印表头 → 遍历打印
 * 表头：记录号 | 销售日期 | 型号 | 颜色 | 数量 | 经手人 | 客户 | 总价
 */
void sale_list_all(void)
{
    /* TODO: 实现遍历打印全部销售记录 */
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
    /* TODO: 实现销售报表（按型号统计 + 按经手人统计 + 文件导出） */
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
    /* TODO: 实现菜单循环 */
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
    /* TODO: 实现文件保存 */
}

void sale_load(void)
{
    /* TODO: 实现文件加载 */
}
