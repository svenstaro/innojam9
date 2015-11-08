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

struct LayerCompound
{
    LayerCompound(std::vector<Path_Def> paths,
                std::vector<unsigned int> number_of_shots,
                std::vector<float> offset);
    unsigned int m_number_of_paths;
    std::vector<Path_Def> m_paths;
    std::vector<unsigned int> m_number_of_shots;
    std::vector<float> m_offset;
    
    static LayerCompound SIN_TWO_SHOTS()
    {
        return {{Path_Def(sin_path)}, {2}, {0.f}}; 
    }

    static LayerCompound SIN_SCYTE_FOUR_TWO()
    {
        return {{Path_Def(sin_path), Path_Def(scythe_path)},{4,4},{0,45.f}};
    }
};

struct Stage
{
    Stage(std::vector<LayerCompound> stages, std::vector<unsigned int> repititions,float cooldown);
    std::vector<LayerCompound> m_path_layers;
    std::vector<unsigned int> m_repititions;
    float m_cooldown;
    unsigned int m_current_repition = 0;
    LayerCompound get_current_repitition();

    bool is_at_end();
    void next();

    static Stage SSFT_SIX_STS_FOUR()
    {
        return {{LayerCompound::SIN_SCYTE_FOUR_TWO(),LayerCompound::SIN_TWO_SHOTS()},{6,4},1.f};   
    }

    static Stage STS_ONE()
    {
        return {{LayerCompound::SIN_TWO_SHOTS()},{1},1.f};
    }
};

struct Level
{
    Level(std::vector<Stage> m_stages,  unsigned int orbs_to_next_level);
    unsigned int m_number_of_stages;
    std::vector<Stage> m_stages;
    unsigned int m_orbs_to_next_level;
    unsigned int m_current_stage = 0;

    Stage get_current_stage();

    bool is_at_end();
    void next();

    static Level LEVEL_ONE()
    {
        return{{Stage::SSFT_SIX_STS_FOUR()},5};
    }

};

/*
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
   return {1.f,20.f,{PatternPart::SIN_TWO_SHOTS()},2};
   }
   static Pattern level2() {
   return {1.f,20.f,{PatternPart::SCYTHE_FOUR_SHOT()},2};
   }
   static Pattern level3() {
   return {1.f,40.f,{PatternPart::SCYTHE_FOUR_SHOT_SIN_TWO_SHOTS(),PatternPart::SCYTHE_FOUR_SHOT()},10};
   }



   float m_cooldown;
   float m_rotation_speed;
   std::vector<PatternPart> m_pattern_parts;
   int m_orbs_needed;
   };
   */
#endif
