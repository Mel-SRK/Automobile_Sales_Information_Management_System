#include "car.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void car_add(void) {
  clear_screen();
  char buf[MAX_STR];
  Car *new_node = calloc(1, sizeof(Car));
  printf("编号: ");
  safe_gets(new_node->id, sizeof(new_node->id));
  if (car_find(new_node->id) != NULL) {
    printf("编号已存在\n");
    pause_screen();
    return;
  }
  printf("型号: ");
  safe_gets(new_node->model, sizeof(new_node->model));
  printf("颜色: ");
  safe_gets(new_node->color, sizeof(new_node->color));
  printf("厂商: ");
  safe_gets(new_node->manufacturer, sizeof(new_node->manufacturer));
  printf("出厂日期(默认当日): ");
  safe_gets(new_node->date, sizeof(new_node->date));
  if (new_node->date[0] == '\0') {
    get_current_date(new_node->date, sizeof(new_node->date));
  }
  printf("价格: ");
  safe_gets(buf, sizeof(buf));
  new_node->price = atof(buf);
  new_node->next = car_head;
  car_head = new_node;
  printf("添加成功\n");
  pause_screen();
  return;
}

void car_delete(void) {
  printf("需要删除的编号: ");
  char id[MAX_STR] = {0};
  safe_gets(id, sizeof(id));
  if (id[0] == '\0') {
    printf("请输入编号\n");
    pause_screen();
    return;
  }
  Car *p = car_head;
  Car *prev = NULL;

  while (p != NULL) {
    if (strcmp(p->id, id) == 0) {
      break;
    }
    prev = p;
    p = p->next;
  }
  if (p == NULL) {
    printf("未找到此编号\n");
    pause_screen();
    return;
  }

  printf("%-10s %-14s %-8s %-14s %-16s %12s\n", "编号", "型号", "颜色",
         "生产厂家", "出厂日期", "价格");
  printf("--------------------------------------------------------------\n");
  printf("%-8s %-12s %-8s %-14s %-12s %10.2f\n", p->id, p->model, p->color,
         p->manufacturer, p->date, p->price);
  printf("确认删除此信息吗？ (y/n): ");
  char confirm[4] = {0};
  safe_gets(confirm, sizeof(confirm));
  if (confirm[0] != 'y' && confirm[0] != '\0') {
    printf("已取消...\n");
    pause_screen();
    return;
  }
  if (prev == NULL) {
    car_head = p->next;
  } else {
    prev->next = p->next;
  }
  free(p);
  printf("已删除\n");
  pause_screen();
  return;
}

void car_modify(void) {
  char id[MAX_STR] = {0};
  printf("需要修改车辆编号: ");
  safe_gets(id, sizeof(id));
  if (id[0] == '\0') {
    printf("请输入被修改车辆编号(id)\n");
    pause_screen();
    return;
  }
  Car *p = car_find(id);
  if (p == NULL) {
    printf("未查询到相关车辆信息\n");
    pause_screen();
    return;
  }
  printf("%-10s %-14s %-8s %-14s %-16s %12s\n", "编号", "型号", "颜色",
         "生产厂家", "出厂日期", "价格");
  printf("--------------------------------------------------------------\n");
  printf("%-8s %-12s %-8s %-14s %-12s %10.2f\n", p->id, p->model, p->color,
         p->manufacturer, p->date, p->price);
  char buf[MAX_STR] = {0};

  printf("请输入新的型号(当前: %s, 回车跳过): ", p->model);
  safe_gets(buf, sizeof(buf));
  if (strlen(buf) != 0) {
    strcpy(p->model, buf);
  }

  printf("请输入新的颜色(当前: %s, 回车跳过): ", p->color);
  safe_gets(buf, sizeof(buf));
  if (strlen(buf) != 0) {
    strcpy(p->color, buf);
  }

  printf("请输入新的厂家(当前: %s, 回车跳过): ", p->manufacturer);
  safe_gets(buf, sizeof(buf));
  if (strlen(buf) != 0) {
    strcpy(p->manufacturer, buf);
  }

  printf("请输入新的出厂日期(当前: %s, 回车跳过): ", p->date);
  safe_gets(buf, sizeof(buf));
  if (strlen(buf) != 0) {
    strcpy(p->date, buf);
  }

  printf("请输入新的价格(当前: %.2f, 回车跳过): ", p->price);
  safe_gets(buf, sizeof(buf));
  if (strlen(buf) != 0) {
    p->price = atof(buf);
  }

  printf("修改成功\n");
  pause_screen();
}

