#ifndef VECTOR3_H
#define VECTOR3_H

#include "Constants.h"
#include <iostream>
#include <mpfr.h>

class Vector3 {
public:
	mpfr_t x, y, z;

	// 构造函数
	Vector3() {
		mpfr_init2(x, Constants::DEFAULT_PRECISION);
		mpfr_init2(y, Constants::DEFAULT_PRECISION);
		mpfr_init2(z, Constants::DEFAULT_PRECISION);
		mpfr_set_zero(x, 0);
		mpfr_set_zero(y, 0);
		mpfr_set_zero(z, 0);
	}

	Vector3(double x_val, double y_val, double z_val) {
		mpfr_init2(x, Constants::DEFAULT_PRECISION);
		mpfr_init2(y, Constants::DEFAULT_PRECISION);
		mpfr_init2(z, Constants::DEFAULT_PRECISION);
		mpfr_set_d(x, x_val, MPFR_RNDN);
		mpfr_set_d(y, y_val, MPFR_RNDN);
		mpfr_set_d(z, z_val, MPFR_RNDN);
	}

	// 拷贝构造函数
	Vector3(const Vector3& other) {
		mpfr_init2(x, Constants::DEFAULT_PRECISION);
		mpfr_init2(y, Constants::DEFAULT_PRECISION);
		mpfr_init2(z, Constants::DEFAULT_PRECISION);
		mpfr_set(x, other.x, MPFR_RNDN);
		mpfr_set(y, other.y, MPFR_RNDN);
		mpfr_set(z, other.z, MPFR_RNDN);
	}

	// 析构函数
	~Vector3() {
		mpfr_clear(x);
		mpfr_clear(y);
		mpfr_clear(z);
	}

	// 赋值运算符
	Vector3& operator=(const Vector3& other) {
		if (this != &other) {
			mpfr_set(x, other.x, MPFR_RNDN);
			mpfr_set(y, other.y, MPFR_RNDN);
			mpfr_set(z, other.z, MPFR_RNDN);
		}
		return *this;
	}

	// 向量加法
	Vector3 operator+(const Vector3& other) const {
		Vector3 result;
		mpfr_add(result.x, x, other.x, MPFR_RNDN);
		mpfr_add(result.y, y, other.y, MPFR_RNDN);
		mpfr_add(result.z, z, other.z, MPFR_RNDN);
		return result;
	}

	// 向量减法
	Vector3 operator-(const Vector3& other) const {
		Vector3 result;
		mpfr_sub(result.x, x, other.x, MPFR_RNDN);
		mpfr_sub(result.y, y, other.y, MPFR_RNDN);
		mpfr_sub(result.z, z, other.z, MPFR_RNDN);
		return result;
	}

	// 标量乘法
	Vector3 operator*(double scalar) const {
		Vector3 result;
		mpfr_mul_d(result.x, x, scalar, MPFR_RNDN);
		mpfr_mul_d(result.y, y, scalar, MPFR_RNDN);
		mpfr_mul_d(result.z, z, scalar, MPFR_RNDN);
		return result;
	}

	// 标量除法
	Vector3 operator/(double scalar) const {
		Vector3 result;
		mpfr_div_d(result.x, x, scalar, MPFR_RNDN);
		mpfr_div_d(result.y, y, scalar, MPFR_RNDN);
		mpfr_div_d(result.z, z, scalar, MPFR_RNDN);
		return result;
	}

	// 点积
	double dot(const Vector3& other) const {
		mpfr_t temp1, temp2, temp3, result;
		mpfr_inits2(Constants::DEFAULT_PRECISION, temp1, temp2, temp3, result, (mpfr_ptr)0);

		mpfr_mul(temp1, x, other.x, MPFR_RNDN);
		mpfr_mul(temp2, y, other.y, MPFR_RNDN);
		mpfr_mul(temp3, z, other.z, MPFR_RNDN);
		mpfr_add(result, temp1, temp2, MPFR_RNDN);
		mpfr_add(result, result, temp3, MPFR_RNDN);

		double val = mpfr_get_d(result, MPFR_RNDN);
		mpfr_clears(temp1, temp2, temp3, result, (mpfr_ptr)0);
		return val;
	}

	// 向量模长
	double magnitude() const {
		mpfr_t temp_x_sq, temp_y_sq, temp_z_sq, sum_sq, result;
		mpfr_inits2(Constants::DEFAULT_PRECISION, temp_x_sq, temp_y_sq, temp_z_sq, sum_sq, result, (mpfr_ptr)0);

		mpfr_mul(temp_x_sq, x, x, MPFR_RNDN);
		mpfr_mul(temp_y_sq, y, y, MPFR_RNDN);
		mpfr_mul(temp_z_sq, z, z, MPFR_RNDN);

		mpfr_add(sum_sq, temp_x_sq, temp_y_sq, MPFR_RNDN);
		mpfr_add(sum_sq, sum_sq, temp_z_sq, MPFR_RNDN);

		mpfr_sqrt(result, sum_sq, MPFR_RNDN);

		double val = mpfr_get_d(result, MPFR_RNDN);
		mpfr_clears(temp_x_sq, temp_y_sq, temp_z_sq, sum_sq, result, (mpfr_ptr)0);
		return val;
	}

	// 单位向量
	Vector3 normalize() const {
		double mag = magnitude();
		if (mag < Constants::MIN_DISTANCE) {
			return Vector3(0, 0, 0);
		}
		return *this / mag;
	}

	// 输出
	void print() const {
		std::cout << "(" << mpfr_get_d(x, MPFR_RNDN) << ", "
			<< mpfr_get_d(y, MPFR_RNDN) << ", "
			<< mpfr_get_d(z, MPFR_RNDN) << ")";
	}
};

#endif // VECTOR3_H