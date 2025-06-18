#pragma once
#include <jps/jump/jump_point_online.h>
#include <bitset>
#include <algorithm>
using namespace jps;
using namespace warthog::domain;

namespace ScanAttribute
{
enum Orientation
{
    CW,
    CCW
};
}

struct scanResult
{
    uint32_t c = {}; //zero count of the comp stride
    uint32_t m = {}; //zero count of the mid stride
    bool top   = {};
    direction d= {};
};

//check if i is inbtween a and b, doesnt matter a or b is larger or smaller
static inline bool between2(uint32_t i, uint32_t a, uint32_t b)
{
    return(i <= std::max(a, b) && i >= std::min(a, b));
}

//mask all bit before the offest(inclusive) to 0, offset is 1-based
template<bool East>
static inline void maskzero(uint64_t &num, uint32_t offset)
{
    if constexpr(East) 
    {
        num &= ((~0ull) << offset);
    }
    else
    {
        num &= ((~0ull) >> offset);
    }
}

static void printEastScan(uint64_t i)
{
    std::string s = std::bitset<64>(i).to_string();
    std::reverse(s.begin(), s.end());
    std::cout<<"->"<<s<<'\n';
}
static void printWestScan(uint64_t i)
{
    std::string s = std::bitset<64>(i).to_string();
    std::reverse(s.begin(), s.end());
    std::cout<<s<<"<-"<<'\n';
}

//return JPS_ID after mooving from a JPS_ID n moves in direction d
inline pad_id shiftInDir(pad_id id, uint32_t n_moves, direction d, gridmap::bittable m)
{
    switch (d)
    {        
    default:
    case direction::EAST:
        return pad_id(id.id + n_moves);
        break;
    case direction::WEST:
        return pad_id(id.id - n_moves);
        break;        
    case direction::NORTH:
        return pad_id(id.id - n_moves * m.width());
        break;
    case direction::SOUTH:
        return pad_id(id.id + n_moves * m.width());
        break;
    case direction::NORTHEAST:
        return pad_id((id.id - n_moves * m.width()) + n_moves);
        break;
    case direction::NORTHWEST:
        return pad_id((id.id - n_moves * m.width()) - n_moves);            
        break;
    case direction::SOUTHEAST:
        return pad_id((id.id + n_moves * m.width()) + n_moves);            
        break;
    case direction::SOUTHWEST:
        return pad_id((id.id + n_moves * m.width()) - n_moves);            
        break;
    }
}


class Scanner
{
public:
    Scanner(jump::jump_point_online<>* _jps);
    ~Scanner(){};

    scanResult init_scan(pad_id start);
    //returns a all convex points of a obstacle
    void scan_obstacle(pad_id start, std::vector<pad_id> &ret, direction dir, bool _top);
    //returns the first poi along obstacle edge
    pad_id scan_obstacle(pad_id start, scanResult &res);

    //scan an obstacle in CW or CCW orientation, returns the first point
    //which results in chang of orientation
    template<ScanAttribute::Orientation o>
    void scan(pad_id parent, pad_id start, pad_id &ret);

    template<bool East>
    uint32_t scan_hori(gridmap::bittable _map, pad_id start, scanResult &res);

    std::vector<pad_id> test_scan_full(pad_id start, uint32_t bx, uint32_t by);
    uint32_t test_scan_single(pad_id start, bool top, char c);
    
    double vecangle(pad_id o, pad_id a, pad_id b);

    uint32_t m_bx;  //x coord of the bounding box
    uint32_t m_by;  //y coord of the bounding box
private:
    uint32_t scan_east(pad_id start, scanResult &res);
    uint32_t scan_west(pad_id start, scanResult &res);
    uint32_t scan_north(pad_id start, scanResult &res);
    uint32_t scan_south(pad_id start, scanResult &res);

    jump::jump_point_online<>* m_jps;
    gridmap::bittable m_map  = {};
	gridmap::bittable m_rmap = {};

    //if d1 and d2 are in different d set of: {E, N}, {W, S}
    inline bool EN_diff_WS(direction d1, direction d2)
    {
        uint8_t a = std::countr_zero(static_cast<uint8_t>(d1));
        uint8_t b = std::countr_zero(static_cast<uint8_t>(d2));
        return (std::max(a, b) - std::min(a, b)) != 2;
    }
};

template<bool East>
uint32_t Scanner::scan_hori(gridmap::bittable _map, pad_id start, scanResult &res)
{
    gridmap_slider slider = gridmap_slider::from_bittable(_map, start);//.id?
    std::array<uint64_t, 3> neis;
    uint32_t steps = 0;
    //[0]middle
    //[1]above
    //[2]below
    if constexpr(!East) 
    {
        slider.adj_bytes(-7);
        slider.width8_bits = (7-slider.width8_bits);
    }
    neis = slider.get_neighbours_64bit_le();
    //get the comparing word based on if the obsticle is on top or bottom 
    uint64_t comp = res.top ? neis[1] : neis[2];
    uint64_t mid = ~neis[0];
    
    std::cout<<"offset: "<<slider.width8_bits<<'\n';
    if constexpr(East)
    {
        maskzero<East>(mid, slider.width8_bits);
        maskzero<East>(comp, slider.width8_bits);
        std::cout<<"mid:    ";
        printEastScan(mid);
        std::cout<<"comp:   ";
        printEastScan(comp);
    }
    else
    {
        maskzero<East>(mid, slider.width8_bits);
        maskzero<East>(comp, slider.width8_bits);
        std::cout<<"mid:    ";
        printWestScan(mid);
        std::cout<<"comp:   ";
        printWestScan(comp);
    }
    if(comp || mid)
    {
        res.m = East? std::countr_zero(mid) : std::countl_zero(mid);
        res.c = East? std::countr_zero(comp) : std::countl_zero(comp);
        steps += std::min(res.m, res.c);
        return steps - slider.width8_bits -1;
    }
    slider.adj_bytes( East? 7:-7);
    steps += 63-slider.width8_bits-1;
    while (true)
    {
    assert(false && "not implemented");
    }    
    return 0;
}

template<ScanAttribute::Orientation o>
void Scanner::scan(pad_id parent, pad_id first, pad_id &ret)
{
    pad_id second;
    scanResult res;
    res.d = SOUTH;
    res.top = true;
    second = scan_obstacle(first, res);
    double angle = vecangle(parent, first, second);
    if constexpr (o == ScanAttribute::CCW) 
    {
        while (angle <= 0)
        {
            first = second;
            second = scan_obstacle(first, res);          
            angle = vecangle(parent, first, second);
        } 
    }
    else if constexpr (o == ScanAttribute::CW) 
    {
        while (angle >= 0)
        {
            first = second;
            second = scan_obstacle(first, res);
            angle = vecangle(parent, first, second);
        } 
    }      
    ret = first;
}