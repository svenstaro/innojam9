#ifndef PATTERNS_HPP
#define PATTERNS_HPP

#include "functions_pathing.hpp"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <functional>

/*
 *
 */


class PatternPart {
    public:
    PatternPart(std::vector<std::function<glm::vec2(glm::vec2, float, float)>>
            shot_types,
            std::vector<int> shots_per_cooldown, std::vector<float> offset);

    std::vector<std::function<glm::vec2(glm::vec2, float, float)>> m_shot_type;
    std::vector<int> m_shots_per_cooldown;
    std::vector<float> m_offset;

    static PatternPart LINEAR_THREE_SHOTS() {
        return {{linear_path},{3},{0.f}};
    }

    static PatternPart LINEAR_FOUR_SHOTS() {
        return {{linear_path},{4},{glm::radians(45.f)}};
    }

    static PatternPart SCYTHE_LINEAR_FOUR_FOUR_SHOTS() {
        return {{scythe_path,linear_path},{4,4},{0,glm::radians(45.f)}};
    }

    static PatternPart PAUSE() {
        return {{linear_path},{0},{0}};
    }

    static PatternPart SIN_ONE_SHOT() {
        return {{sin_path},{1},{0.f}};
    }

    static PatternPart SIN_TWO_SHOT() {
        return {{sin_path},{2},{0.f}};
    }

    static PatternPart SIN_THREE_SHOT() {
        return {{sin_path},{3},{0.f}};
    }

    static PatternPart SIN_FOUR_SHOT() {
        return {{sin_path},{4},{0.f}};
    }

    static PatternPart SIN_FIVE_SHOT() {
        return {{sin_path},{5},{0.f}};
    }

    static PatternPart SIN_SIX_SHOT() {
        return {{sin_path},{6},{0.f}};
    }

    static PatternPart SPIRAL_ONE_SHOT() {
        return {{spiral_path},{1},{0.f}};
    }
};

class Pattern {
    public:
    Pattern(float cooldown, float rotation_speed,
            std::vector<PatternPart> pattern_parts,
            std::vector<unsigned int> pattern_parts_lengths, int orbs_needed);

    static Pattern spiral_test() {
        return {1.f,25.f,{PatternPart::SPIRAL_ONE_SHOT()},{1},2000000};
    }
    
    static Pattern start() {
        return {0.f,0.f,{PatternPart::PAUSE()},{1},5};
    }

    static Pattern level1() {
        return {1.f,25.f,{PatternPart::SIN_FOUR_SHOT()},{1},5};
    }

    static Pattern level2() {
        return {1.f,15.f,{PatternPart::SCYTHE_LINEAR_FOUR_FOUR_SHOTS()},{1},15};
    }

    static Pattern level3() {
        return {1.f,19.f,{PatternPart::LINEAR_FOUR_SHOTS(),PatternPart::SCYTHE_LINEAR_FOUR_FOUR_SHOTS()},{10,10},20};
    }
    
    static Pattern level4() {
        return {1.f,30.f,{PatternPart::SIN_FOUR_SHOT(), PatternPart::SCYTHE_LINEAR_FOUR_FOUR_SHOTS(), PatternPart::LINEAR_FOUR_SHOTS()},{1,1,1},45};
    }   
    
        float m_cooldown;
    float m_rotation_speed;
    unsigned int m_pattern_length;
    std::vector<PatternPart> m_pattern_parts;
    std::vector<unsigned int> m_pattern_parts_lengths;
    int m_orbs_needed;
};

#endif
