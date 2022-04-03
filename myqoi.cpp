#include <fstream>
#include <string>
#include <iostream>
#include <cinttypes>
#include <vector>
#include <array>

#include "qoi.h"

struct pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

inline int hash(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    return (red*3 + green*5 + blue*7 + alpha*11) % 64;
}

int main(int argc, char const *argv[]) {
    std::vector<std::vector<uint8_t>> testData = {
        {0, 100, 0, 100}, 
        {150, 50, 150, 50}, 
        {100, 200, 100, 200}, 
        {250, 150, 250, 150}
    };

    uint8_t alpha = 255;
    const uint8_t STARTING = 0;

    struct qoi_header header;
    header.magic;
    header.width = 4;
    header.height = 4;
    header.channels = 1;
    header.colorspace = 1;
    
    std::array<uint8_t, 64> previouslySeen {};

    for (size_t row = 0; row < testData.size(); row++)
    {
        for (size_t col = 0; col < testData[0].size(); col++)
        {
            /* code */
        }
        
    }
    

    return 0;
}
