#include "Constants.h"
#include "ThreeBodySolver.h"
#include "Vector3.h"
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <mpfr.h>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

ThreeBodySolver::ThreeBodySolver(int precision)
	: time_step(Constants::DEFAULT_TIME_STEP),
	simulation_time(Constants::DEFAULT_SIMULATION_TIME),
	precision_bits(precision),
	use_symplectic(true),
	adaptive_step(false) {
	mpfr_set_default_prec(precision_bits);
}

ThreeBodySolver::~ThreeBodySolver() {
	// Vector3的析构函数会自动清理MPFR变量
}

void ThreeBodySolver::add_body(const std::string& name, double mass,
	double px, double py, double pz,
	double vx, double vy, double vz) {
	bodies.emplace_back(name, mass, px, py, pz, vx, vy, vz);
}

void ThreeBodySolver::set_simulation_params(double dt, double total_time,
	bool symplectic, bool adaptive) {
	time_step = dt;
	simulation_time = total_time;
	use_symplectic = symplectic;
	adaptive_step = adaptive;
}

void ThreeBodySolver::calculate_accelerations() {
	for (size_t i = 0; i < bodies.size(); ++i) {
		bodies[i].acceleration = Vector3(0, 0, 0);

		for (size_t j = 0; j < bodies.size(); ++j) {
			if (i == j) continue;

			Vector3 r_vec = bodies[j].position - bodies[i].position;
			double r = r_vec.magnitude();

			if (r < Constants::MIN_DISTANCE) {
				r = Constants::MIN_DISTANCE;
			}

			double force_magnitude = Constants::GRAVITATIONAL_CONSTANT *
				bodies[j].mass / (r * r * r);

			bodies[i].acceleration = bodies[i].acceleration + r_vec * force_magnitude;
		}
	}
}

void ThreeBodySolver::symplectic_step() {
	// 位置Verlet算法（二阶辛算法）
	// 第一步：更新位置
	for (auto& body : bodies) {
		body.position = body.position + body.velocity * time_step +
			body.acceleration * (0.5 * time_step * time_step);
	}

	// 保存旧的加速度
	std::vector<Vector3> old_acc(bodies.size());
	for (size_t i = 0; i < bodies.size(); ++i) {
		old_acc[i] = bodies[i].acceleration;
	}

	// 重新计算加速度
	calculate_accelerations();

	// 第二步：更新速度
	for (size_t i = 0; i < bodies.size(); ++i) {
		bodies[i].velocity = bodies[i].velocity +
			(old_acc[i] + bodies[i].acceleration) * (0.5 * time_step);
	}
}

void ThreeBodySolver::rk4_step() {
	// 四阶龙格-库塔法
	size_t n = bodies.size();

	// 保存当前状态
	std::vector<Vector3> current_pos(n);
	std::vector<Vector3> current_vel(n);
	for (size_t i = 0; i < n; ++i) {
		current_pos[i] = bodies[i].position;
		current_vel[i] = bodies[i].velocity;
	}

	// 临时变量
	std::vector<Vector3> k1_pos(n), k1_vel(n);
	std::vector<Vector3> k2_pos(n), k2_vel(n);
	std::vector<Vector3> k3_pos(n), k3_vel(n);
	std::vector<Vector3> k4_pos(n), k4_vel(n);

	// 计算k1
	calculate_accelerations();
	for (size_t i = 0; i < n; ++i) {
		k1_pos[i] = bodies[i].velocity * time_step;
		k1_vel[i] = bodies[i].acceleration * time_step;
	}

	// 计算k2
	for (size_t i = 0; i < n; ++i) {
		bodies[i].position = current_pos[i] + k1_pos[i] * 0.5;
		bodies[i].velocity = current_vel[i] + k1_vel[i] * 0.5;
	}
	calculate_accelerations();
	for (size_t i = 0; i < n; ++i) {
		k2_pos[i] = bodies[i].velocity * time_step;
		k2_vel[i] = bodies[i].acceleration * time_step;
	}

	// 计算k3
	for (size_t i = 0; i < n; ++i) {
		bodies[i].position = current_pos[i] + k2_pos[i] * 0.5;
		bodies[i].velocity = current_vel[i] + k2_vel[i] * 0.5;
	}
	calculate_accelerations();
	for (size_t i = 0; i < n; ++i) {
		k3_pos[i] = bodies[i].velocity * time_step;
		k3_vel[i] = bodies[i].acceleration * time_step;
	}

	// 计算k4
	for (size_t i = 0; i < n; ++i) {
		bodies[i].position = current_pos[i] + k3_pos[i];
		bodies[i].velocity = current_vel[i] + k3_vel[i];
	}
	calculate_accelerations();
	for (size_t i = 0; i < n; ++i) {
		k4_pos[i] = bodies[i].velocity * time_step;
		k4_vel[i] = bodies[i].acceleration * time_step;
	}

	// 更新最终状态
	for (size_t i = 0; i < n; ++i) {
		bodies[i].position = current_pos[i] + (k1_pos[i] + k2_pos[i] * 2.0 +
			k3_pos[i] * 2.0 + k4_pos[i]) / 6.0;
		bodies[i].velocity = current_vel[i] + (k1_vel[i] + k2_vel[i] * 2.0 +
			k3_vel[i] * 2.0 + k4_vel[i]) / 6.0;
	}
}

