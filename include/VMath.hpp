#pragma once

#include <cmath>

namespace VMath {
/* Constants */

    inline constexpr float PI          = 3.1415926535f;
    inline constexpr float TWO_PI      = 6.2831853071f;
    inline constexpr float HALF_PI     = 1.5707963267f;
    inline constexpr float INV_PI      = 0.3183098861f;
    inline constexpr float INV_HALF_PI = 0.6366197724f;
    inline constexpr float INV_TWO_PI  = 0.1591549431f;
    inline constexpr float DEG2RAD     = 0.0174532925f;
    inline constexpr float RAD2DEG     = 57.295779513f;
    inline constexpr float EPSILON     = 0.000001f;
    inline constexpr float DtoR(float D) { return D * DEG2RAD; }
    inline constexpr float RtoD(float R) { return R * RAD2DEG; }

/*Functions*/

    // returns value clamped between min and max
    inline float clamp(float value, float min, float max) { return fmax(min, fmin(value, max)); }
    // returns value clamped between 0 and 1
    inline float clamp01(float value) { return fmax(0, fmin(value, 1)); }
    // returns true if value is within EPSILON of zero
    inline bool IsZero(float a) { return std::abs(a) < EPSILON; }
    // returns true if the difference between a and b is less than EPSILON
    inline bool IsEqual(float a, float b) { return std::abs(a - b) < EPSILON; }
    // returns a value linearly interpolated between a and b by a coefficient t
    inline float Lerp(float a, float b, float t) { return a * (1 - t) + b * t; }
    // returns the interpolation coefficient of a value within the range [a, b]
    inline float InverseLerp(float a, float b, float value) { if (IsZero(b - a)) return 0; return (value - a) / (b - a); }
    // maps a value from the input range [a1, b1] to the output range [a2, b2]
    inline float MapRange(float a1, float b1, float a2, float b2, float value) { return Lerp(a2, b2, InverseLerp(a1, b1, value)); }

//Two float
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

        bool operator== (const Vec2& v) const { return Equals(v); }
        bool operator!= (const Vec2& v) const { return !(*this == v); }
        Vec2 operator-() const { return Vec2(-x, -y); }

        float Length() const { return sqrt(x*x + y*y); }
        float Normalize() { float l = Length(); if (l > EPSILON) { x /= l; y /= l; } return l; }
        Vec2 Normalized() const { Vec2 n = *this; n.Normalize(); return n; }
        float Dot(Vec2 v) const { return x*v.x + y*v.y;}
        float Cross(Vec2 v) const { return x*v.y - y*v.x;}
        Vec2 Rotate(float angle) const { return Vec2(x*cos(angle) - y*sin(angle), x*sin(angle) + y*cos(angle)); }

        Vec2 Lerp(const Vec2& v, float t) const { return *this * (1.0f - t) + v * t; }

        bool Equals(const Vec2& v, float epsilon = EPSILON) const { return abs(x - v.x) < epsilon && abs(y - v.y) < epsilon; }
    };
    inline Vec2 operator*(float a, const Vec2& v) { return v * a; }
