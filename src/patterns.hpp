#ifndef PATTERNS_HPP
#define PATTERNS_HPP

#include "functions_pathing.hpp"
#include "path_definition.hpp"

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <vector>

int number_of_patterns;

struct PatternPart
{
    PatternPart(std::vector<Path_Def> shot_types, std::vector<int> shots_per_cooldown, std::vector<float> offset) :
    m_shot_type(shot_types), m_shots_per_cooldown(shots_per_cooldown),  m_offset(offset)
    {}
    std::vector<Path_Def> m_shot_type;
    std::vector<int> m_shots_per_cooldown;
    std::vector<float> m_offset;
};

struct Pattern
{
    Pattern(float cooldown, float rotation_speed,
            std::vector<PatternPart> pattern_parts, std::vector<unsigned int> pattern_parts_lengths)
        : m_cooldown(cooldown), m_rotation_speed(rotation_speed), m_pattern_length(0),
          m_pattern_parts(pattern_parts), m_pattern_parts_lengths(pattern_parts_lengths)
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

};

PatternPart LINEAR_THREE_SHOTS({Path_Def(linear_path)},{3},{0.f});
PatternPart SCYTHE_LINEAR_FOUR_FOUR_SHOTS({Path_Def(scythe_path), Path_Def(linear_path)},{4,4},{0,glm::radians(45.f)});
PatternPart PAUSE({Path_Def(linear_path)},{0},{0});

Pattern level1(0.5f,19.f,{
        LINEAR_THREE_SHOTS,SCYTHE_LINEAR_FOUR_FOUR_SHOTS},{10,10});


#endif
