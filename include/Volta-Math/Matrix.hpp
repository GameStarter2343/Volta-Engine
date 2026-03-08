#include "Quaternion.hpp"

class m4 {
    public:
    float m[4][4] = {};
    m4() { m[0][0] = 1.0f; m[1][1] = 1.0f; m[2][2] = 1.0f; m[3][3] = 1.0f; }
    static m4 Identity() noexcept {
        m4 r;
        r.m[0][0] = 1.0f; r.m[1][1] = 1.0f; r.m[2][2] = 1.0f; r.m[3][3] = 1.0f;
        return r; }
    m4 Inverse() const noexcept {
        float a00 = m[0][0], a01 = m[0][1], a02 = m[0][2];
        float a10 = m[1][0], a11 = m[1][1], a12 = m[1][2];
        float a20 = m[2][0], a21 = m[2][1], a22 = m[2][2];

        float c00 = a11*a22 - a12*a21;
        float c01 = a02*a21 - a01*a22;
        float c02 = a01*a12 - a02*a11;
        float c10 = a12*a20 - a10*a22;
        float c11 = a00*a22 - a02*a20;
        float c12 = a02*a10 - a00*a12;
        float c20 = a10*a21 - a11*a20;
        float c21 = a01*a20 - a00*a21;
        float c22 = a00*a11 - a01*a10;

        float det = a00*c00 + a01*c10 + a02*c20;
        if (fabsf(det) < 1e-6f) return Identity();
        float invdet = 1.0f / det;

        m4 inv = Identity();
        inv.m[0][0] = c00 * invdet; inv.m[0][1] = c01 * invdet; inv.m[0][2] = c02 * invdet;
        inv.m[1][0] = c10 * invdet; inv.m[1][1] = c11 * invdet; inv.m[1][2] = c12 * invdet;
        inv.m[2][0] = c20 * invdet; inv.m[2][1] = c21 * invdet; inv.m[2][2] = c22 * invdet;

        float tx = m[3][0], ty = m[3][1], tz = m[3][2];
        inv.m[3][0] = -(inv.m[0][0]*tx + inv.m[0][1]*ty + inv.m[0][2]*tz);
        inv.m[3][1] = -(inv.m[1][0]*tx + inv.m[1][1]*ty + inv.m[1][2]*tz);
        inv.m[3][2] = -(inv.m[2][0]*tx + inv.m[2][1]*ty + inv.m[2][2]*tz);

        return inv; }
    m4 InverseOrthonormal() const noexcept {
        m4 inv = Identity();
        inv.m[0][0] = m[0][0]; inv.m[0][1] = m[1][0]; inv.m[0][2] = m[2][0];
        inv.m[1][0] = m[0][1]; inv.m[1][1] = m[1][1]; inv.m[1][2] = m[2][1];
        inv.m[2][0] = m[0][2]; inv.m[2][1] = m[1][2]; inv.m[2][2] = m[2][2];

        float tx = m[3][0], ty = m[3][1], tz = m[3][2];
        inv.m[3]
            [0] = -(inv.m[0][0]*tx + inv.m[0][1]*ty + inv.m[0][2]*tz);
        inv.m[3][1] = -(inv.m[1][0]*tx + inv.m[1][1]*ty + inv.m[1][2]*tz);
        inv.m[3][2] = -(inv.m[2][0]*tx + inv.m[2][1]*ty + inv.m[2][2]*tz);

        return inv;}
    static m4 FromQuaternion(const Quaternion& q) {
        m4 r = Identity();
        r.m[0][0] = 1 - 2*q.y*q.y - 2*q.z*q.z;
        r.m[0][1] = 2*q.x*q.y + 2*q.w*q.z;
        r.m[0][2] = 2*q.x*q.z - 2*q.w*q.y;
        r.m[1][0] = 2*q.x*q.y - 2*q.w*q.z;
        r.m[1][1] = 1 - 2*q.x*q.x - 2*q.z*q.z;
        r.m[1][2] = 2*q.y*q.z + 2*q.w*q.x;
        r.m[2][0] = 2*q.x*q.z + 2*q.w*q.y;
        r.m[2][1] = 2*q.y*q.z - 2*q.w*q.x;
        r.m[2][2] = 1 - 2*q.x*q.x - 2*q.y*q.y;
        return r; }

