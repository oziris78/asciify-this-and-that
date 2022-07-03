#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <imgui.h>



namespace twistral {

    std::string get_ascifiied(const std::string& image_path);
    
    double map_range(double input, double input_start, double input_end, double output_start, double output_end);
}


