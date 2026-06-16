#include "customer.h"

Customer *cust_head = NULL;
static int cust_id_counter = 1000; /* 自增编号计数器，生成 C0001, C0002... */

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
    /* TODO: 遍历链表比较 id */
    return NULL;
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
    /* TODO: 实现添加客户 */
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
    /* TODO: 实现删除客户 */
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
    /* TODO: 实现修改客户 */
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
    /* TODO: 实现查询客户 */
}

/**
 * cust_list_all - 显示全部客户记录
 *
 * 统计数量 → 打印表头(编号/名称/联系方式/地址) → 遍历打印
 */
void cust_list_all(void)
{
    /* TODO: 实现遍历打印全部客户 */
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
    /* TODO: 实现追加联系记录 */
}

/* ---- 菜单 ---- */

/**
 * customer_menu - 客户信息管理子菜单
 *
 * 菜单项：1-添加 2-删除 3-修改 4-查询 5-显示全部 6-追加联系记录 0-返回
 */
void customer_menu(void)
{
    /* TODO: 实现菜单循环 */
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
    /* TODO: 实现文件保存 */
}

void cust_load(void)
{
    /* TODO: 实现文件加载 */
}
