#include "menu.h"
#include "car.h"
#include "employee.h"
#include "customer.h"
#include "sale.h"
#include "data_io.h"
#include "utils.h"

void main_menu(void) {
    int choice;
    while (1) {
        clear_screen();
        printf("==========================================\n");
        printf("       汽车销售信息管理系统 v1.0\n");
        printf("==========================================\n");
        printf("   1. 轿车信息管理\n");
        printf("   2. 员工信息管理\n");
        printf("   3. 客户信息管理\n");
        printf("   4. 销售信息管理\n");
        printf("   5. 保存所有数据\n");
        printf("   0. 退出系统\n");
        printf("==========================================\n");
        printf("请选择: ");

        choice = read_int();
        switch (choice) {
        case 1: car_menu(); break;
        case 2: employee_menu(); break;
        case 3: customer_menu(); break;
        case 4: sale_menu(); break;
        case 5:
            save_all_data();
            printf("所有数据已保存！\n");
            pause_screen();
            break;
        case 0:
            save_all_data();
            printf("数据已保存，感谢使用，再见！\n");
            return;
        default:
            printf("无效选择！\n");
            pause_screen();
        }
    }
}