//Three float
    class Vec3 {
        public:
        float x, y, z;
        Vec3() : x(0), y(0), z(0) {}
        Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

        Vec3 operator+ (const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z);}
        Vec3 operator- (const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z);}
        Vec3 operator* (const float& a) const { return Vec3(x * a, y * a, z * a);}
        Vec3 operator/ (const float& a) const { return Vec3(x / a, y / a, z / a);}
        Vec3 operator* (const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z);}
        Vec3 operator/ (const Vec3& v) const { return Vec3(x / v.x, y / v.y, z / v.z);}

        void operator+= (const Vec3& v) {x += v.x; y += v.y; z += v.z;}
        void operator-= (const Vec3& v) {x -= v.x; y -= v.y; z -= v.z;}
        void operator*= (const float& a) {x *= a; y *= a; z *= a;}
        void operator/= (const float& a) {x /= a; y /= a; z /= a;}
        void operator*= (const Vec3& v) {x *= v.x; y *= v.y; z *= v.z;}
        void operator/= (const Vec3& v) {x /= v.x; y /= v.y; z /= v.z;}

        bool operator== (const Vec3& v) const { return Equals(v); }
        bool operator!= (const Vec3& v) const { return !Equals(v); }
        Vec3 operator-() const { return Vec3(-x, -y, -z); }

        float LengthSq() const { return x*x + y*y + z*z; }
        float Length() const {return sqrt(LengthSq());}
        float Normalize() {float l = Length(); if (l > EPSILON) { x /= l; y /= l; z /= l; } return l;}
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

        bool Equals(const Vec3& v, float epsilon = EPSILON) const { return std::abs(x - v.x) < epsilon && std::abs(y - v.y) < epsilon && std::abs(z - v.z) < epsilon; }
        explicit Vec3(const Vec2& v, float _z = 0.0f) : x(v.x), y(v.y), z(_z) {}
    };
    inline Vec3 operator*(float a, const Vec3& v) { return v * a; }
//Four float
    class Vec4 {
        public:
        float x, y, z, w;
        Vec4() : x(0), y(0), z(0), w(0) {}
        Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        Vec4 operator+ (const Vec4& v) const { return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);}
        Vec4 operator- (const Vec4& v) const { return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);}
        Vec4 operator* (const float& a) const { return Vec4(x * a, y * a, z * a, w * a);}
        Vec4 operator/ (const float& a) const { return Vec4(x / a, y / a, z / a, w / a);}
        Vec4 operator* (const Vec4& v) const { return Vec4(x * v.x, y * v.y, z * v.z, w * v.w);}
        Vec4 operator/ (const Vec4& v) const { return Vec4(x / v.x, y / v.y, z / v.z, w / v.w);}

        void operator+= (const Vec4& v) {x += v.x; y += v.y; z += v.z; w += v.w;}
        void operator-= (const Vec4& v) {x -= v.x; y -= v.y; z -= v.z; w -= v.w;}
        void operator*= (const float& a) {x *= a; y *= a; z *= a; w *= a;}
        void operator/= (const float& a) {x /= a; y /= a; z /= a; w /= a;}
        void operator*= (const Vec4& v) {x *= v.x; y *= v.y; z *= v.z; w *= v.w;}
        void operator/= (const Vec4& v) {x /= v.x; y /= v.y; z /= v.z; w /= v.w;}

        bool operator== (const Vec4& v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
        bool operator!= (const Vec4& v) const { return !(*this == v); }
        Vec4 operator-() const { return Vec4(-x, -y, -z, -w); }

        float LengthSq() const { return x*x + y*y + z*z + w*w; }
        float Length() const { return sqrt(LengthSq()); }
        float Normalize() { float l = Length(); if (l > EPSILON) { x /= l; y /= l; z /= l; w /= l; } return l; }
        Vec4 Normalized() const { Vec4 n = *this; n.Normalize(); return n; }

        float Dot(Vec4 v) const { return x*v.x + y*v.y + z*v.z + w*v.w; }
        static float Dot(Vec4 a, Vec4 b) { return a.Dot(b); }

        Vec4 Lerp(const Vec4& v, float t) const { return *this * (1.0f - t) + v * t; }
        static float Distance(const Vec4& a, const Vec4& b) { return (a - b).Length(); }

        Vec4 Reflect(const Vec4& normal) const { return *this - normal * 2.0f * Dot(normal); }
        Vec4 ProjectOn(const Vec4& v) const { return v * (Dot(v) / v.LengthSq()); }
        float AngleBetween(const Vec4& v) const { return acos(clamp(Dot(v) / (Length() * v.Length()), -1.0f, 1.0f)); }

        bool Equals(const Vec4& v, float epsilon = EPSILON) const {
            return abs(x - v.x) < epsilon && abs(y - v.y) < epsilon && abs(z - v.z) < epsilon && abs(w - v.w) < epsilon; }

        explicit Vec4(const Vec3& v, float _w = 1.0f) : x(v.x), y(v.y), z(v.z), w(_w) {}
        Vec3 ToVec3() const { return Vec3(x, y, z); } // Simple truncation
        Vec3 ToVec3Perspective() const { return (w != 0.0f) ? Vec3(x/w, y/w, z/w) : Vec3(x, y, z); }
    };
    inline Vec4 operator*(float a, const Vec4& v) { return v * a; }
