#include <iostream>
#include <fstream>
#include "Volta-Math.hpp"
class Debug {
public:
    static bool isOn;
    static std::fstream file;
    Debug(bool isOn) {
        if (isOn) {
            Debug::isOn = true;
            file.open("log.txt", std::ios::out | std::ios::trunc);
            if (file.is_open()) {
                Log("=== ENGINE STARTUP ===");
                Log("");
            }
        }
    }

    ~Debug() {
        if (file.is_open()) {
            file << "\n=== ENGINE SHUTDOWN ===\n";
            file.close();
        }
    }

    static void Log(const int& v) { if (isOn && file.is_open()) file << v << "\n"; }
    static void Log(const float& v) { if (isOn && file.is_open()) file << v << "\n"; }
    static void Log(const std::string& s) { if (isOn && file.is_open()) file << s << "\n"; }
    static void Log(const VMath::Vec3& v) { if (isOn && file.is_open()) file << v.x << ", " << v.y << ", " << v.z << "\n"; }
    static void Log(const VMath::Quaternion& v) { if (isOn && file.is_open()) file << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "\n"; }
};
