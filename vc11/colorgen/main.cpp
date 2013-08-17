///////////////////////////////////////////////////////////////////////////////
/// \file   main.cpp
/// \author Benjamin Crist
///
/// \brief  Command-line utility for calculating RGBA8888 integral values
///         from floating point component strengths from 0.0 to 1.0.

#include <iostream>
#include <algorithm>
#include <glm/glm.hpp>

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    glm::vec4 color;

    if (argc >= 4)
    {
        color.r = float(atof(argv[1]));
        color.g = float(atof(argv[2]));
        color.b = float(atof(argv[3]));
        color.a = 1.0f;
    }
    else
    {
        std::cerr << "Usage:" << std::endl
                  << "   " << (argc >= 1 ? argv[0] : "colorgen") << "<r> <g> <b> [a]" << std::endl;
        return -1;
    }

    if (argc >= 5)
        color.a = float(atof(argv[4]));
    
    glm::ivec4 temp(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
    temp.r = std::max(0, std::min(255, temp.r));
    temp.g = std::max(0, std::min(255, temp.g));
    temp.b = std::max(0, std::min(255, temp.b));
    temp.a = std::max(0, std::min(255, temp.a));

    int out = temp.r | (temp.g << 8) | (temp.b << 16) | (temp.a << 24);

    std::cout << out << std::endl;

    return 0;
}
