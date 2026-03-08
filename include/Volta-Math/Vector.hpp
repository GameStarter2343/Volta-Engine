#include "Utils.hpp"
#pragma once

class Vec2 {
    public:
    float x, y;
    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}

    Vec2 operator+ (const Vec2& v) const { return Vec2(x + v.x, y + v.y);}
    Vec2 operator- (const Vec2& v) const { return Vec2(x - v.x, y - v.y);}
    Vec2 operator* (const float& a) const { return Vec2(x * a, y * a);}
    Vec2 operator/ (const float& a) const { return Vec2(x / a, y / a);}
    Vec2 operator* (const Vec2& a) const { return Vec2(x * a.x, y * a.y);}
    Vec2 operator/ (const Vec2& a) const { return Vec2(x / a.x, y / a.y);}

    void operator+= (const Vec2& v) {x += v.x; y += v.y;}
    void operator-= (const Vec2& v) {x -= v.x; y -= v.y;}
    void operator*= (const float& a) {x *= a; y *= a;}
    void operator/= (const float& a) {x /= a; y /= a;}
    void operator*= (const Vec2& a) {x *= a.x; y *= a.y;}
    void operator/= (const Vec2& a) {x /= a.x; y /= a.y;}

    bool operator== (const Vec2& v) const { return x == v.x && y == v.y; }
    bool operator!= (const Vec2& v) const { return !(*this == v); }
    Vec2 operator-() const { return Vec2(-x, -y); }
    float operator[] (int i) const { return (&x)[i]; }
    float& operator[] (int i) { return (&x)[i]; }

    float Length() const { return sqrt(x*x + y*y); }
    float Normalize() { float l = Length(); x /= l; y /= l; return l; }
    Vec2 Normalized() const { Vec2 n = *this; n.Normalize(); return n; }
    float Dot(Vec2 v) const { return x*v.x + y*v.y;}
    float Cross(Vec2 v) const { return x*v.y - y*v.x;}
    Vec2 Rotate(float angle) const { return Vec2(x*cos(angle) - y*sin(angle), x*sin(angle) + y*cos(angle)); }

    Vec2 Lerp(const Vec2& v, float t) const { return *this * (1.0f - t) + v * t; }

    bool Equals(const Vec2& v, float epsilon = 0.0001f) const { return abs(x - v.x) < epsilon && abs(y - v.y) < epsilon; }
};
inline Vec2 operator*(float a, const Vec2& v) { return v * a; }

class Vec3 {
    public:
    float x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z);}
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z);}
    Vec3 operator*(const float& a) const { return Vec3(x * a, y * a, z * a);}
    Vec3 operator/(const float& a) const { return Vec3(x / a, y / a, z / a);}
    Vec3 operator*(const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z);}
    Vec3 operator/(const Vec3& v) const { return Vec3(x / v.x, y / v.y, z / v.z);}

    void operator+=(const Vec3& v) {x += v.x; y += v.y; z += v.z;}
    void operator-=(const Vec3& v) {x -= v.x; y -= v.y; z -= v.z;}
    void operator*=(const float& a) {x *= a; y *= a; z *= a;}
    void operator/=(const float& a) {x /= a; y /= a; z /= a;}
    void operator*=(const Vec3& v) {x *= v.x; y *= v.y; z *= v.z;}
    void operator/=(const Vec3& v) {x /= v.x; y /= v.y; z /= v.z;}

    bool operator== (const Vec3& v) const { return x == v.x && y == v.y && z == v.z; }
    bool operator!= (const Vec3& v) const { return !(*this == v); }
    Vec3 operator-() const { return Vec3(-x, -y, -z); }
    float operator[] (int i) const { return (&x)[i]; }
    float& operator[] (int i) { return (&x)[i]; }

    float LengthSq() const { return x*x + y*y + z*z; }
    float Length() const {return sqrt(LengthSq());}
    float Normalize() {float l = Length(); if (l > 0.0001f) { x /= l; y /= l; z /= l; } return l;}
    Vec3 Normalized() const { Vec3 n = *this; n.Normalize(); return n; }
    float Dot(Vec3 v) const {return x*v.x + y*v.y + z*v.z;}
    Vec3 Cross(Vec3 v) const {return Vec3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);}
    static Vec3 Cross(Vec3 a, Vec3 b) { return a.Cross(b); }
    static float Dot(Vec3 a, Vec3 b) { return a.Dot(b); }
    Vec3 Rotate(const Vec3& axis, const float& ang) const { float c = cos(ang); return *this * c + axis.Cross(*this) * sin(ang) + axis * axis.Dot(*this) * (1.0f - c); }

    Vec3 Lerp(const Vec3& v, float t) const { return *this * (1.0f - t) + v * t; }
    static float Distance(const Vec3& a, const Vec3& b) { return (a - b).Length(); }
    Vec3 Reflect(const Vec3& normal) const { return *this - normal * 2.0f * Dot(normal); }
    Vec3 ProjectOn(const Vec3& v) const { return v * (Dot(v) / v.LengthSq()); }
    float AngleBetween(const Vec3& v) const { return acos(clamp(Dot(v) / (Length() * v.Length()), -1.0f, 1.0f)); }

    bool Equals(const Vec3& v, float epsilon = 0.0001f) const {
        return abs(x - v.x) < epsilon &&
               abs(y - v.y) < epsilon &&
               abs(z - v.z) < epsilon;
    }
    explicit Vec3(const Vec2& v, float _z = 0.0f) : x(v.x), y(v.y), z(_z) {}
};
inline Vec3 operator*(float a, const Vec3& v) { return v * a; }
