#pragma once
#include <stdexcept>
#include <cmath>

constexpr auto M_PI = 3.14159265358979323846f;
constexpr auto M_RADPI = 57.295779513082f;
#define M_PI_F		((float)(M_PI))
#define RAD2DEG( x )  ( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD( x )  ( (float)(x) * (float)(M_PI_F / 180.f) )

struct Vector3
{
	constexpr Vector3(const float x = 0.f, const float y = 0.f, const float z = 0.f) noexcept :
		x(x), y(y), z(z) { }


	constexpr Vector3 operator+(const Vector3& other) const noexcept
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	constexpr Vector3 operator-(const Vector3& other) const noexcept
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	constexpr Vector3 operator*(const Vector3& other) const noexcept
	{
		return Vector3(x * other.x, y * other.y, z * other.z);
	}

	constexpr Vector3 operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	constexpr Vector3 operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	constexpr Vector3 operator*(const float scale) const noexcept
	{
		return Vector3(x * scale, y * scale, z * scale);
	}
	constexpr Vector3 operator/(const float scale) const noexcept
	{
		return Vector3(x / scale, y / scale, z / scale);
	}

	constexpr float operator[](const size_t index) const noexcept
	{
		if (index == 0) return x;
		if (index == 1) return y;
		if (index == 2) return z;
	}

	float Distance(const Vector3& other) const noexcept
	{
		return std::sqrtf((x - other.x) * (x - other.x) +
			(y - other.y) * (y - other.y) +
			(z - other.z) * (z - other.z));
	}
	float Magnitude() const noexcept { //计算向量的长度
		return sqrt(x * x + y * y + z * z);
	}

	Vector3 Cross(const Vector3& other) const {//向量叉积
		return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}

	Vector3  Rotate(float theta, Vector3 axis) const {
		Vector3 v = *this;
		Vector3 v_rotated;
		v_rotated = v * cos(theta) + axis.Cross(v) * sin(theta) + axis * (axis.Dot(v)) * (1 - cos(theta));
		return v_rotated;
	}

	Vector3 Normalized() const {
		float mag = Magnitude();
		return Vector3(x / mag, y / mag, z / mag);
	}

	bool IsEqual(const Vector3& other, const float margin = std::numeric_limits<float>::epsilon()) const noexcept
	{
		return (std::fabsf(x - other.x) < margin &&
			std::fabsf(y - other.y) < margin &&
			std::fabsf(z - other.z) < margin);
	}

	constexpr Vector3 Scale(const float scale) const noexcept
	{
		return Vector3{ x * scale, y * scale, z * scale };
	}

	float Length() const noexcept
	{
		return std::sqrtf(x * x + y * y + z * z);
	}

	float Dot(const Vector3& Other) const {
		return x * Other.x + y * Other.y + z * Other.z;
	}
	float x, y, z;

};

struct Matrix3x3
{
	float data[3][3] = { };
	constexpr float* operator[](int index) noexcept
	{
		return data[index];
	}

	constexpr const float* operator[](int index) const noexcept
	{
		return data[index];
	}
	constexpr Matrix3x3 operator/(float scalar) const noexcept
	{
		Matrix3x3 result;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				result[i][j] = data[i][j] / scalar;
			}
		}
		return result;
	}
	constexpr Matrix3x3 operator-(const Matrix3x3& other) const noexcept
	{
		Matrix3x3 result;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				result[i][j] = data[i][j] - other[i][j];
			}
		}
		return result;
	}

	float trace() const noexcept {
		return data[0][0] + data[1][1] + data[2][2];
	}
	Vector3 operator*(Vector3 v) const {
		float x = data[0][0] * v.x + data[0][1] * v.y + data[0][2] * v.z;
		float y = data[1][0] * v.x + data[1][1] * v.y + data[1][2] * v.z;
		float z = data[2][0] * v.x + data[2][1] * v.y + data[2][2] * v.z;

		return Vector3(x, y, z);
	}
	void FromAxisAngle(Vector3 axis, float angle) noexcept
	{
		const float c = cos(angle);
		const float s = sin(angle);
		const float t = 1.0f - c;

		const float x = axis.x;
		const float y = axis.y;
		const float z = axis.z;

		data[0][0] = t * x * x + c;
		data[0][1] = t * x * y - s * z;
		data[0][2] = t * x * z + s * y;
		data[1][0] = t * x * y + s * z;
		data[1][1] = t * y * y + c;
		data[1][2] = t * y * z - s * x;
		data[2][0] = t * x * z - s * y;
		data[2][1] = t * y * z + s * x;
		data[2][2] = t * z * z + c;
	}
};

struct ViewMatrix
{
	constexpr float* operator[](int index) noexcept
	{
		return data[index];
	}

	constexpr const float* operator[](int index) const noexcept
	{
		return data[index];
	}

	float data[4][4] = { };
};


struct Vector2
{
public:
	float x, y;

	Vector2() { x = y = 0; }
	Vector2(const float x, const float y) : x(x), y(y) {}
	Vector2 operator + (const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
	Vector2 operator - (const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
	Vector2 operator * (const float& rhs) const { return Vector2(x * rhs, y * rhs); }
	Vector2 operator / (const float& rhs) const { return Vector2(x / rhs, y / rhs); }
	Vector2& operator += (const Vector2& rhs) { return *this = *this + rhs; }
	Vector2& operator -= (const Vector2& rhs) { return *this = *this - rhs; }
	Vector2& operator *= (const float& rhs) { return *this = *this * rhs; }
	Vector2& operator /= (const float& rhs) { return *this = *this / rhs; }
	float operator[](int i) const {
		return ((float*)this)[i];
	}

	float& operator[](int i);
	float dot() const { return x * x + y * y; }
	float Length() const { return sqrtf(dot()); }
};




