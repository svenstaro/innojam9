#include "patterns.hpp"

PatternPart::PatternPart(
        std::vector<Path_Def> shot_types,
        std::vector<int> shots_per_cooldown, std::vector<float> offset,
        std::vector<unsigned int> pattern_parts_lengths)
    : m_shot_type(shot_types), m_shots_per_cooldown(shots_per_cooldown),
    m_offset(offset),m_pattern_parts_lengths(pattern_parts_lengths){
    }

Pattern::Pattern(float cooldown, float rotation_speed,
        std::vector<PatternPart> pattern_parts,
        int orbs_needed)
    : m_cooldown(cooldown), m_rotation_speed(rotation_speed),
    m_pattern_parts(pattern_parts),
    m_orbs_needed(orbs_needed) {
    }
