#pragma once
#include <jps/jump/jump_point_online.h>
#include <scanner.h>
#include <Ray.h>
#include <Log.h>
using namespace warthog::domain;
using namespace jps;

template<direction d>
static inline std::pair<int, int> get_adj()
{
    static_assert(d==NORTHWEST || d==NORTHEAST || d==SOUTHWEST || d==SOUTHEAST);
    switch (d)  
    {
    case NORTHWEST:
        return std::pair<int, int>{-1, -1};
        break;
    case NORTHEAST:
        return std::pair<int, int>{1, -1};
        break;
    case SOUTHWEST:
        return std::pair<int, int>{-1, 1};
        break;
    case SOUTHEAST:
        return std::pair<int, int>{1, 1};
        break;
    }
}

class Solver
{
public:
    Solver(jump::jump_point_online<>* _jps);
    ~Solver() = default;
    
    void test_func(pad_id id1, pad_id id2);
private:
    jump::jump_point_online<>*          m_jps;
    std::shared_ptr<Tracer>             m_tracer;
    warthog::domain::gridmap::bittable  m_map;
    warthog::domain::gridmap::bittable  m_rmap;
    Ray                                 m_ray;
    Scanner                             m_scanner;

    inline Attributes::Quadrant target_quadtrant(pad_id start, pad_id target);
};

// template<bool East>
// uint32_t Solver::shoot_ray_hori(pad_id start, gridmap::bittable map)
// {
//     uint32_t steps = 0;
//     auto slider = gridmap_slider::from_bittable(map, start);
//     if constexpr(!East) 
//     {
//         slider.adj_bytes(-7);
//         slider.width8_bits = (7-slider.width8_bits);
//     }
//     uint64_t mid = ~slider.get_neighbours_64bit_le()[0];
//     maskzero<East>(mid, slider.width8_bits);
//     if(mid)
//     {
//         steps = East? std::countr_zero(mid) : std::countl_zero(mid);
//         return steps - slider.width8_bits - 1;
//     }
//     slider.adj_bytes(East? 7 : -7);
//     steps += 63 - slider.width8_bits -1;
//     while (true)
//     {
//         if(mid)
//         {
//             steps = East? std::countr_zero(mid) : std::countl_zero(mid);
//             return steps - slider.width8_bits;
//         }
//         slider.adj_bytes(East? 7 : -7);
//         steps += 63;
//     }    
// }

