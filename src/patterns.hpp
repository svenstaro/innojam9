#ifndef PATTERNS_HPP
#define PATTERNS_HPP

#include "functions_pathing.hpp"
#include "path_definition.hpp"

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <functional>

class PatternPart {
    public:
    PatternPart(std::vector<Path_Def> shot_types,
            std::vector<int> shots_per_cooldown, std::vector<float> offset);

    std::vector<Path_Def> m_shot_type;
    std::vector<int> m_shots_per_cooldown;
    std::vector<float> m_offset;

    static PatternPart LINEAR_THREE_SHOTS() {
        return {{Path_Def(linear_path)},{3},{0.f}};
    }

    static PatternPart LINEAR_FOUR_SHOTS() {
        return {{linear_path},{4},{glm::radians(45.f)}};
    }

    static PatternPart SCYTHE_LINEAR_FOUR_FOUR_SHOTS() {
        return {{Path_Def(scythe_path),Path_Def(linear_path)},{4,4},{0,glm::radians(45.f)}};
    }

    static PatternPart PAUSE() {
        return {{Path_Def(linear_path)},{0},{0}};
    }

    static PatternPart SIN_ONE_SHOT() {
        return {{Path_Def(sin_path)},{1},{0.f}};
    }

    static PatternPart SIN_TWO_SHOT() {
        return {{Path_Def(sin_path)},{2},{0.f}};
    }

    static PatternPart SIN_THREE_SHOT() {
        return {{Path_Def(sin_path)},{3},{0.f}};
    }

    static PatternPart SIN_FOUR_SHOT() {
        return {{Path_Def(sin_path)},{4},{0.f}};
    }

    static PatternPart SIN_FIVE_SHOT() {
        return {{Path_Def(sin_path)},{5},{0.f}};
    }

    static PatternPart SIN_SIX_SHOT() {
        return {{Path_Def(sin_path)},{6},{0.f}};
    }
};

class Pattern {
    public:
    Pattern(float cooldown, float rotation_speed,
            std::vector<PatternPart> pattern_parts,
            std::vector<unsigned int> pattern_parts_lengths, int orbs_needed);

    static Pattern level1() {
        return {0.4f,25.f,{PatternPart::SIN_FOUR_SHOT()},{1},5};
    }

    static Pattern level2() {
        return {0.4f,15.f,{PatternPart::SCYTHE_LINEAR_FOUR_FOUR_SHOTS()},{1},15};
    }

    static Pattern level3() {
        return {0.5f,19.f,{PatternPart::LINEAR_FOUR_SHOTS(),PatternPart::SCYTHE_LINEAR_FOUR_FOUR_SHOTS()},{10,10},20};
    }
    
    static Pattern level4() {
        return {0.4f,30.f,{PatternPart::SIN_FOUR_SHOT(), PatternPart::SCYTHE_LINEAR_FOUR_FOUR_SHOTS(), PatternPart::LINEAR_FOUR_SHOTS()},{1,1,1},45};
    }   
    
        float m_cooldown;
    float m_rotation_speed;
    unsigned int m_pattern_length;
    std::vector<PatternPart> m_pattern_parts;
    std::vector<unsigned int> m_pattern_parts_lengths;
    int m_orbs_needed;
};
#endif