double ThreeBodySolver::estimate_error() {
	// 简单的误差估计（基于加速度变化）
	double max_error = 0.0;
	for (const auto& body : bodies) {
		double acc_mag = body.acceleration.magnitude();
		if (acc_mag > max_error) {
			max_error = acc_mag;
		}
	}
	return max_error * time_step * time_step;
}

double ThreeBodySolver::calculate_total_energy() const {
	double kinetic_energy = 0.0;
	double potential_energy = 0.0;

	// 动能
	for (const auto& body : bodies) {
		double v_sq = body.velocity.magnitude();
		v_sq *= v_sq;
		kinetic_energy += 0.5 * body.mass * v_sq;
	}

	// 势能
	for (size_t i = 0; i < bodies.size(); ++i) {
		for (size_t j = i + 1; j < bodies.size(); ++j) {
			Vector3 r_vec = bodies[j].position - bodies[i].position;
			double r = r_vec.magnitude();
			if (r < Constants::MIN_DISTANCE) r = Constants::MIN_DISTANCE;
			potential_energy -= Constants::GRAVITATIONAL_CONSTANT *
				bodies[i].mass * bodies[j].mass / r;
		}
	}

	return kinetic_energy + potential_energy;
}

double ThreeBodySolver::calculate_angular_momentum() const {
	double total_l = 0.0;
	for (const auto& body : bodies) {
		Vector3 r = body.position;
		Vector3 v = body.velocity;
		// 角动量 = r × p = r × (m*v)
		Vector3 l;
		mpfr_t temp, temp2;
		mpfr_init2(temp, precision_bits);
		mpfr_init2(temp2, precision_bits);

		// Lx = y*pz - z*py
		mpfr_mul(temp, r.y, v.z, MPFR_RNDN);
		mpfr_mul_d(temp, temp, body.mass, MPFR_RNDN);
		mpfr_mul(temp2, r.z, v.y, MPFR_RNDN);
		mpfr_mul_d(temp2, temp2, body.mass, MPFR_RNDN);
		mpfr_sub(temp, temp, temp2, MPFR_RNDN);
		double lx = mpfr_get_d(temp, MPFR_RNDN);

		// Ly = z*px - x*pz
		mpfr_mul(temp, r.z, v.x, MPFR_RNDN);
		mpfr_mul_d(temp, temp, body.mass, MPFR_RNDN);
		mpfr_mul(temp2, r.x, v.z, MPFR_RNDN);
		mpfr_mul_d(temp2, temp2, body.mass, MPFR_RNDN);
		mpfr_sub(temp, temp, temp2, MPFR_RNDN);
		double ly = mpfr_get_d(temp, MPFR_RNDN);

		// Lz = x*py - y*px
		mpfr_mul(temp, r.x, v.y, MPFR_RNDN);
		mpfr_mul_d(temp, temp, body.mass, MPFR_RNDN);
		mpfr_mul(temp2, r.y, v.x, MPFR_RNDN);
		mpfr_mul_d(temp2, temp2, body.mass, MPFR_RNDN);
		mpfr_sub(temp, temp, temp2, MPFR_RNDN);
		double lz = mpfr_get_d(temp, MPFR_RNDN);

		total_l += std::sqrt(lx * lx + ly * ly + lz * lz);

		mpfr_clears(temp, temp2, (mpfr_ptr)0);
	}
	return total_l;
}

bool ThreeBodySolver::check_collisions() {
	for (size_t i = 0; i < bodies.size(); ++i) {
		for (size_t j = i + 1; j < bodies.size(); ++j) {
			Vector3 r_vec = bodies[j].position - bodies[i].position;
			double r = r_vec.magnitude();
			if (r < Constants::MIN_DISTANCE) {
				std::cerr << "WARNING: Collision detected between "
					<< bodies[i].name << " and " << bodies[j].name
					<< " at distance " << r << " m" << std::endl;
				return true;
			}
		}
	}
	return false;
}

