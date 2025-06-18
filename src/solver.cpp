#include <solver.h>

Solver::Solver(jump::jump_point_online<>* _jps) : 
m_jps(_jps), m_tracer(new Tracer), m_map(m_jps->get_map()), m_rmap(m_jps->get_rmap()), m_ray(m_tracer, m_jps), m_scanner(m_jps)
{
    // m_map = _jps->get_map();
    // m_rmap = _jps->get_rmap();
    // m_jps = _jps;
}

inline Attributes::Quadrant Solver::target_quadtrant(pad_id start, pad_id target)
{
    auto c1 = m_map.id_to_xy(start), c2 = m_map.id_to_xy(target);
    uint32_t x = c1.first, y = c1.second, tx = c2.first, ty = c2.second;
    if(tx<x)
    {
        if(ty<y) return Attributes::North_West;
        else     return Attributes::South_West;
    }
    else
    {
        if(ty<y) return Attributes::North_East;
        else     return Attributes::South_East;
    }
}

void Solver::test_func(pad_id id1, pad_id id2)
{
    m_tracer->init(m_map.id_to_xy(id1), m_map.id_to_xy(id2));
    auto a = m_ray.shoot_to_target(id1, id2);
    m_tracer->expand(m_map.id_to_xy(a));
}