    Vec3 operator*(const Vec3& v) const { return Vec3(
            m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z + m[3][0],
            m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z + m[3][1],
            m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z + m[3][2] );}
    m4 operator*(const m4& b) const noexcept {
        m4 r{};
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                for (int k = 0; k < 4; ++k)
                    r.m[i][j] += m[i][k] * b.m[k][j];
        return r; }

    static m4 Rotate(float angle, const Vec3& axis) noexcept {
        float c = cos(angle);
        float s = sin(angle);
        float omc = 1.0f - c;
        Vec3 a = axis.Normalized();
        float x = a.x, y = a.y, z = a.z;
        m4 r;
        r.m[0][0] = c + x * x * omc;
        r.m[0][1] = x * y * omc - z * s;
        r.m[0][2] = x * z * omc + y * s;
        r.m[0][3] = 0.0f;
        r.m[1][0] = y * x * omc + z * s;
        r.m[1][1] = c + y * y * omc;
        r.m[1][2] = y * z * omc - x * s;
        r.m[1][3] = 0.0f;
        r.m[2][0] = z * x * omc - y * s;
        r.m[2][1] = z * y * omc + x * s;
        r.m[2][2] = c + z * z * omc;
        r.m[2][3] = 0.0f;
        r.m[3][0] = 0.0f; r.m[3][1] = 0.0f; r.m[3][2] = 0.0f; r.m[3][3] = 1.0f;
        return r; }
    static m4 Translate(const Vec3& v) noexcept { m4 r = Identity(); r.m[3][0] = v.x; r.m[3][1] = v.y; r.m[3][2] = v.z; return r; }
    static m4 Scale(const Vec3& v) noexcept { m4 r = Identity(); r.m[0][0] = v.x; r.m[1][1] = v.y; r.m[2][2] = v.z; return r; }
    static m4 TRS(const Vec3& pos, const Quaternion& rot, const Vec3& scale) noexcept { return Translate(pos) * FromQuaternion(rot) * Scale(scale); }

    static m4 Ortho(float left, float right, float bottom, float top, float near, float far) noexcept {
        m4 r = Identity();
        r.m[0][0] = 2.0f / (right - left);
        r.m[1][1] = 2.0f / (top - bottom);
        r.m[2][2] = -2.0f / (far - near);
        r.m[3][0] = -(right + left) / (right - left); r.m[3][1] = -(top + bottom) / (top - bottom); r.m[3][2] = -(far + near) / (far - near);
        return r; }
    static m4 Perspective(float fovY, float aspect, float zNear, float zFar) noexcept {
        m4 r{};
        float tanHalf = tan(fovY * 0.5f);
        r.m[0][0] = 1.0f / (aspect * tanHalf);
        r.m[1][1] = 1.0f / tanHalf;
        r.m[2][2] = -(zFar + zNear) / (zFar - zNear);
        r.m[2][3] = -1.0f;
        r.m[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
        return r; }

    static m4 LookAt(Vec3 eye, Vec3 center, Vec3 up) noexcept {
        Vec3 f = (center - eye).Normalized();
        Vec3 s = Vec3::Cross(f, up).Normalized();
        Vec3 u = Vec3::Cross(s, f);
        m4 result;
        result.m[0][0] = s.x; result.m[0][1] = s.y; result.m[0][2] = s.z; result.m[0][3] = -s.Dot(eye);
        result.m[1][0] = u.x; result.m[1][1] = u.y; result.m[1][2] = u.z; result.m[1][3] = -u.Dot(eye);
        result.m[2][0] = -f.x; result.m[2][1] = -f.y; result.m[2][2] = -f.z; result.m[2][3] = f.Dot(eye);
        result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;
        return result; }
    const float* data() const noexcept { return &m[0][0]; }

};
