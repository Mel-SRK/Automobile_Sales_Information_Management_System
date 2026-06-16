# 汽车销售信息管理系统

C 语言课程设计项目。本仓库为**框架版本**，已完成项目结构、数据结构定义、菜单系统和全部接口规范注释，功能函数留有 `TODO` 待实现。

## 系统功能

| 模块 | 功能 | 查询方式 |
|------|------|----------|
| 轿车信息 | 编号、型号、颜色、生产厂家、出厂日期、价格 | 按编号精确、型号模糊、价格范围 |
| 员工信息 | 员工编号、姓名、性别、年龄、籍贯、学历 | 按编号精确、姓名模糊、按学历筛选 |
| 客户信息 | 客户名称、联系方式、地址、业务联系记录 | 按编号精确、名称模糊、查看完整记录 |
| 销售信息 | 销售日期、汽车类型、颜色、数量、经手人 | 按编号、型号、经手人、日期范围 |
| 销售报表 | 按型号汇总 + 按经手人汇总，屏幕显示 + 文件导出 | — |

## 环境要求

- CMake >= 3.10
- GCC / Clang（支持 C11）
- Linux / macOS / Windows

## 编译运行

```bash
mkdir build && cd build
cmake ..
make
./auto_sales
```

清理编译产物重新构建：

```bash
rm -rf build/ && mkdir build && cd build
cmake .. && make
```

## 项目结构

```
.
├── CMakeLists.txt              # 构建配置
├── README.md
├── include/                    # 头文件（接口声明 + 数据结构定义）
│   ├── utils.h                 # 公共宏定义 + 工具函数
│   ├── car.h                   # 轿车数据结构 + 接口声明
│   ├── employee.h              # 员工数据结构 + 接口声明
│   ├── customer.h              # 客户数据结构 + 接口声明
│   ├── sale.h                  # 销售数据结构 + 接口声明
│   ├── data_io.h               # 数据读写汇总接口
│   └── menu.h                  # 主菜单接口
├── src/                        # 源文件
│   ├── main.c                  # 程序入口（已实现）
│   ├── menu.c                  # 主菜单循环（已实现）
│   ├── utils.c                 # 工具函数（已实现）
│   ├── car.c                   # 轿车模块（TODO × 9）
│   ├── employee.c              # 员工模块（TODO × 10）
│   ├── customer.c              # 客户模块（TODO × 10）
│   ├── sale.c                  # 销售模块（TODO × 9）
│   └── data_io.c               # 文件读写（TODO × 2）
└── data/                       # 运行时数据目录（自动创建）
    ├── cars.dat
    ├── employees.dat
    ├── customers.dat
    ├── sales.dat
    └── report.txt
```

## 系统架构

```
main.c ──> menu.c ──┬──> car.c      ──┐
                    ├──> employee.c ──┤──> utils.h（公共宏 + 工具函数）
                    ├──> customer.c ──┤
                    └──> sale.c ──────┘
                              │
                              ▼
                         data_io.c  ──> data/*.dat（二进制文件）
```

- 数据结构：单向链表，每个模块独立管理链表头指针
- 存储方式：fwrite/fread 二进制逐条读写，加载时头插法重建链表
- 销售模块通过编号关联轿车、员工、客户，冗余存储名称字段

## 菜单结构

```
==========================================
       汽车销售信息管理系统 v1.0
==========================================
   1. 轿车信息管理
   2. 员工信息管理
   3. 客户信息管理
   4. 销售信息管理
   5. 保存所有数据
   0. 退出系统
==========================================

轿车信息管理          员工信息管理          客户信息管理          销售信息管理
├─ 1. 添加轿车       ├─ 1. 添加员工       ├─ 1. 添加客户       ├─ 1. 添加销售记录
├─ 2. 删除轿车       ├─ 2. 删除员工       ├─ 2. 删除客户       ├─ 2. 删除销售记录
├─ 3. 修改轿车       ├─ 3. 修改员工       ├─ 3. 修改客户       ├─ 3. 查询销售记录
├─ 4. 查询轿车       ├─ 4. 查询员工       ├─ 4. 查询客户       ├─ 4. 显示全部记录
├─ 5. 显示全部       ├─ 5. 显示全部       ├─ 5. 显示全部       ├─ 5. 打印销售报表
└─ 0. 返回主菜单     └─ 0. 返回主菜单     ├─ 6. 追加联系记录   └─ 0. 返回主菜单
                                          └─ 0. 返回主菜单
```

## 公共定义

### 宏常量

