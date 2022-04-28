
#include "Constants.h"
#include "Utils.h"

#include <iostream>
#include <filesystem>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


namespace telek {

    std::string get_ascifiied(const std::string& image_path) {
        std::filesystem::path path(image_path);
        std::string extension = path.extension().string();
        if (!(extension == ".jpg" || extension == ".jpeg" || extension == ".png"))
            return "Unsupported file extension, \nsupported ones are \n.jpg, .jpeg and .png";

        int img_width, img_height, img_channels;
        uint8_t* img = stbi_load(image_path.c_str(), &img_width, &img_height, &img_channels, 0);

        if (img == nullptr)
            return "Image file was not found";

        const size_t img_size = img_width * img_height * img_channels;
        uint8_t* p = img;

        std::string** arr = new std::string * [img_height];
        for (int i = 0; i < img_height; i++) {
            arr[i] = new std::string[img_width];
            for (int j = 0; j < img_width; j++) {
                uint8_t red = *p;
                uint8_t green = *(p + 1);
                uint8_t blue = *(p + 2);
                uint8_t average = (red + green + blue) / 3.0;

                uint8_t char_index = floor(telek::map_range(average, 0, 255, 0, telek::CHAR_SET_SIZE));
                arr[i][j] = telek::ASCII[char_index];

                // iterate to the next
                p += img_channels;
            }
        }

        std::string result = std::string();
        for (int i = 0; i < img_height; i++) {
            for (int j = 0; j < img_width; j++) {
                result.append(arr[i][j]);
            }
            result.append("\n");
        }

        stbi_image_free(img);

        return result;
    }


    double map_range(double input, double input_start, double input_end, double output_start, double output_end) {
        return output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start);
    }


}


