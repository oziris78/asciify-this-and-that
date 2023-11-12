/*
* Copyright 2023 Oğuzhan Topaloğlu
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <imgui.h>



namespace twistral {
    static std::string ASCII = std::string("Ñ@#W$9876543210?!abc;:+=-,._ ");
    static const int32_t CHAR_SET_SIZE = ASCII.size();

    std::string get_ascifiied(const std::string& image_path);
    
    double map_range(double input, double input_start, double input_end, double output_start, double output_end);
}


