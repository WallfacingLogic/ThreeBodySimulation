# ThreeBodySimulation

一个基于 C++ 的高精度三体问题数值模拟程序，使用经典牛顿万有引力定律进行多体轨道演化计算。

## 📋 项目简介

本项目旨在演示和研究经典牛顿引力下的三体动力学问题，适用于教学、实验与可视化场景。程序支持多种数值积分方法和高精度浮点运算，能够准确模拟天体系统的长期演化行为。

### 核心特性

- ✨ **高精度计算**：基于 MPFR 库实现任意精度浮点运算（默认 256 位）
- 🔬 **多种积分器**：支持辛算法（Symplectic）和龙格-库塔 4 阶（RK4）方法
- 🎯 **自适应步长**：可选的自适应步长控制以提高计算效率和精度
- 💾 **数据导出**：支持将轨道数据导出为 CSV 格式，便于后续可视化和分析
- 🌍 **预设示例**：内置太阳-地球-月球系统、等质量三体系统和混沌三体系统
- ⚙️ **灵活配置**：支持自定义初始条件、时间步长和总模拟时长

## 🏗️ 系统架构

```
ThreeBodySimulation/
├── include/              # 头文件目录
│   ├── Constants.h       # 物理常量和配置参数
│   ├── ThreeBodySolver.h # 求解器类声明
│   └── Vector3.h         # 三维向量数学类
├── src/                  # 源代码目录
│   ├── ThreeBodySolver.cpp # 求解器实现
│   └── main.cpp          # 程序入口及交互界面
├── bin/                  # 编译输出的可执行文件
├── CMakeLists.txt        # CMake 构建配置
└── README.md             # 项目说明文档
```

### 主要组件

- **`ThreeBodySolver`**：核心求解器类，负责天体状态管理、数值积分和能量守恒验证
- **`Vector3`**：三维向量类，提供位置、速度、加速度的向量运算
- **`Constants`**：定义物理常量（如万有引力常数）和默认配置参数
- **`CelestialBody`**：天体结构体，存储名称、质量、位置、速度和加速度
- **`SimulationResult`**：模拟结果结构体，记录每个时间步的系统状态

## 🛠️ 技术栈

