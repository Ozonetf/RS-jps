#pragma once
#include <string>
#include <fstream>
#include <fstream>
#include <iostream>
#include <array>
#include <jps/forward.h>
#include <memory>
#include <warthog/domain/gridmap.h>

using namespace std;

class Tracer
{
private:
    const string m_fileName;
    ofstream m_trace;
    const bool adj_for_padding = true;
public:
    Tracer(string _fname) : m_fileName(_fname) 
    { 
        // myptr = std::make_shared<ofstream>(m_fileName);
        m_trace = ofstream{m_fileName};
    };
    Tracer() : m_trace(ofstream{"myTrace.trace.yaml"}) {};
    ~Tracer() {    m_trace.close();};

    void init(pair<uint32_t, uint32_t> start, pair<uint32_t, uint32_t> finish);
    void expand(uint32_t x, uint32_t y);
    void expand(std::pair<uint32_t, uint32_t> p);
    void path(uint32_t x, uint32_t y, uint32_t x2, uint32_t y2);
};
