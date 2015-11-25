#ifndef LEVEL_LOADER_HPP
#define LEVEL_LOADER_HPP

#include <yaml-cpp/yaml.h>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>

struct LoadedLevel {
    std::string name;
    int orbs;
    std::vector<Level*> dependencies;
    LoadedLevel* next;
    LoadedLevel(std::string name, int orbs) : name(name), orbs(orbs) {

    }
};

class LevelLoader {
  public:
    LevelLoader() {
    }
    void load() {
        DIR *dir;
        struct dirent *entry;
        if((dir = opendir("levels/")) != nullptr) {
            while((entry = readdir(dir)) != nullptr) {
                auto name = std::string(entry->d_name);
                if(name != ".." && name != ".") {
                    load_level("levels/" + name);
                }
            }
            closedir(dir);
        }
        else {
            std::cout << "Levels directory missing" << std::endl;
        }
    }
    ~LevelLoader() {
        for(Level *level : levels) {
            delete level;
        }
    }
    std::vector<Level*> levels;
    std::map<std::string, Level*> level_map;
  private:
    void load_level(std::string &&filename) {
        std::cout << filename << std::endl;
        auto root = YAML::LoadFile(filename);
    }
};

#endif
