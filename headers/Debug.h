#include <fstream>
#include <iostream>
#include <string>
#include "Math.h"

#pragma once
class Debug {
    public:
    inline static bool isOn;
    inline static std::fstream file;

    static void Log(int v) { if (isOn && file.is_open()) file << v << "\n"; }
    static void Log(float v) { if (isOn && file.is_open()) file << v << "\n"; }
    static void Log(std::string s) { if (isOn && file.is_open()) file << s << "\n"; }

    static void Log(Vec2 v) { if (isOn && file.is_open()) file << "(" << v.x << ", " << v.y << ")\n"; }
    static void Log(Vec3 v) { if (isOn && file.is_open()) file << "(" << v.x << ", " << v.y << ", " << v.z << ")\n"; }
    static void Log(Quaternion v) { if (isOn && file.is_open()) file << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")\n"; }
    static void Log(m4 v) { if (isOn && file.is_open()) {
        file << "Matrix:\n";
        for (int i=0; i<4; i++) {
            file << "(" << v.m[i][0] << ", " << v.m[i][1] << ", " << v.m[i][2] << ", " << v.m[i][3] << ")\n";
        }
    }}

    Debug(bool isOn) {
        if (isOn) {
            Debug::isOn = true;
            file.open("log.txt", std::ios::out | std::ios::trunc);
            if (!file.is_open()) {
                std::cout << "Failed to open log file!" << std::endl;
                isOn = false;
            }
            Log("=== ENGINE STARTUP ===\n");
        }
    }

    ~Debug() {
        if (file.is_open()) {
            file << "\n=== ENGINE SHUTDOWN ===\n";
            file.close();
        }
    }
};