std::vector<SimulationResult> ThreeBodySolver::run_simulation(int output_interval) {
	if (bodies.size() != 3) {
		throw std::runtime_error("Exactly 3 bodies are required for three-body simulation");
	}

	std::vector<SimulationResult> results;
	double current_time = 0.0;
	int step_count = 0;

	// 初始加速度计算
	calculate_accelerations();

	// 保存初始状态
	if (output_interval == 1 || step_count % output_interval == 0) {
		SimulationResult initial;
		initial.time = current_time;
		for (const auto& body : bodies) {
			initial.positions.push_back(body.position);
			initial.velocities.push_back(body.velocity);
		}
		initial.total_energy = calculate_total_energy();
		initial.angular_momentum = calculate_angular_momentum();
		results.push_back(initial);
	}

	// 主模拟循环
	while (current_time < simulation_time) {
		// 选择积分方法
		if (use_symplectic) {
			symplectic_step();
		}
		else {
			rk4_step();
		}

		// 碰撞检测
		if (check_collisions()) {
			std::cerr << "Simulation terminated due to collision at t = "
				<< current_time << " s" << std::endl;
			break;
		}

		current_time += time_step;
		step_count++;

		// 输出结果
		if (output_interval == 1 || step_count % output_interval == 0) {
			SimulationResult result;
			result.time = current_time;
			for (const auto& body : bodies) {
				result.positions.push_back(body.position);
				result.velocities.push_back(body.velocity);
			}
			result.total_energy = calculate_total_energy();
			result.angular_momentum = calculate_angular_momentum();
			results.push_back(result);
		}

		// 进度显示
		if (step_count % 100 == 0) {
			double progress = (current_time / simulation_time) * 100.0;
			std::cout << "\rSimulation progress: " << std::fixed << std::setprecision(2)
				<< progress << "%" << std::flush;
		}
	}

	std::cout << "\rSimulation completed: 100.00%" << std::endl;
	return results;
}

void ThreeBodySolver::export_to_csv(const std::vector<SimulationResult>& results,
	const std::string& filename) {
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file: " + filename);
	}

	// 写入表头
	file << "Time(s),";
	for (size_t i = 0; i < bodies.size(); ++i) {
		file << bodies[i].name << "_X(m)," << bodies[i].name << "_Y(m),"
			<< bodies[i].name << "_Z(m),";
		file << bodies[i].name << "_VX(m/s)," << bodies[i].name << "_VY(m/s),"
			<< bodies[i].name << "_VZ(m/s),";
	}
	file << "Total_Energy(J),Angular_Momentum(kg*m^2/s)\n";

	// 写入数据
	for (const auto& result : results) {
		file << std::scientific << std::setprecision(15);
		file << result.time << ",";

		for (size_t i = 0; i < bodies.size(); ++i) {
			file << mpfr_get_d(result.positions[i].x, MPFR_RNDN) << ","
				<< mpfr_get_d(result.positions[i].y, MPFR_RNDN) << ","
				<< mpfr_get_d(result.positions[i].z, MPFR_RNDN) << ",";
			file << mpfr_get_d(result.velocities[i].x, MPFR_RNDN) << ","
				<< mpfr_get_d(result.velocities[i].y, MPFR_RNDN) << ","
				<< mpfr_get_d(result.velocities[i].z, MPFR_RNDN) << ",";
		}

		file << result.total_energy << "," << result.angular_momentum << "\n";
	}

	file.close();
	std::cout << "Results exported to " << filename << std::endl;
}

bool ThreeBodySolver::verify_energy_conservation(const std::vector<SimulationResult>& results,
	double tolerance) {
	if (results.empty()) return false;

	double initial_energy = results[0].total_energy;
	double max_deviation = 0.0;

	for (const auto& result : results) {
		double deviation = std::abs((result.total_energy - initial_energy) / initial_energy);
		if (deviation > max_deviation) {
			max_deviation = deviation;
		}
	}

	bool conserved = max_deviation < tolerance;

	std::cout << "\nEnergy Conservation Check:" << std::endl;
	std::cout << "  Initial Energy: " << std::scientific << std::setprecision(15)
		<< initial_energy << " J" << std::endl;
	std::cout << "  Final Energy: " << results.back().total_energy << " J" << std::endl;
	std::cout << "  Maximum Relative Deviation: " << max_deviation * 100.0 << "%" << std::endl;
	std::cout << "  Tolerance: " << tolerance * 100.0 << "%" << std::endl;
	std::cout << "  Status: " << (conserved ? "PASSED" : "FAILED") << std::endl;

	return conserved;
}

double ThreeBodySolver::get_initial_energy() const {
	return calculate_total_energy();
}