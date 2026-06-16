#include "data_io.h"
#include "car.h"
#include "employee.h"
#include "customer.h"
#include "sale.h"

/*
 * 数据读写模块
 * ============
 * 负责启动时加载、退出时保存全部四类数据
 * 文件格式：二进制（fwrite/fread，按结构体大小逐条读写）
 * 注意事项：
 *   - 链表的 next 指针不序列化，加载时头插法重建
 *   - 文件不存在时 load 函数应静默跳过，不报错
 *   - save 前确保 data/ 目录存在
 */

/**
 * load_all_data - 启动时加载全部数据
 *
 * 实现步骤：
 *   1. 确保 data/ 目录存在：system("mkdir -p data/")
 *   2. 依次调用 car_load, emp_load, cust_load, sale_load
 *
 * 调用时机：main 函数入口，进入菜单之前
 */
void load_all_data(void)
{
    /* TODO: 依次调用四个模块的 load 函数 */
}

/**
 * save_all_data - 保存全部数据
 *
 * 依次调用 car_save, emp_save, cust_save, sale_save
 * 调用时机：用户选择"保存数据"或退出系统时
 */
void save_all_data(void)
{
    /* TODO: 依次调用四个模块的 save 函数 */
}