- **编程语言**：C++17
- **构建工具**：CMake 3.15+
- **编译器**：GCC / Clang / MSVC
- **核心依赖**：
  - [MPFR](https://www.mpfr.org/) - 高精度浮点运算库
  - [GMP](https://gmplib.org/) - GNU 多精度算术库（MPFR 的底层依赖）
- **辅助工具**：pkg-config（用于查找 MPFR 库）

## 📦 安装依赖

### Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install build-essential cmake pkg-config libmpfr-dev libgmp-dev
```

### macOS (Homebrew)

```bash
brew install cmake mpfr gmp pkg-config
```

### Windows

#### 方式 1：使用 MSYS2

```bash
# 安装 MSYS2 后，在 MSYS2 MinGW64 终端中运行
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-mpfr mingw-w64-x86_64-gmp
```

#### 方式 2：手动编译 GMP/MPFR

对于 MinGW 交叉编译，需要从源码编译 GMP 和 MPFR 库并安装到自定义目录（如 `/opt/mingw-libs`）。

## 🔨 构建项目

### 使用 CMake（推荐）

```bash
# 克隆或进入项目目录
cd ThreeBodySimulation

# 创建构建目录
mkdir build && cd build

# 配置项目
cmake .. -DCMAKE_BUILD_TYPE=Release

# 编译项目
cmake --build . --config Release
```

编译成功后，可执行文件将生成在 `bin/` 目录下。

### 使用 Visual Studio（Windows）

1. 打开 `ThreeBodySimulation.vcxproj` 文件
2. 选择 Debug/Release 配置及 x86/x64 平台
3. 点击"生成" (Build)
4. 按 F5 运行或从输出目录运行可执行文件

### 清理构建

```bash
rm -rf build/
```

## 🚀 运行程序

```bash
# Linux/macOS
./bin/ThreeBodySimulation

# Windows
.\bin\ThreeBodySimulation.exe
```

### 交互式使用流程

程序启动后会引导你完成以下步骤：

1. **选择示例场景**：
   - `1` - 太阳-地球-月球系统
   - `2` - 等质量三体系统（拉格朗日点配置）
   - `3` - 混沌三体系统
   - `4` - 自定义输入

2. **设置模拟参数**：
   - 时间步长（秒），默认 3600（1 小时）
   - 总模拟时长（秒），默认 31536000（1 年）
   - 是否使用辛积分器（推荐：是）
   - 是否使用自适应步长（默认：否）

3. **查看实时反馈**：
   - 初始总能量显示
   - 每 100 步输出一次进度
   - 能量守恒验证结果

4. **导出数据**：
   - 输入输出文件名（默认：`three_body_results.csv`）
   - CSV 文件包含时间与所有天体的位置坐标

### 示例运行

```
=========================================
  Scientific Three-Body Simulation
=========================================

Select example:
1. Sun-Earth-Moon System
2. Equal Mass Three-Body System
3. Chaotic Three-Body System
4. Custom Input

Enter choice (1-4): 1

=== Example 1: Sun-Earth-Moon System ===

Simulation Parameters:
Time step (s) [default: 3600]: 
Total simulation time (s) [default: 31536000 = 1 year]: 
Use symplectic integrator? (1=Yes, 0=No) [default: 1]: 
Use adaptive step size? (1=Yes, 0=No) [default: 0]: 

Initial Total Energy: -2.651234567890123e+33 J

Starting simulation...
[进度输出...]

Energy conservation verified: PASSED
Relative energy error: 1.23e-12

Enter output filename (e.g., results.csv): sun_earth_moon.csv

Results saved to: sun_earth_moon.csv
```

## 📊 输出数据格式

CSV 文件包含以下列：

| 列名 | 描述 | 单位 |
|------|------|------|
| Time | 模拟时间 | 秒 (s) |
| Body_Name | 天体名称 | - |
| Position_X | X 坐标 | 米 (m) |
| Position_Y | Y 坐标 | 米 (m) |
| Position_Z | Z 坐标 | 米 (m) |
| Velocity_X | X 方向速度 | 米/秒 (m/s) |
| Velocity_Y | Y 方向速度 | 米/秒 (m/s) |
| Velocity_Z | Z 方向速度 | 米/秒 (m/s) |
| Total_Energy | 系统总能量 | 焦耳 (J) |

### 数据可视化示例（Python）

```python
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# 读取数据
df = pd.read_csv('sun_earth_moon.csv')

# 绘制地球轨道
earth = df[df['Body_Name'] == 'Earth']
plt.figure(figsize=(10, 10))
plt.plot(earth['Position_X'], earth['Position_Y'], label='Earth Orbit')
plt.xlabel('X Position (m)')
plt.ylabel('Y Position (m)')
plt.title('Earth Orbit around Sun')
plt.legend()
plt.grid(True)
plt.axis('equal')
plt.show()
```

## 🔬 数值方法

### 积分算法

1. **辛算法（Symplectic Integrator）**
   - 保持哈密顿系统的几何结构
   - 长期模拟中能量守恒性更好
   - 适合天体力学长时间演化

2. **龙格-库塔 4 阶（RK4）**
   - 经典四阶精度方法
   - 短期模拟精度高
   - 计算成本相对较高

### 精度控制

- **固定步长**：用户指定的恒定时间步长
- **自适应步长**：根据局部截断误差动态调整步长
  - 误差估计基于嵌入方法
  - 自动平衡精度与计算效率

### 能量守恒验证

程序会自动计算并验证系统总能量守恒：

```
相对能量误差 = |E_final - E_initial| / |E_initial|
```

通常要求相对误差 < 10⁻¹⁰（可通过 `Constants::ENERGY_TOLERANCE` 配置）。

## 🧪 预设示例详解

### 示例 1：太阳-地球-月球系统

真实天文数据简化模型：
- **太阳**：质量 1.989×10³⁰ kg，位于原点
- **地球**：质量 5.972×10²⁴ kg，距离太阳 1.496×10¹¹ m（1 AU）
- **月球**：质量 7.348×10²² kg，距离地球 3.844×10⁸ m

适用于研究地月系统稳定性和摄动效应。

### 示例 2：等质量三体系统

三个等质量天体（各 10³⁰ kg）初始位于等边三角形顶点，具有特定初速度以形成周期性轨道。这是经典的拉格朗日解，用于验证数值方法的准确性。

### 示例 3：混沌三体系统

三个等质量恒星的非对称初始配置，展示三体问题的混沌特性。微小初始扰动会导致完全不同的长期演化，适合研究混沌动力学。

## ⚙️ API 使用指南

### 编程接口示例

```cpp
#include "ThreeBodySolver.h"

int main() {
    // 创建求解器（256 位精度）
    ThreeBodySolver solver(256);
    
    // 添加天体
    solver.add_body("Star1", 1.0e30, 
                    0.0, 0.0, 0.0,      // 位置 (x, y, z)
                    0.0, 0.0, 0.0);     // 速度 (vx, vy, vz)
    
    solver.add_body("Star2", 1.0e30,
                    1.0e11, 0.0, 0.0,
                    0.0, 40000.0, 0.0);
    
    solver.add_body("Star3", 1.0e30,
                    0.0, 2.0e11, 0.0,
                    30000.0, 0.0, 0.0);
    
    // 设置模拟参数
    double time_step = 3600.0;      // 1 小时
    double total_time = 31536000.0; // 1 年
    bool use_symplectic = true;
    bool adaptive_step = false;
    
    solver.set_simulation_params(time_step, total_time, 
                                 use_symplectic, adaptive_step);
    
    // 运行模拟（每 100 步输出一次）
    auto results = solver.run_simulation(100);
    
    // 验证能量守恒
    solver.verify_energy_conservation(results);
    
    // 导出结果
    solver.export_to_csv(results, "results.csv");
    
    return 0;
}
```

### 主要 API

| 方法 | 描述 |
|------|------|
| `ThreeBodySolver(int precision)` | 构造函数，指定精度位数 |
| `add_body(...)` | 添加天体到系统 |
| `set_simulation_params(...)` | 设置时间步长、总时长、积分器类型 |
| `run_simulation(int interval)` | 运行模拟，返回结果向量 |
| `export_to_csv(results, filename)` | 导出结果为 CSV |
| `verify_energy_conservation(results)` | 验证能量守恒 |
| `get_initial_energy()` | 获取初始总能量 |

## 🐛 常见问题

### Q1: 编译时找不到 MPFR 库

**解决方案**：
```bash
# Ubuntu/Debian
sudo apt-get install libmpfr-dev libgmp-dev

# 检查 pkg-config 是否能找到
pkg-config --cflags --libs mpfr
```

### Q2: 运行时提示缺少动态库

**解决方案**：
```bash
# Linux: 更新动态库缓存
sudo ldconfig

# 或者设置 LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

### Q3: 模拟结果不稳定或发散

**建议**：
- 减小时间步长（例如从 3600 改为 600）
- 启用辛积分器（更适合长期模拟）
- 启用自适应步长
- 检查初始条件是否合理（避免天体过于接近）

### Q4: 如何提高计算精度？

**方法**：
- 增加 MPFR 精度位数（构造函数参数，如 512 或 1024）
- 使用更小的时间步长
- 启用自适应步长控制
- 选择辛积分器而非 RK4

## 📝 许可证

本项目采用 MIT 许可证。详见 [LICENSE](LICENSE) 文件。

## 🤝 贡献指南

欢迎提交 Issue 和 Pull Request！

1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

## 📚 参考资料

- [三体问题 - Wikipedia](https://en.wikipedia.org/wiki/Three-body_problem)
- [MPFR Library](https://www.mpfr.org/)
- [GMP Library](https://gmplib.org/)
- Hairer, E., Lubich, C., & Wanner, G. (2006). *Geometric Numerical Integration*. Springer.