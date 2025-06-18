#include <Ray.h>

pad_id Ray::shoot_to_target(pad_id start, pad_id target)
{
    int curx = m_map.id_to_xy(start).first;
    int cury = m_map.id_to_xy(start).second;
    int tx = m_map.id_to_xy(target).first;
    int ty = m_map.id_to_xy(target).second;
    int adjx, adjy;
    if(curx < tx)   //TODO: optimize this
    {
        if (cury < ty)
        {
            adjx = 1; adjy = 1;
        } 
        else 
        {
            adjx = 1; adjy = -1;
        }
    }
    else    
    {
        if (cury < ty) 
        {
            adjx = -1; adjy = 1;
        }
        else 
        {
            adjx = -1; adjy = -1;
        }
    }

    while(curx != tx && cury != ty)
    {
        auto next = m_map.get(m_map.xy_to_id(curx+adjx, cury+adjy));
        //stops if next cell is blocked
        if(next == 0)
        {
            return m_map.xy_to_id(curx, cury);
        }
        else 
        {
            curx+=adjx; cury+=adjy;
            m_tracer->expand(curx, cury);
        }
    }

    start = m_map.xy_to_id(curx, cury);
    if(curx == tx)
    {
        if(cury < ty)
        {
            uint32_t steps = shoot_ray_south(start);
            m_tracer->expand(curx, cury + steps);
            if(cury + steps >= ty)
            {
                return target;
            }
            else return shiftInDir(start, steps, SOUTH, m_map);
        }
        else    
        {
            uint32_t steps = shoot_ray_north(start);
            m_tracer->expand(curx, cury - steps);
            if(cury - steps <= ty)
            {
                return target;
            }
            else return shiftInDir(start, steps, NORTH, m_map);
        }
    }
    else //(cury == ty)
    {
        if(curx < tx)
        {
            uint32_t steps = shoot_ray_east(start);
            m_tracer->expand(curx + steps, cury);
            if(curx + steps > tx)
            {
                return target;
            }
            else return shiftInDir(start, steps, EAST, m_map);
        }
        else    
        {
            uint32_t steps = shoot_ray_west(start);
            m_tracer->expand(curx - steps, cury);
            if(curx - steps > tx)
            {
                return target;
            }
            else return shiftInDir(start, steps, WEST, m_map);
        }
    }
}
