#include "Utils.hpp"
#include "Vector.hpp"
#pragma once

class Quaternion {
    public:
    float w, x, y, z;
    Quaternion() : w(1), x(0), y(0), z(0) {}
    Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

    Vec3 ToEuler() const {
        return Vec3(
            atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y)),
            asin(clamp(2 * (w * y - z * x), -1.0f, 1.0f)),
            atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z)));}
    static Quaternion FromAxisAngle(Vec3 axis, float angle) {
        float s = sin(angle * 0.5f);
        return Quaternion(cos(angle * 0.5f), axis.x * s, axis.y * s, axis.z * s); }

    float Length() { return sqrt(w*w + x*x + y*y + z*z); }
    void Normalize() { float l = Length(); if (l > 0) { w /= l; x /= l; y /= l; z /= l; } }
    float Dot(const Quaternion& b) const { return w * b.w + x * b.x + y * b.y + z * b.z;}
    Quaternion Normalized() const { Quaternion n = *this; n.Normalize(); return n; }
    Quaternion Conjugate() const { return Quaternion(w, -x, -y, -z); }
    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w*q.w - x*q.x - y*q.y - z*q.z,
            w*q.x + x*q.w + y*q.z - z*q.y,
            w*q.y - x*q.z + y*q.w + z*q.x,
            w*q.z + x*q.y - y*q.x + z*q.w
        ); }

    Quaternion Lerp(const Quaternion& b, float t) const {
        float dot = Dot(b);
        Quaternion axis = (dot < 0.0f) ? Quaternion(-b.w, -b.x, -b.y, -b.z) : b;
        return Quaternion(
            w * (1.0f - t) + axis.w * t, x * (1.0f - t) + axis.x * t,
            y * (1.0f - t) + axis.y * t, z * (1.0f - t) + axis.z * t ).Normalized(); }
    Quaternion Slerp(Quaternion b, float t) const {
        float dot = Dot(b);

        if (dot < 0.0f) { b = Quaternion(-b.w, -b.x, -b.y, -b.z); dot = -dot; }
        if (dot > 0.9995f) return Lerp(b, t);

        float theta_0 = acos(dot);
        float theta = theta_0 * t;
        float sin_theta = sin(theta);
        float sin_theta_0 = sin(theta_0);

        float s0 = cos(theta) - dot * sin_theta / sin_theta_0;
        float s1 = sin_theta / sin_theta_0;

        return Quaternion((w * s0) + (b.w * s1), (x * s0) + (b.x * s1), (y * s0) + (b.y * s1), (z * s0) + (b.z * s1)); }
};
