#ifndef PATTERNS_HPP
#define PATTERNS_HPP

#include "functions_pathing.hpp"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <vector>

int number_of_patterns;

struct PatternPart
{
    PatternPart(std::vector<std::function<glm::vec2(glm::vec2, float, float)>>  shot_types, std::vector<int> shots_per_cooldown, std::vector<float> offset) :
    m_shot_type(shot_types), m_shots_per_cooldown(shots_per_cooldown),  m_offset(offset)
    {}
    std::vector<std::function<glm::vec2(glm::vec2, float, float)>> m_shot_type;
    std::vector<int> m_shots_per_cooldown;
    std::vector<float> m_offset;
};

struct Pattern
{
    Pattern(float cooldown, float rotation_speed, 
            std::vector<PatternPart> pattern_parts, std::vector<unsigned int> pattern_parts_lengths, int orbs_needed) 
        : m_cooldown(cooldown), m_rotation_speed(rotation_speed), m_pattern_length(0),
          m_pattern_parts(pattern_parts), m_pattern_parts_lengths(pattern_parts_lengths),
          m_orbs_needed(orbs_needed)
    {
        for(unsigned int i = 0; i < m_pattern_parts.size(); i++)
        {
            m_pattern_length += m_pattern_parts_lengths[i];
        } 
    }
    float m_cooldown;
    float m_rotation_speed;
    unsigned int m_pattern_length;
    std::vector<PatternPart> m_pattern_parts;
    std::vector<unsigned int> m_pattern_parts_lengths;
    int m_orbs_needed;

};

PatternPart LINEAR_THREE_SHOTS({linear_path},{3},{0.f});
PatternPart SCYTHE_LINEAR_FOUR_FOUR_SHOTS({scythe_path,linear_path},{4,4},{0,glm::radians(45.f)});
PatternPart PAUSE({linear_path},{0},{0});
PatternPart SIN_ONE_SHOT({sin_path},{1},{0.f});
PatternPart SIN_TWO_SHOT({sin_path},{2},{0.f});
PatternPart SIN_THREE_SHOT({sin_path},{3},{0.f});
PatternPart SIN_FOUR_SHOT({sin_path},{4},{0.f});
PatternPart SIN_FIVE_SHOT({sin_path},{5},{0.f});
PatternPart SIN_SIX_SHOT({sin_path},{6},{0.f});

Pattern level1(0.4f,25.f,{SIN_FOUR_SHOT,},{1},5);
Pattern level2(0.4f,15.f,{SCYTHE_LINEAR_FOUR_FOUR_SHOTS},{1},15);
Pattern test_level(0.5f,19.f,{
        LINEAR_THREE_SHOTS,SCYTHE_LINEAR_FOUR_FOUR_SHOTS},{10,10},200);


#endif
