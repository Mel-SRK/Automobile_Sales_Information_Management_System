#include "employee.h"

Employee *emp_head = NULL;

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
    /* TODO: 枚举转字符串 */
    return "未知";
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
    /* TODO: 遍历链表比较 id */
    return NULL;
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
    /* TODO: 实现添加员工 */
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
    /* TODO: 实现删除员工 */
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
    /* TODO: 实现修改员工 */
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
    /* TODO: 实现查询员工 */
}

/**
 * emp_list_all - 显示全部员工记录
 *
 * 统计数量 → 打印表头 → 遍历打印 → 空链表提示
 */
void emp_list_all(void)
{
    /* TODO: 实现遍历打印全部员工 */
}

/* ---- 菜单 ---- */

/**
 * employee_menu - 员工信息管理子菜单
 *
 * 菜单项：1-添加 2-删除 3-修改 4-查询 5-显示全部 0-返回
 */
void employee_menu(void)
{
    /* TODO: 实现菜单循环 */
}

/* ---- 文件读写 ---- */

/**
 * emp_save - 保存员工数据到 data/employees.dat（wb）
 * emp_load - 从 data/employees.dat 加载（rb），头插法重建链表
 */
void emp_save(void)
{
    /* TODO: 实现文件保存 */
}

void emp_load(void)
{
    /* TODO: 实现文件加载 */
}
