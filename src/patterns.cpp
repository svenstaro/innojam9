#include "patterns.hpp"
/*
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
*/

LayerCompound::LayerCompound(std::vector<Path_Def> paths,
                         std::vector<unsigned int> number_of_shots,
                         std::vector<float> offset
                         )
    : m_paths(paths), m_number_of_shots(number_of_shots), m_offset(offset)
{

}

    Stage::Stage(std::vector<LayerCompound> stages,
                 std::vector<unsigned int> repitition_per_stage,
                 float cooldown)
: m_path_layers(stages), m_repititions(repitition_per_stage), m_cooldown(cooldown)
{

}
void Stage::next()
{
    m_current_repition++;
    if(m_current_repition == m_repititions.size())
    {
        m_current_repition = 0;
    }
}
LayerCompound Stage::get_current_repitition()
    {
        return m_path_layers[m_current_repition];
    }

    Level::Level(std::vector<Stage> stages, unsigned int orbs_to_next_level)
:m_stages(stages) , m_orbs_to_next_level(orbs_to_next_level)
{
    m_number_of_stages = stages.size();
}
bool Level::is_at_end()
{
    if(m_current_stage == 0)
    {
        return true;
    }
    return false;
}

bool Stage::is_at_end()
{
    if(m_current_repition == 0)
    {
        return true;
    }
    return false;
}
void Level::next()
{
    m_current_stage++;
    if(m_current_stage == m_number_of_stages)
    {
        m_current_stage = 0;
    }
}

Stage Level::get_current_stage()
{
    std::cout << m_current_stage << "HEER IS DEBUG CODE FINDE MICHT" << std::endl;
    return m_stages[m_current_stage];
}
