#include "car.h"
#include "utils.h"
#include <stdio.h>
#include <time.h>

Car *car_head = NULL;

/*
 * 轿车信息模块
 * ============
 * 数据结构字段：编号(id)、型号(model)、颜色(color)、
 *              生产厂家(manufacturer)、出厂日期(date)、价格(price)
 * 存储方式：单向链表，头指针 car_head，头插法插入
 * 文件存储：data/cars.dat（二进制，按 sizeof(Car) 逐条写入）
 */

Car *car_find(const char *id) {
  Car *p = car_head;
  while (p != NULL) {
    if (strcmp(p->id, id) == 0) {
      return p;
    }
    p = p->next;
  }
  return NULL;
}

/* ---- 增删改查 ---- */

/**
 * car_add - 添加轿车记录
 *
 * 实现步骤：
 *   1. 清屏，打印标题
 *   2. 分配新节点 (calloc)
 *   3. 依次读取：编号、型号、颜色、生产厂家、出厂日期、价格
 *   4. 编号查重，已存在则提示并返回
 *   5. 头插法插入链表
 *   6. 提示成功，pause_screen
 */
void car_add(void) { /* TODO: 实现添加轿车 */ }

/**
 * car_delete - 删除轿车记录
 *
 * 实现步骤：
 *   1. 读取要删除的编号
 *   2. 遍历链表找到节点（需记录前驱节点 prev）
 *   3. 打印该记录，要求用户确认 (y/n)
 *   4. 确认后从链表摘除：prev->next = p->next（头节点特殊处理）
 *   5. free 释放内存
 */
void car_delete(void) { /* TODO: 实现删除轿车 */ }

/**
 * car_modify - 修改轿车记录
 *
 * 实现步骤：
 *   1. 读取编号，car_find 查找
 *   2. 未找到则提示返回
 *   3. 打印当前信息
 *   4. 逐字段提示输入新值，直接回车保留原值
 *   5. 可用 safe_gets + strlen 判断是否输入了新值
 */
void car_modify(void) { /* TODO: 实现修改轿车 */ }

/**
 * car_query - 查询轿车记录（子菜单）
 *
 * 支持三种查询方式：
 *   1. 按编号查询 — 调用 car_find，精确匹配
 *   2. 按型号查询 — 遍历链表，strstr 模糊匹配
 *   3. 按价格范围查询 — 读取最低/最高价格，遍历比较 p->price
 *
 * 打印表头后逐条输出匹配记录，无结果提示"未找到"
 */
void car_query(void) {
  char id[MAX_STR];
  printf("请输入查询车辆id:");
  safe_gets(id, sizeof(id));
  Car *p = car_find(id);
  if (p == NULL) {
    printf("\n未查询到此车辆信息\n");
    pause_screen();
    return;
  }
  printf("已查询到如下信息:\n");

  printf("%-8s %-12s %-8s %-14s %-12s %10s\n", "编号", "型号", "颜色",
         "生产厂家", "出厂日期", "价格");
  printf("--------------------------------------------------------------\n");
  printf("%-8s %-12s %-8s %-14s %-12s %10.2f\n", p->id, p->model, p->color,
         p->manufacturer, p->date, p->price);
  pause_screen();
  return;
}

void car_list_all(void) {
  clear_screen();
  if (car_head == NULL) {
    printf("暂无数据\n");
    return;
  }
  printf("%-8s %-12s %-8s %-14s %-12s %10s\n", "编号", "型号", "颜色",
         "生产厂家", "出厂日期", "价格");
  printf("--------------------------------------------------------------\n");
  Car *p = car_head;
  while (p != NULL) {
    printf("%-8s %-12s %-8s %-14s %-12s %10.2f\n", p->id, p->model, p->color,
           p->manufacturer, p->date, p->price);
    p = p->next;
  }
}

/* ---- 菜单 ---- */

/**
 * car_menu - 轿车信息管理子菜单
 *
 * 菜单项：
 *   1. 添加轿车  -> car_add
 *   2. 删除轿车  -> car_delete
 *   3. 修改轿车  -> car_modify
 *   4. 查询轿车  -> car_query
 *   5. 显示全部  -> car_list_all
 *   0. 返回主菜单 -> return
 *
 * 循环显示菜单，read_int 读取选择，switch 分发
 */
void car_menu(void) {
  while (1) {
    int choice = -1;
    clear_screen();
    printf("==========================================\n");
    printf("             车辆信息管理系统\n");
    printf("==========================================\n");
    printf("   1. 添加车辆信息\n");
    printf("   2. 删除车辆信息\n");
    printf("   3. 修改车辆信息\n");
    printf("   4. 查询车辆信息\n");
    printf("   5. 显示全部车辆\n");
    printf("   0. 返回\n");
    printf("==========================================\n");
    printf("请选择: ");

    choice = read_int();
    switch (choice) {
    case 1:
      car_add();
      break;
    case 2:
      car_delete();
      break;
    case 3:
      car_modify();
      break;
    case 4:
      car_query();
      break;
    case 5:
      car_list_all();
      break;
    case 0:
      return;
    default:
      printf("非法输入!\n");
      break;
    }
  }
}

/**
 * car_save - 保存轿车数据到文件
 *
 * 打开 data/cars.dat（wb 模式）
 * 遍历链表，fwrite 每个节点（sizeof(Car)）
 * 注意：next 指针也会被写入，加载时需要重建
 */
void car_save(void) {
  FILE *fp = fopen(DATA_DIR "cars.dat", "wb");
  for (Car *p = car_head; p != NULL; p = p->next)
    fwrite(p, sizeof(Car), 1, fp);
  fclose(fp);
}

/**
 * car_load - 从文件加载轿车数据
 *
 * 打开 data/cars.dat（rb 模式），文件不存在则跳过
 * 循环 fread 到临时变量，malloc 新节点，头插法插入链表
 */
void car_load(void) {
  FILE *fp = fopen(DATA_DIR "cars.dat", "rb");
  if (fp == NULL)
    return;
  Car tmp;
  while (fread(&tmp, sizeof(Car), 1, fp) == 1) {
    Car *new_node = calloc(1, sizeof(Car));
  }
  fclose(fp);
}