| 宏 | 值 | 说明 |
|----|----|------|
| `MAX_STR` | 64 | 一般字符串字段最大长度 |
| `MAX_LONG` | 256 | 长文本字段（地址、联系记录）最大长度 |
| `DATA_DIR` | `"data/"` | 数据文件存储目录 |

### 工具函数（utils.h，已实现）

| 函数 | 功能 |
|------|------|
| `void clear_screen(void)` | 清屏，Windows/Linux 自适应 |
| `void pause_screen(void)` | 打印提示并等待回车 |
| `void safe_gets(char *buf, int size)` | 安全读取一行，去末尾换行 |
| `int read_int(void)` | 读一行转整数，失败返回 0 |
| `void get_current_date(char *buf, int size)` | 获取当前日期 YYYY-MM-DD |

---

## 数据结构与接口规范

### 1. 轿车信息模块 (car.h / car.c)

#### 数据结构

```c
typedef struct Car {
    char id[MAX_STR];           // 编号（主键，用户输入）
    char model[MAX_STR];        // 型号
    char color[MAX_STR];        // 颜色
    char manufacturer[MAX_STR]; // 生产厂家
    char date[MAX_STR];         // 出厂日期 (YYYY-MM-DD)
    double price;               // 价格
    struct Car *next;           // 链表指针
} Car;
extern Car *car_head;
```

#### 接口函数

| 函数 | 功能 | 说明 |
|------|------|------|
| `Car *car_find(const char *id)` | 按编号查找 | 遍历链表 strcmp，返回指针或 NULL |
| `void car_add(void)` | 添加 | 编号查重 → 逐字段输入 → 头插法 |
| `void car_delete(void)` | 删除 | 查找 → 确认 → 摘链 → free |
| `void car_modify(void)` | 修改 | 查找 → 逐字段修改，回车保留原值 |
| `void car_query(void)` | 查询 | 子菜单：编号精确/型号模糊/价格范围 |
| `void car_list_all(void)` | 列表 | 统计数量 → 表头 → 遍历打印 |
| `void car_menu(void)` | 子菜单 | 循环 1-5+0 分发 |
| `void car_save(void)` | 保存 | fwrite 到 data/cars.dat |
| `void car_load(void)` | 加载 | fread + 头插法重建链表 |

---

### 2. 员工信息模块 (employee.h / employee.c)

#### 数据结构

```c
typedef enum {
    EDU_HIGH_SCHOOL = 1,   // 高中
    EDU_COLLEGE,           // 大专
    EDU_BACHELOR,          // 本科
    EDU_MASTER,            // 硕士
    EDU_PHD                // 博士
} Education;

typedef struct Employee {
    char id[MAX_STR];       // 编号（主键）
    char name[MAX_STR];     // 姓名
    char gender[4];         // 性别 男/女
    int age;                // 年龄
    char hometown[MAX_STR]; // 籍贯
    Education education;    // 学历枚举
    struct Employee *next;
} Employee;
extern Employee *emp_head;
```

#### 接口函数

| 函数 | 功能 | 说明 |
|------|------|------|
| `Employee *emp_find(const char *id)` | 按编号查找 | 返回指针或 NULL |
| `const char *edu_to_str(Education e)` | 学历转中文 | 枚举 → "本科" 等 |
| `void emp_add(void)` | 添加 | 输入 1-5 选择学历 |
| `void emp_delete(void)` | 删除 | 查找 → 确认 → 摘链 |
| `void emp_modify(void)` | 修改 | 回车保留原值 |
| `void emp_query(void)` | 查询 | 编号精确/姓名模糊/学历筛选 |
| `void emp_list_all(void)` | 列表 | 遍历打印 |
| `void employee_menu(void)` | 子菜单 | 1-5+0 |
| `void emp_save(void)` / `void emp_load(void)` | 文件读写 | data/employees.dat |

---

### 3. 客户信息模块 (customer.h / customer.c)

#### 数据结构

```c
typedef struct Customer {
    char id[MAX_STR];             // 编号（自动生成 C0001 格式）
    char name[MAX_STR];           // 名称
    char phone[MAX_STR];          // 联系方式
    char address[MAX_LONG];       // 地址
    char contact_log[MAX_LONG];   // 业务联系记录（可追加）
    struct Customer *next;
} Customer;
extern Customer *cust_head;
```

#### 接口函数

