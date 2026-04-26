#include "ThreeBodySolver.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>

// 示例1：太阳-地球-月球系统
void example_solar_system(ThreeBodySolver &solver)
{
    std::cout << "\n=== Example 1: Sun-Earth-Moon System ===" << std::endl;

    // 太阳 (质量: 1.989e30 kg)
    solver.add_body("Sun", 1.989e30, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

    // 地球 (质量: 5.972e24 kg, 距离太阳约1.496e11 m)
    solver.add_body("Earth", 5.972e24, 1.496e11, 0.0, 0.0, 0.0, 29780.0, 0.0);

    // 月球 (质量: 7.348e22 kg, 距离地球约3.844e8 m)
    solver.add_body("Moon", 7.348e22, 1.496e11 + 3.844e8, 0.0, 0.0, 0.0, 29780.0 + 1022.0, 0.0);
}

// 示例2：等质量三体系统（拉格朗日点配置）
void example_equal_mass(ThreeBodySolver &solver)
{
    std::cout << "\n=== Example 2: Equal Mass Three-Body System ===" << std::endl;

    double mass = 1.0e30;
    double radius = 1.0e11;

    // 三个等质量天体在等边三角形顶点
    solver.add_body("Body1", mass, radius, 0.0, 0.0, 0.0, 30000.0, 0.0);
    solver.add_body("Body2", mass, -radius * 0.5, radius * std::sqrt(3.0) * 0.5, 0.0,
                    -15000.0, -25980.0, 0.0);
    solver.add_body("Body3", mass, -radius * 0.5, -radius * std::sqrt(3.0) * 0.5, 0.0,
                    -15000.0, 25980.0, 0.0);
}

// 示例3：混沌三体系统
void example_chaotic(ThreeBodySolver &solver)
{
    std::cout << "\n=== Example 3: Chaotic Three-Body System ===" << std::endl;

    double mass = 1.0e30;
    solver.add_body("Star1", mass, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    solver.add_body("Star2", mass, 1.0e11, 0.0, 0.0, 0.0, 40000.0, 0.0);
    solver.add_body("Star3", mass, 0.0, 2.0e11, 0.0, 30000.0, 0.0, 0.0);
}

int main()
{
    std::cout << "=========================================" << std::endl;
    std::cout << "  Scientific Three-Body Simulation" << std::endl;
    std::cout << "=========================================" << std::endl;

    // 创建求解器（使用256位精度）
    ThreeBodySolver solver(256);

    // 选择示例
    std::cout << "\nSelect example:" << std::endl;
    std::cout << "1. Sun-Earth-Moon System" << std::endl;
    std::cout << "2. Equal Mass Three-Body System" << std::endl;
    std::cout << "3. Chaotic Three-Body System" << std::endl;
    std::cout << "4. Custom Input" << std::endl;
    std::cout << "\nEnter choice (1-4): ";

    int choice;
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        example_solar_system(solver);
        break;
    case 2:
        example_equal_mass(solver);
        break;
    case 3:
        example_chaotic(solver);
        break;
    case 4:
        // 自定义输入
        std::cout << "\nEnter parameters for three bodies:" << std::endl;
        for (int i = 1; i <= 3; ++i)
        {
            std::string name;
            double mass, x, y, z, vx, vy, vz;

            std::cout << "\nBody " << i << ":" << std::endl;
            std::cout << "  Name: ";
            std::cin >> name;
            std::cout << "  Mass (kg): ";
            std::cin >> mass;
            std::cout << "  Position X (m): ";
            std::cin >> x;
            std::cout << "  Position Y (m): ";
            std::cin >> y;
            std::cout << "  Position Z (m): ";
            std::cin >> z;
            std::cout << "  Velocity VX (m/s): ";
            std::cin >> vx;
            std::cout << "  Velocity VY (m/s): ";
            std::cin >> vy;
            std::cout << "  Velocity VZ (m/s): ";
            std::cin >> vz;

            solver.add_body(name, mass, x, y, z, vx, vy, vz);
        }
        break;
    default:
        std::cout << "Invalid choice. Using default example." << std::endl;
        example_solar_system(solver);
    }

    // 设置模拟参数
    double time_step, total_time;
    bool use_symplectic = true;
    bool adaptive_step = false;

    std::cout << "\nSimulation Parameters:" << std::endl;
    std::cout << "Time step (s) [default: 3600]: ";
    if (!(std::cin >> time_step))
    {
        time_step = 3600.0; // 默认1小时
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Total simulation time (s) [default: 31536000 = 1 year]: ";
    if (!(std::cin >> total_time))
    {
        total_time = 31536000.0;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Use symplectic integrator? (1=Yes, 0=No) [default: 1]: ";
    int symplectic_choice;
    if (std::cin >> symplectic_choice)
    {
        use_symplectic = (symplectic_choice == 1);
    }

    std::cout << "Use adaptive step size? (1=Yes, 0=No) [default: 0]: ";
    int adaptive_choice;
    if (std::cin >> adaptive_choice)
    {
        adaptive_step = (adaptive_choice == 1);
    }

    solver.set_simulation_params(time_step, total_time, use_symplectic, adaptive_step);

    // 显示初始能量
    double initial_energy = solver.get_initial_energy();
    std::cout << "\nInitial Total Energy: " << std::scientific
              << std::setprecision(15) << initial_energy << " J" << std::endl;

    // 运行模拟
    std::cout << "\nStarting simulation..." << std::endl;
    auto results = solver.run_simulation(100); // 每100步输出一次

    // 验证能量守恒
    solver.verify_energy_conservation(results);

    // 导出结果
    std::string filename;
    std::cout << "\nEnter output filename (e.g., results.csv): ";
    std::cin >> filename;

    if (filename.empty())
    {
        filename = "three_body_results.csv";
    }

    solver.export_to_csv(results, filename);

    // 显示最后几个时间步的结果
    std::cout << "\n\nFinal few time steps:" << std::endl;
    std::cout << std::setw(12) << "Time(s)" << std::setw(25) << "Total Energy(J)" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    int start_idx = std::max(0, static_cast<int>(results.size()) - 5);
    for (int i = start_idx; i < static_cast<int>(results.size()); ++i)
    {
        std::cout << std::setw(12) << std::fixed << std::setprecision(2)
                  << results[i].time
                  << std::setw(25) << std::scientific << std::setprecision(15)
                  << results[i].total_energy << std::endl;
    }

    std::cout << "\nSimulation completed successfully!" << std::endl;
    std::cout << "Results saved to: " << filename << std::endl;

    return 0;
}