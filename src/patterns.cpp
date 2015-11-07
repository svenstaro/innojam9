#include "patterns.hpp"

PatternPart::PatternPart(
    std::vector<std::function<glm::vec2(glm::vec2, float, float)>> shot_types,
    std::vector<int> shots_per_cooldown, std::vector<float> offset)
    : m_shot_type(shot_types), m_shots_per_cooldown(shots_per_cooldown),
      m_offset(offset) {}

Pattern::Pattern(float cooldown, float rotation_speed,
                        std::vector<PatternPart> pattern_parts,
                        std::vector<unsigned int> pattern_parts_lengths,
                        int orbs_needed)
    : m_cooldown(cooldown), m_rotation_speed(rotation_speed),
      m_pattern_length(0), m_pattern_parts(pattern_parts),
      m_pattern_parts_lengths(pattern_parts_lengths),
      m_orbs_needed(orbs_needed) {
    for (unsigned int i = 0; i < m_pattern_parts.size(); i++) {
        m_pattern_length += m_pattern_parts_lengths[i];
    }
}
