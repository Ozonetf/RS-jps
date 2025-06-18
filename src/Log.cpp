#include <Log.h>

void Tracer::expand(uint32_t x, uint32_t y)
{
    if (adj_for_padding)
    {
        y-=3;
    }
    m_trace << "- { type: " << "expanding" << ", color: yellow" << ", id: " << to_string(x) + ":" + to_string(y) << ", x: " << to_string(x) << ", y: " << to_string(y) << "}\n";
}

void Tracer::expand(std::pair<uint32_t, uint32_t> p)
{
    auto x = uint32_t{p.first}, y = uint32_t{p.second};  
    if (adj_for_padding)
    {
        y-=3;
    }
    m_trace << "- { type: " << "expanding" << ", color: yellow" << ", id: " << to_string(x) + ":" + to_string(y) << ", x: " << to_string(x) << ", y: " << to_string(y) << "}\n";
}

void Tracer::init(pair<uint32_t, uint32_t> start, pair<uint32_t, uint32_t> finish)
{
    m_trace.clear();
    auto sy = uint32_t{start.second}, fy = uint32_t{finish.second};    
    if (adj_for_padding)
    {
        sy-=3; fy-=3;
    }
    m_trace << "version: 1.4.0\n";
    m_trace << "views:\n  main:\n    - $: rect\n      fill: ${{ $.color }}\n      width: 1\n      height: 1\n      x: ${{ $.x }}\n      y: ${{ $.y }}\n";
    m_trace << "events:\n";
    string type;
    type = "source";
    m_trace << "- { type: " << type << ", color: blue" << ", id: " << to_string(start.first) + ":" + to_string(start.second) << ", x: " << start.first << ", y: " << sy << "}\n";
    type = "destination";
    m_trace << "- { type: " << type << ", color: green" << ", id: " << to_string(finish.first) + ":" + to_string(finish.second) << ", x: " << finish.first << ", y: " << fy << "}\n";
}