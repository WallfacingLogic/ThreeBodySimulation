#ifndef THREE_BODY_SOLVER_H
#define THREE_BODY_SOLVER_H

#include "Constants.h"
#include "Vector3.h"
#include <string>
#include <vector>

// 天体结构体
struct CelestialBody {
	std::string name;
	double mass;          // 质量 (kg)
	Vector3 position;     // 位置 (m)
	Vector3 velocity;     // 速度 (m/s)
	Vector3 acceleration; // 加速度 (m/s^2)

	CelestialBody(const std::string& n, double m,
		double px, double py, double pz,
		double vx, double vy, double vz)
		: name(n), mass(m), position(px, py, pz), velocity(vx, vy, vz) {
	}
};

// 模拟结果结构体
struct SimulationResult {
	double time;
	std::vector<Vector3> positions;
	std::vector<Vector3> velocities;
	double total_energy;
	double angular_momentum;
};

// 三体问题求解器类
class ThreeBodySolver {
private:
	std::vector<CelestialBody> bodies;
	double time_step;
	double simulation_time;
	int precision_bits;
	bool use_symplectic;
	bool adaptive_step;

	// 计算加速度
	void calculate_accelerations();

	// 辛算法步进
	void symplectic_step();

	// 龙格-库塔4阶步进
	void rk4_step();

	// 自适应步长控制
	double estimate_error();

	// 计算系统总能量
	double calculate_total_energy() const;

	// 计算角动量
	double calculate_angular_momentum() const;

	// 碰撞检测
	bool check_collisions();

public:
	ThreeBodySolver(int precision = Constants::DEFAULT_PRECISION);
	~ThreeBodySolver();

	// 添加天体
	void add_body(const std::string& name, double mass,
		double px, double py, double pz,
		double vx, double vy, double vz);

	// 设置模拟参数
	void set_simulation_params(double dt, double total_time,
		bool symplectic = true,
		bool adaptive = false);

	// 运行模拟
	std::vector<SimulationResult> run_simulation(int output_interval = 1);

	// 导出结果到CSV文件
	void export_to_csv(const std::vector<SimulationResult>& results,
		const std::string& filename);

	// 验证能量守恒
	bool verify_energy_conservation(const std::vector<SimulationResult>& results,
		double tolerance = Constants::ENERGY_TOLERANCE);

	// 获取初始总能量
	double get_initial_energy() const;
};

#endif // THREE_BODY_SOLVER_H