#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include "VMath.hpp"
class Debug {
public:
    inline static uint8_t isOn;
    inline static std::fstream file;
    Debug(uint8_t isOn) {
        if (isOn > 0) {
            Debug::isOn = isOn;
            file.open("build/log.txt", std::ios::out | std::ios::trunc);
            if (file.is_open()) {
                Log("=== ENGINE STARTUP ===\n", 1);
            }
            else std::cout << "Failed to open log file\n";
        }
    }

    ~Debug() {
        if (file.is_open()) {
            file << "\n=== ENGINE SHUTDOWN ===\n";
            file.close();
        }
    }

    static void LogSpace(uint8_t l) { file << std::endl; }
    static void Log(int v, uint8_t l) { if (isOn >= l && file.is_open()) file << v; }
    static void Log(float v, uint8_t l) { if (isOn >= l && file.is_open()) file << v; }
    static void Log(const std::string& s, uint8_t l) { if (isOn >= l && file.is_open()) file << s; }
    static void Log(const VMath::Vec3& v, uint8_t l) { if (isOn >= l && file.is_open()) file << v.x << ", " << v.y << ", " << v.z; }
    static void Log(const VMath::Quaternion& v, uint8_t l) { if (isOn >= l && file.is_open()) file << v.x << ", " << v.y << ", " << v.z << ", " << v.w; }
};
