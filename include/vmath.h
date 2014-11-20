
#ifndef __MATH_H__
#define __MATH_H__

#include <cmath>

class Vector3 {
public:
	float x, y, z;

public:
	Vector3() : x(0), y(0), z(0) {}
	
	Vector3(float x, float y, float z)
            : x(x), y(y), z(z) {}
	
	Vector3(const Vector3& from, const Vector3& to)  {
		x = to.x - from.x;
		y = to.y - from.y;
		z = to.z - from.z;
	}

    void operator += (const Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}

    Vector3 operator + (const Vector3& v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	
	void operator -= (const Vector3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}
	
	Vector3 operator-(const Vector3& v) const {
		return Vector3(x-v.x, y-v.y, z-v.z);
	}
	
	void operator*=(float value) {
		x *= value;
		y *= value;
		z *= value;
	}
	
	Vector3 operator*(float value) const {
		return Vector3(x*value, y*value, z*value);
	}
	
	bool operator==(const Vector3& other) const {
		return 
			x == other.x &&
			y == other.y &&
			z == other.z;
	}
	
	bool operator!=(const Vector3& other) const {
		return !(*this == other);
	}
	
	Vector3 operator%(const Vector3 &vector) const {
            return Vector3(y*vector.z-z*vector.y,
                           z*vector.x-x*vector.z,
                           x*vector.y-y*vector.x);
	}

	float operator*(const Vector3 &vector) const {
		return x*vector.x + y*vector.y + z*vector.z;
	}
	
	float Length() const {
		return sqrtf(x*x+y*y+z*z);
	}
	
	float SqLength() const {
		return x*x+y*y+z*z;
	}
	
	void Normalise()  {
		float l = Length();
		if (l > 0) (*this) *= (1.0f)/l;
	}
	
	Vector3 Unit() const {
		Vector3 result = *this;
		result.Normalise();
		return result;
	}
	
	void setMagnitude(float magnitude) {
		Normalise();
        (*this) *= magnitude;
	}

    void Clear() {
		x = y = z = 0;
	}
	
	void Invert() {
		x = -x;
		y = -y;
		x = -z;
	}
	
	bool isZero() const {
		return x == 0 && y == 0 && z == 0;
	}
	
	float distance(const Vector3 &other) const {
		return Vector3(*this, other).Length();
	}
};


/** 2*pi */
const float _2PI = 6.28318530717958647692f;

/** pi/180 */
const float _PI_180 = 0.01745329251994329576f;

/** 180/pi */
const float _180_PI = 57.2957795130823208767f;

/** pi */
const float _PI = 3.14159265358979323846f;

/** pi/2 */
const float _PI_2 = 1.57079632679489661923f;

/** pi/4 */
const float _PI_4 = 0.78539816339744830962f;

/** 1/pi */
const float _1_PI = 0.31830988618379067154f;

/** 2/pi */
const float _2_PI = 0.63661977236758134308f;

struct Sphere {
	Vector3 position;
	float radius;
};

float randomReal(float max=1);
float randomBinomial(float max=1);

#endif // __MATH_H__