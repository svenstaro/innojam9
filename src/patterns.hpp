#ifndef PATTERNS_HPP
#define PATTERNS_HPP

#include "functions_pathing.hpp"
#include "path_definition.hpp"

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <functional>

/*
 *
 */


class PatternPart {
    public:
    PatternPart(std::vector<Path_Def> shot_types,
            std::vector<int> shots_per_cooldown, std::vector<float> offset,
                std::vector<unsigned int> pattern_parts_lengths);

    std::vector<Path_Def> m_shot_type;
    std::vector<int> m_shots_per_cooldown;
    std::vector<float> m_offset;
    std::vector<unsigned int> m_pattern_parts_lengths;

    
    static PatternPart SIN_TWO_SHOTS() {
        return {{Path_Def(sin_path)},{2},{0.f},{3}};
    }

    static PatternPart SCYTHE_FOUR_SHOT() {
        return {{Path_Def(scythe_path)},{4},{0.f},{4}};
    }

    static PatternPart SCYTHE_FOUR_SHOT_SIN_TWO_SHOTS() {
        return {{Path_Def(scythe_path), Path_Def(sin_path)},{4,2},{0,45.f},{5,2}};
    }
    
};

class Pattern {
    public:
        Pattern(float cooldown, float rotation_speed,
                std::vector<PatternPart> pattern_parts, int orbs_needed);

        static Pattern level1() {
            return {1.f,20.f,{PatternPart::SIN_TWO_SHOTS()},5};
        }
        static Pattern level2() {
            return {1.f,20.f,{PatternPart::SCYTHE_FOUR_SHOT()},5};
        }
        static Pattern level3() {
            return {1.f,40.f,{PatternPart::SCYTHE_FOUR_SHOT_SIN_TWO_SHOTS(),PatternPart::SCYTHE_FOUR_SHOT()},10};
        }

        
    
    float m_cooldown;
    float m_rotation_speed;
    std::vector<PatternPart> m_pattern_parts;
    int m_orbs_needed;
        };
#endif