| 函数 | 功能 | 说明 |
|------|------|------|
| `Customer *cust_find(const char *id)` | 按编号查找 | 返回指针或 NULL |
| `void cust_add(void)` | 添加 | 编号自动生成，不需用户输入 |
| `void cust_delete(void)` | 删除 | 查找 → 确认 → 摘链 |
| `void cust_modify(void)` | 修改 | 名称/联系方式/地址，回车保留 |
| `void cust_query(void)` | 查询 | 编号精确/名称模糊/完整信息(含记录) |
| `void cust_list_all(void)` | 列表 | 遍历打印 |
| `void cust_add_contact_log(void)` | 追加联系记录 | 格式 `[YYYY-MM-DD] 内容; `，strcat 追加 |
| `void customer_menu(void)` | 子菜单 | 1-6+0 |
| `void cust_save(void)` / `void cust_load(void)` | 文件读写 | data/customers.dat |

---

### 4. 销售信息模块 (sale.h / sale.c)

#### 数据结构

```c
typedef struct Sale {
    char id[MAX_STR];           // 编号（自动生成 S0001 格式）
    char date[MAX_STR];         // 销售日期
    char car_id[MAX_STR];       // 汽车编号 → 关联 Car.id
    char car_model[MAX_STR];    // 型号（冗余，从 Car 查出）
    char color[MAX_STR];        // 颜色（冗余）
    int quantity;               // 数量
    char emp_id[MAX_STR];       // 经手人编号 → 关联 Employee.id
    char emp_name[MAX_STR];     // 经手人姓名（冗余）
    char cust_id[MAX_STR];      // 客户编号 → 关联 Customer.id
    char cust_name[MAX_STR];    // 客户名称（冗余）
    double total_price;         // 总价 = 单价 × 数量
    struct Sale *next;
} Sale;
extern Sale *sale_head;
```

#### 接口函数

| 函数 | 功能 | 说明 |
|------|------|------|
| `Sale *sale_find(const char *id)` | 按编号查找 | 返回指针或 NULL |
| `void sale_add(void)` | 添加 | 关联三表自动填充，未找到降级手动输入 |
| `void sale_delete(void)` | 删除 | 查找 → 确认 → 摘链 |
| `void sale_query(void)` | 查询 | 编号/型号/经手人/日期范围(字符串比较) |
| `void sale_list_all(void)` | 列表 | 遍历打印 |
| `void sale_report(void)` | 销售报表 | 按型号统计 + 按经手人统计 + 导出 report.txt |
| `void sale_menu(void)` | 子菜单 | 1-5+0 |
| `void sale_save(void)` / `void sale_load(void)` | 文件读写 | data/sales.dat |

#### 关联逻辑（sale_add 核心）

```
输入汽车编号 → car_find() → 自动填充型号、颜色、单价
输入员工编号 → emp_find() → 自动填充经手人姓名
输入客户编号 → cust_find() → 自动填充客户名称
总价 = car->price × quantity（找不到车则手动输入单价）
```

#### 报表输出格式

```
【按型号统计】
型号              销售数量       销售金额
-------------------------------------------
帕萨特                 2       360000.00
...
-------------------------------------------
合计                   5       780000.00

【按经手人统计】
经手人        销售数量       销售金额
-------------------------------------------
张三               3       480000.00
...
```

同时导出到 `data/report.txt`。

---

### 5. 数据读写模块 (data_io.h / data_io.c)

| 函数 | 功能 |
|------|------|
| `void load_all_data(void)` | 启动时加载全部四类数据，文件不存在则跳过 |
| `void save_all_data(void)` | 保存全部四类数据，退出时自动调用 |

#### 文件格式

| 文件 | 格式 | 说明 |
|------|------|------|
| `data/cars.dat` | 二进制 | fwrite sizeof(Car) 逐条写入 |
| `data/employees.dat` | 二进制 | 同上 |
| `data/customers.dat` | 二进制 | 同上 |
| `data/sales.dat` | 二进制 | 同上 |
| `data/report.txt` | 文本 | 覆盖写入，fprintf |

## 使用流程示例

```
1. 轿车管理 → 添加：A001, 帕萨特, 黑色, 大众, 2024-01-15, 180000
2. 员工管理 → 添加：E001, 张三, 男, 28, 北京, 本科
3. 客户管理 → 添加：李四, 13800138000, 上海市..., 首次联系...
4. 销售管理 → 添加：
   汽车编号 A001 → 自动显示 帕萨特/黑色/180000
   数量 2
   员工编号 E001 → 自动显示 张三
   客户编号 C0001 → 自动显示 李四
   总价自动算出 360000
5. 销售报表 → 按型号/经手人汇总，导出 report.txt
```
