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


// my preprocessors
#include "Utils.h"

// C++ preprocessors
#include <iostream>
#include <filesystem>
#include <string>

// STB preprocessors
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize2.h>



namespace twistral {

    std::string get_ascifiied(const std::string& image_path) {
        // check extension
        std::filesystem::path path(image_path);
        std::string extension = path.extension().string();
        if (!(extension == ".jpg" || extension == ".jpeg" || extension == ".png"))
            return "Unsupported file extension, \nsupported ones are \n.jpg, .jpeg and .png";

        // get image file
        int img_width, img_height, img_channels;
        uint8_t* img = stbi_load(image_path.c_str(), &img_width, &img_height, &img_channels, 0);

        if (img == nullptr)
            return "Image file was not found";

        // resize image down
        static const double MAX_WIDTH = 90.0;
        if (img_width >= MAX_WIDTH) {
            double ratio = img_width / MAX_WIDTH;
            int new_width = MAX_WIDTH;
            int new_height = img_height / ratio;
            stbir_resize_uint8_linear(img, img_width, img_height, 0, img, new_width, new_height, 0, (stbir_pixel_layout) img_channels);
            img_width = new_width;
            img_height = new_height;
        }

        // create a string using the image file
        uint8_t* p = img;
        std::string result = std::string();
        for (int i = 0; i < img_height; i++) {
            for (int j = 0; j < img_width; j++) {
                uint8_t red = *p;
                uint8_t green = *(p + 1);
                uint8_t blue = *(p + 2);
                uint8_t average = (red + green + blue) / 3.0;

                uint8_t char_index = floor(twistral::map_range(average, 0, 255, 0, twistral::CHAR_SET_SIZE));
                result += twistral::ASCII[char_index];

                // iterate to the next
                p += img_channels;
            }
            result += "\n";
        }

        stbi_image_free(img);
        return result;
    }


    double map_range(double x, double in_start, double in_end, double out_start, double out_end) {
        return out_start + ((out_end - out_start) / (in_end - in_start)) * (x - in_start);
    }


}


