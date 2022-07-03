
#include "Constants.h"
#include "Utils.h"

#include <iostream>
#include <filesystem>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>



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
            stbir_resize_uint8(img, img_width, img_height, 0, img, new_width, new_height, 0, img_channels);
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


    double map_range(double input, double input_start, double input_end, double output_start, double output_end) {
        return output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start);
    }


}