//Two float Triangle
    class Tri2 {
        public:
        Vec2 a, b, c;
        Tri2(const Vec2& _a, const Vec2& _b, const Vec2& _c) : a(_a), b(_b), c(_c) {}
    };
//Three float Triangle
    class Tri3 {
        public:
        Vec3 a, b, c;
        Tri3(const Vec3& _a, const Vec3& _b, const Vec3& _c) : a(_a), b(_b), c(_c) {}
    };
//Four float Triangle
    class Tri4 {
        public:
        Vec4 a, b, c;
        Tri4(const Vec4& _a, const Vec4& _b, const Vec4& _c) : a(_a), b(_b), c(_c) {}
    };
//Four float But Cool
    class Quaternion {
        public:
        float w, x, y, z;

        Quaternion() : w(1), x(0), y(0), z(0) {}
        Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

        static Quaternion FromAxisAngle(Vec3 axis, float angle) {
            axis.Normalize();
            float s = std::sin(angle * 0.5f);
            return Quaternion(std::cos(angle * 0.5f), axis.x * s, axis.y * s, axis.z * s); }

        Quaternion Conjugate() const { return Quaternion(w, -x, -y, -z); }
        Quaternion Inverse() const {
            float lenSq = w*w + x*x + y*y + z*z;
            if (lenSq < EPSILON) return *this;
            float inv = 1.0f / lenSq;
            return Quaternion(w*inv, -x*inv, -y*inv, -z*inv); }

        float Length() const { return std::sqrt(w*w + x*x + y*y + z*z); } // Added const

        void Normalize() {
            float l = Length();
            if (l > 1e-6f) { float inv = 1.0f / l; w *= inv; x *= inv; y *= inv; z *= inv; }
            else { w = 1; x = y = z = 0; }
        }

        Quaternion operator*(const Quaternion& q) const {
            return Quaternion(
                w*q.w - x*q.x - y*q.y - z*q.z,
                w*q.x + x*q.w + y*q.z - z*q.y,
                w*q.y - x*q.z + y*q.w + z*q.x,
                w*q.z + x*q.y - y*q.x + z*q.w
            );
        }

        Quaternion Slerp(const Quaternion& b, float t) const {
            float dot = w*b.w + x*b.x + y*b.y + z*b.z;
            Quaternion b2 = b;

            if (dot < 0.0f) { b2 = Quaternion(-b.w, -b.x, -b.y, -b.z); dot = -dot; }

            if (dot > 0.9995f) {
                Quaternion res = Quaternion(
                    w * (1.0f - t) + b2.w * t,
                    x * (1.0f - t) + b2.x * t,
                    y * (1.0f - t) + b2.y * t,
                    z * (1.0f - t) + b2.z * t
                );
                res.Normalize();
                return res;
            }

            float theta_0 = std::acos(dot);
            float theta = theta_0 * t;
            float sin_theta_0 = std::sin(theta_0);

            float s0 = std::sin(theta_0 - theta) / sin_theta_0;
            float s1 = std::sin(theta) / sin_theta_0;

            return Quaternion(
                w * s0 + b2.w * s1,
                x * s0 + b2.x * s1,
                y * s0 + b2.y * s1,
                z * s0 + b2.z * s1
            );
        }
    };
