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
    unsigned int m_number_of_shots_done = 0;
    std::vector<Path_Def> m_paths;
    std::vector<unsigned int> m_number_of_shots;
    std::vector<float> m_offset;
    
    static LayerCompound SIN_TWO_SHOTS()
    {
        return {{Path_Def(sin_path)}, {2}, {0.f}}; 
    }
   
    static LayerCompound PAUSE()
    {
        return {{Path_Def(sin_path)}, {0},{0.f}};
    }
    
    static LayerCompound SIN_TWO_SHOTS_FIVE_DEF_OFFSET()
    {
        return {{Path_Def(sin_path)}, {2}, {5.f}}; 
    }

    static LayerCompound SIN_FOUR_SHOTS()
    {
        return {{Path_Def(sin_path)}, {4}, {0.f}}; 
    }

    /*
     *  Parameter 1:
     *      vetcor von path funtionen die zusammen das Compound ergeben
     *  Parameter 2:
     *      vector von unsigned ints. Fuer jede funtion von vorherigen 
     *      element wird hier die anzahl von schuessen festgelegt
     *  Parameter 3:
     *      vector von offsets. geben an wie weit die path funktion
     *      verschobnen wird.
     * */
    static LayerCompound SIN_SCYTHE_FOUR_TWO()
    {
        return {{Path_Def(sin_path), Path_Def(scythe_path)},{4,4},{0,0.f}};
    }

    static LayerCompound SPIRAL_ONE()
    {
        return {{Path_Def(spiral_path)},{1},{0.f}};
    }

    static LayerCompound DUAL_TWIN()
    {
        return {{Path_Def(linear_path), Path_Def(linear_path)}, {2, 2}, {0.f, 5.f}};
    }

    static LayerCompound FOUR_TWIN()
    {
        return {{Path_Def(linear_path), Path_Def(linear_path), Path_Def(linear_path), Path_Def(linear_path)}, {4, 4, 4, 4},{0.f,5.f,10.f,15.f}};
    }

    static LayerCompound LINEAR_TWO_SIN_TWO()
    {
        return {{Path_Def(linear_path), Path_Def(sin_path)}, {10, 4}, {0.f, 2.f}};
    }

    static LayerCompound LINEAR_TWO()
    {
        return {{Path_Def(linear_path)}, {2}, {0.f}};
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
        /*
         *  parameter 1: vektor von Compunds die nacheinander 
         *      ausgefuehrt werden
         *  Parameter 2:
         *      fuer jedes Compound wird hier angegeben wie oft
         *      es ausgefuehrt werden soll bevor das naechste
         *      ausgefuehrt wirf
         *  Parameter 3:
         *      allgemeiner cooldown. wie schnell werden die schuesse
         *      abgegeben
         * */
        return {{LayerCompound::SIN_SCYTHE_FOUR_TWO(),LayerCompound::SIN_TWO_SHOTS()}, {2, 2}, 1.f};   
    }
    static Stage LINEAR_TWO_PULSED()
    {
        return {{LayerCompound::LINEAR_TWO(),LayerCompound::PAUSE()},{3,3},1.f};
    }

    static Stage LINEAR_TWO_PULSED_TWIN()
    {
        return {{LayerCompound::DUAL_TWIN(),LayerCompound::PAUSE()},{3,2},0.7f}; 
    }

    static Stage LINEAR_FOUR_PULSED_TWIN()
    {
        return {{LayerCompound::FOUR_TWIN(),LayerCompound::PAUSE()},{3,2},0.7f}; 
    }

    static Stage STS_ONE()
    {
        return {{LayerCompound::SIN_TWO_SHOTS()}, {1}, 1.f};
    }

    static Stage LINEAR_TWO_SIN_TWO()
    {
        return {{LayerCompound::LINEAR_TWO_SIN_TWO()}, {1}, 1.f};
    }

    static Stage SIN_TWO()
    {
        return {{LayerCompound::SIN_TWO_SHOTS()}, {1}, 1.f};
    }

    static Stage SPIRAL_ONE()
    {
        return{{LayerCompound::SPIRAL_ONE()}, {4}, 1.f};   
    }

    static Stage LINEAR_TWO()
    {
        return {{LayerCompound::LINEAR_TWO()}, {4}, 1.f};
    }

};

struct Level
{
    Level(std::vector<Stage> m_stages,  unsigned int orbs_to_next_level, float rotations_speed);
    unsigned int m_number_of_stages;
    std::vector<Stage> m_stages;
    unsigned int m_orbs_to_next_level;
    unsigned int m_current_stage = 0;
    float m_rotation_speed = 0;

    Stage get_current_stage();

    bool is_at_end();
    void next();

    static Level LEVEL_ONE()
    {
        /*
         * Parameter 1: 
         *      vektor von Stages die nacheinander ausgefuehrt werden
         *  Parameter 2:
         *      anzahl von orbs die eingesammelt werden muessen um den
         *      level abzuschlieszen.
         * */
        return{{Stage::LINEAR_TWO()}, 5, glm::radians(20.f)};
    }

    static Level LEVEL_TWO()
    {
        return{{Stage::LINEAR_TWO_PULSED()}, 5, glm::radians(20.f)};
    }

    static Level LEVEL_THREE()
    {
        return{{Stage::LINEAR_TWO_PULSED_TWIN()}, 5, glm::radians(20.f)};
    }

    static Level LEVEL_FOUR()
    {
        return{{Stage::LINEAR_FOUR_PULSED_TWIN()}, 5, 20.f};
    }

    static Level LEVEL_FIVE()
    {
        return{{Stage::SIN_TWO()}, 5, 20.f};
    }

    static Level LEVEL_SIX()
    {
        return{{Stage::LINEAR_TWO_SIN_TWO()}, 5, 20.f};
    }

    static Level LEVEL_SEVEN()
    {
        return{{Stage::SPIRAL_ONE()}, 5, 20.f};
    }

    static Level LEVEL_EIGHT()
    {
        return{{Stage::SPIRAL_ONE()}, 20, 20.f};
    }
};
#endif
