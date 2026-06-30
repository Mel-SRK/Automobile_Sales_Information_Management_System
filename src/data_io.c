#include "data_io.h"
#include "car.h"
#include "customer.h"
#include "employee.h"
#include "sale.h"

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

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

static void ensure_data_dir(void) {
#ifdef _WIN32
  _mkdir("data");
#else
  mkdir("data", 0755);
#endif
}

void load_all_data(void) {
  ensure_data_dir();
  car_load();
  emp_load();
  cust_load();
  sale_load();
}

void save_all_data(void) {
  ensure_data_dir();
  car_save();
  emp_save();
  cust_save();
  sale_save();
}
