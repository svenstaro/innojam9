
#ifndef COMPONENT_LIFETIMME_HPP
#define COMPONENT_LIFETIMME_HPP

#include "entityx/entityx.h"

struct LifeTime : entityx::Component<LifeTime> 
{
    LifeTime(float life_time) : m_life_time(life_time), m_current_time_lived(0.0f){}

    float m_life_time;
    float m_current_time_lived;

    float get_current_time_lived()
    {
        return m_current_time_lived;
    }
    
    float get_life_time()
    {
        return m_life_time;
    }
    void update(float dt)
    {
        m_current_time_lived += dt;
    }

};


#endif
