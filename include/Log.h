#pragma once
#include <rjps.h>
#include <string>
#include <fstream>
#include <fstream>
#include <iostream>
// #include <array>
// #include <jps/forward.h>
// #include <memory>
// #include <warthog/domain/gridmap.h>

class Tracer
{
private:
    const string                        m_fileName;
    ofstream                            m_trace;
    warthog::memory::bittable_dimension m_dim{1000, 1000};
    const bool adj_for_padding = true;

public:
    Tracer(string _fname) : m_fileName(_fname), m_trace(_fname){};
    Tracer() : m_trace(ofstream{"myTrace.trace.yaml"}) {};
    ~Tracer() {};

    void set_dim(warthog::memory::bittable_dimension _dim){m_dim = _dim;};

    void init(point start, point finish);
    void close();
    
    void expand(uint32_t x, uint32_t y, string color, string type);
    void expand(point p, string color, string type);
    //draws a cell that clears after 1 step
    void draw_cell(point p, string color, string type);
    void close_node(point p);

    //draws a 2d ray from start to finish, clears after 1 step
    void trace_ray(point start, point finish, string color, string type);
    //draws a 2d ray from start to finish, clears on a close trace with the matching closeid
    void trace_ray_till_close(point closeid, point start, point finish, string color, string type);
    void draw_bounds(point p, direction_id dir);
};
