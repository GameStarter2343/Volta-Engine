#include <iostream>
#include <fstream>
#include <string>
#include "Volta-Math.hpp"
class Debug {
public:
    inline static bool isOn;
    inline static std::fstream file;
    Debug(bool isOn) {
        if (isOn) {
            Debug::isOn = true;
            file.open("build/log.txt", std::ios::out | std::ios::trunc);
            if (file.is_open()) {
                Log("=== ENGINE STARTUP ===\n");
            }
            else std::cout << "Failed to open log file\n" << std::endl;
        }
    }

    ~Debug() {
        if (file.is_open()) {
            file << "\n=== ENGINE SHUTDOWN ===\n";
            file.close();
        }
    }

    static void Log(int v) { if (isOn && file.is_open()) file << v << std::endl; }
    static void Log(float v) { if (isOn && file.is_open()) file << v << std::endl; }
    static void Log(const std::string& s) { if (isOn && file.is_open()) file << s << std::endl; }
    static void Log(const VMath::Vec3& v) { if (isOn && file.is_open()) file << v.x << ", " << v.y << ", " << v.z << std::endl; }
    static void Log(const VMath::Quaternion& v) { if (isOn && file.is_open()) file << v.x << ", " << v.y << ", " << v.z << ", " << v.w << std::endl; }
};