static void car_print_header(void) {
  printf("%-10s %-14s %-8s %-14s %-16s %12s\n", "编号", "型号", "颜色",
         "生产厂家", "出厂日期", "价格");
  printf("--------------------------------------------------------------\n");
}

static void car_print_one(Car *p) {
  printf("%-8s %-12s %-8s %-14s %-12s %10.2f\n", p->id, p->model, p->color,
         p->manufacturer, p->date, p->price);
}

void car_query(void) {
  while (1) {
    clear_screen();
    printf("========== 查询车辆信息 ==========\n");
    printf("   1. 按编号查询\n");
    printf("   2. 按型号模糊查询\n");
    printf("   3. 按价格范围查询\n");
    printf("   0. 返回\n");
    printf("==================================\n");
    printf("请选择: ");
    int choice = read_int();

    if (choice == 0)
      return;

    if (choice == 1) {
      char id[MAX_STR];
      printf("请输入编号: ");
      safe_gets(id, sizeof(id));
      Car *p = car_find(id);
      if (p == NULL) {
        printf("未找到\n");
        pause_screen();
        continue;
      }
      car_print_header();
      car_print_one(p);
      pause_screen();

    } else if (choice == 2) {
      char keyword[MAX_STR];
      printf("请输入型号关键词: ");
      safe_gets(keyword, sizeof(keyword));
      int count = 0;
      car_print_header();
      for (Car *p = car_head; p != NULL; p = p->next) {
        if (strcasestr(p->model, keyword) != NULL) {
          car_print_one(p);
          count++;
        }
      }
      if (count == 0)
        printf("未找到匹配记录\n");
      else
        printf("共 %d 条记录\n", count);
      pause_screen();

    } else if (choice == 3) {
      char buf[MAX_STR];
      printf("最低价格: ");
      safe_gets(buf, sizeof(buf));
      double min_price = atof(buf);
      printf("最高价格: ");
      safe_gets(buf, sizeof(buf));
      double max_price = atof(buf);
      int count = 0;
      car_print_header();
      for (Car *p = car_head; p != NULL; p = p->next) {
        if (p->price >= min_price && p->price <= max_price) {
          car_print_one(p);
          count++;
        }
      }
      if (count == 0)
        printf("未找到匹配记录\n");
      else
        printf("共 %d 条记录\n", count);
      pause_screen();

    } else {
      printf("无效选择\n");
      pause_screen();
    }
  }
}

void car_list_all(void) {
  clear_screen();
  if (car_head == NULL) {
    printf("暂无数据\n");
    pause_screen();
    return;
  }
  printf("%-10s %-14s %-8s %-14s %-16s %12s\n", "编号", "型号", "颜色",
         "生产厂家", "出厂日期", "价格");
  printf("--------------------------------------------------------------\n");
  Car *p = car_head;
  while (p != NULL) {
    printf("%-8s %-12s %-8s %-14s %-12s %10.2f\n", p->id, p->model, p->color,
           p->manufacturer, p->date, p->price);
    p = p->next;
  }
  pause_screen();
}

/* ---- 菜单 ---- */
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

void car_save(void) {
  FILE *fp = fopen(DATA_DIR "cars.dat", "wb");
  for (Car *p = car_head; p != NULL; p = p->next)
    fwrite(p, sizeof(Car), 1, fp);
  fclose(fp);
}

void car_load(void) {
  FILE *fp = fopen(DATA_DIR "cars.dat", "rb");
  if (fp == NULL)
    return;
  Car tmp;
  while (fread(&tmp, sizeof(Car), 1, fp) == 1) {
    Car *new_node = calloc(1, sizeof(Car));
    *new_node = tmp;
    new_node->next = car_head;
    car_head = new_node;
  }
  fclose(fp);
}