//Four Four float
    class m4 {
        public:
        float m[16] = {};

        m4() { m[0]=1.f; m[5]=1.f; m[10]=1.f; m[15]=1.f; }

        static m4 Identity() noexcept {
            m4 r;
            r.m[0]=1.f; r.m[5]=1.f; r.m[10]=1.f; r.m[15]=1.f;
            return r; }

        m4 Inverse() const noexcept {
            float a00=m[0], a01=m[1], a02=m[2];
            float a10=m[4], a11=m[5], a12=m[6];
            float a20=m[8], a21=m[9], a22=m[10];

            float c00=a11*a22-a12*a21, c01=a02*a21-a01*a22, c02=a01*a12-a02*a11;
            float c10=a12*a20-a10*a22, c11=a00*a22-a02*a20, c12=a02*a10-a00*a12;
            float c20=a10*a21-a11*a20, c21=a01*a20-a00*a21, c22=a00*a11-a01*a10;

            float det = a00*c00 + a01*c10 + a02*c20;
            if (fabsf(det) < 1e-6f) return Identity();
            float invdet = 1.f / det;

            m4 inv = Identity();
            inv.m[0]=c00*invdet; inv.m[1]=c01*invdet; inv.m[2]=c02*invdet;
            inv.m[4]=c10*invdet; inv.m[5]=c11*invdet; inv.m[6]=c12*invdet;
            inv.m[8]=c20*invdet; inv.m[9]=c21*invdet; inv.m[10]=c22*invdet;

            float tx=m[12], ty=m[13], tz=m[14];
            inv.m[12]=-(inv.m[0]*tx + inv.m[1]*ty + inv.m[2]*tz);
            inv.m[13]=-(inv.m[4]*tx + inv.m[5]*ty + inv.m[6]*tz);
            inv.m[14]=-(inv.m[8]*tx + inv.m[9]*ty + inv.m[10]*tz);

            return inv; }

        m4 InverseOrthonormal() const noexcept {
            m4 inv = Identity();
            inv.m[0]=m[0]; inv.m[1]=m[4]; inv.m[2]=m[8];
            inv.m[4]=m[1]; inv.m[5]=m[5]; inv.m[6]=m[9];
            inv.m[8]=m[2]; inv.m[9]=m[6]; inv.m[10]=m[10];

            float tx=m[12], ty=m[13], tz=m[14];
            inv.m[12]=-(inv.m[0]*tx + inv.m[1]*ty + inv.m[2]*tz);
            inv.m[13]=-(inv.m[4]*tx + inv.m[5]*ty + inv.m[6]*tz);
            inv.m[14]=-(inv.m[8]*tx + inv.m[9]*ty + inv.m[10]*tz);

            return inv; }

        static m4 FromQuaternion(const Quaternion& q) {
            m4 r = Identity();
            r.m[0] =1-2*q.y*q.y-2*q.z*q.z; r.m[1] =2*q.x*q.y+2*q.w*q.z; r.m[2] =2*q.x*q.z-2*q.w*q.y;
            r.m[4] =2*q.x*q.y-2*q.w*q.z;   r.m[5] =1-2*q.x*q.x-2*q.z*q.z; r.m[6] =2*q.y*q.z+2*q.w*q.x;
            r.m[8] =2*q.x*q.z+2*q.w*q.y;   r.m[9] =2*q.y*q.z-2*q.w*q.x;   r.m[10]=1-2*q.x*q.x-2*q.y*q.y;
            return r; }

        Vec3 operator*(const Vec3& v) const { return Vec3(
            m[0]*v.x + m[4]*v.y + m[8]*v.z  + m[12],
            m[1]*v.x + m[5]*v.y + m[9]*v.z  + m[13],
            m[2]*v.x + m[6]*v.y + m[10]*v.z + m[14] ); }

        Vec4 operator*(const Vec4& v) const { return Vec4(
            m[0]*v.x + m[4]*v.y + m[8]*v.z  + m[12]*v.w,
            m[1]*v.x + m[5]*v.y + m[9]*v.z  + m[13]*v.w,
            m[2]*v.x + m[6]*v.y + m[10]*v.z + m[14]*v.w,
            m[3]*v.x + m[7]*v.y + m[11]*v.z + m[15]*v.w ); }

        m4 operator*(const m4& b) const noexcept {
            m4 r{};
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    for (int k = 0; k < 4; ++k)
                        r.m[i*4+j] += m[i*4+k] * b.m[k*4+j];
            return r; }

        static m4 Rotate(float angle, const Vec3& axis) noexcept {
            float c=cos(angle), s=sin(angle), omc=1.f-c;
            Vec3 a=axis.Normalized();
            float x=a.x, y=a.y, z=a.z;
            m4 r;
            r.m[0]=c+x*x*omc;    r.m[1]=x*y*omc-z*s;  r.m[2]=x*z*omc+y*s;  r.m[3]=0.f;
            r.m[4]=y*x*omc+z*s;  r.m[5]=c+y*y*omc;    r.m[6]=y*z*omc-x*s;  r.m[7]=0.f;
            r.m[8]=z*x*omc-y*s;  r.m[9]=z*y*omc+x*s;  r.m[10]=c+z*z*omc;   r.m[11]=0.f;
            r.m[12]=0.f; r.m[13]=0.f; r.m[14]=0.f; r.m[15]=1.f;
            return r; }

        static m4 Translate(const Vec3& v) noexcept { m4 r=Identity(); r.m[12]=v.x; r.m[13]=v.y; r.m[14]=v.z; return r; }
        static m4 Scale(const Vec3& v)     noexcept { m4 r=Identity(); r.m[0]=v.x;  r.m[5]=v.y;  r.m[10]=v.z; return r; }
        static m4 TRS(const Vec3& pos, const Quaternion& rot, const Vec3& scale) noexcept { return Translate(pos) * FromQuaternion(rot) * Scale(scale); }

        static m4 Ortho(float left, float right, float bottom, float top, float near, float far) noexcept {
            m4 r=Identity();
            r.m[0]=2.f/(right-left); r.m[5]=2.f/(top-bottom); r.m[10]=-2.f/(far-near);
            r.m[12]=-(right+left)/(right-left); r.m[13]=-(top+bottom)/(top-bottom); r.m[14]=-(far+near)/(far-near);
            return r; }

        static m4 Perspective(float fovY, float aspect, float zNear, float zFar) noexcept {
            m4 r{};
            float tanHalf=tan(fovY*0.5f);
            r.m[0]=1.f/(aspect*tanHalf); r.m[5]=1.f/tanHalf;
            r.m[10]=-(zFar+zNear)/(zFar-zNear); r.m[11]=-1.f;
            r.m[14]=-(2.f*zFar*zNear)/(zFar-zNear);
            return r; }

        static m4 LookAt(Vec3 eye, Vec3 center, Vec3 up) noexcept {
            Vec3 f=(center-eye).Normalized();
            Vec3 s=Vec3::Cross(f,up).Normalized();
            Vec3 u=Vec3::Cross(s,f);
            m4 r;
            r.m[0]=s.x;  r.m[1]=s.y;  r.m[2]=s.z;  r.m[3]=-s.Dot(eye);
            r.m[4]=u.x;  r.m[5]=u.y;  r.m[6]=u.z;  r.m[7]=-u.Dot(eye);
            r.m[8]=-f.x; r.m[9]=-f.y; r.m[10]=-f.z; r.m[11]=f.Dot(eye);
            r.m[12]=0.f; r.m[13]=0.f; r.m[14]=0.f;  r.m[15]=1.f;
            return r; }

        const float* data() const noexcept { return m; }
    };
//More Functions
    inline Vec3 rgb(int a, int b, int c) { return Vec3(a / 255.0f, b / 255.0f, c / 255.0f); }
    inline Vec4 rgba(int a, int b, int c) { return Vec4(a / 255.0f, b / 255.0f, c / 255.0f, 1.0f); }
    inline Vec4 rgba(int a, int b, int c, int d) { return Vec4(a / 255.0f, b / 255.0f, c / 255.0f, d / 255.0f); }
} //namespace VMath
