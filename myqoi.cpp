#include <fstream>
#include <ostream>
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
bool operator==(const struct pixel& lhs, const struct pixel& rhs) {
        return lhs.red == rhs.red && lhs.green == rhs.green && lhs.blue == rhs.blue && lhs.alpha == rhs.alpha;
    }
typedef std::vector<std::vector<struct pixel>> pixelmatrix;

inline int hash(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    return (red*3 + green*5 + blue*7 + alpha*11) % 64;
}
inline int hash(struct pixel p) {
    return (p.red*3 + p.green*5 + p.blue*7 + p.alpha*11) % 64;
}

void encode (std::ostream& output, pixelmatrix input, bool hasAlpha, uint8_t colorspace) {
    // Input must be a rectangular matrix
    // Currently only works for input with alpha
    // Return an array of the encoded bytes
    std::array<struct pixel, 64> previouslySeen {};
    size_t index_position = 0;
    uint8_t runLength = 0;
    bool lastWasIndex = false;
    struct pixel lastPixel;
    lastPixel.red = 0;
    lastPixel.green = 0;
    lastPixel.blue = 0;
    lastPixel.alpha = 255;

    uint32_t height = input.size();
    uint32_t width = input[0].size();

    // header is big endian
    // header.height = htonl(height);
    // header.width = htonl(width);

    // save header
    output.put('q');
    output.put('o');
    output.put('i');
    output.put('f');
    output.put(*((char*)(&width) + 3));
    output.put(*((char*)(&width) + 2));
    output.put(*((char*)(&width) + 1));
    output.put(*(char*)(&width));
    output.put(*((char*)(&height) + 3));
    output.put(*((char*)(&height) + 2));
    output.put(*((char*)(&height) + 1));
    output.put(*(char*)(&height));
    output.put((hasAlpha ? 4 : 3));
    output.put(colorspace);

    for (size_t row = 0; row < height; row++)
    {
        for (size_t col = 0; col < width; col++)
        {
            struct pixel curr = input[row][col];
            index_position = hash(curr);
            struct pixel savedPixel = previouslySeen[index_position];

            if (lastPixel == curr) {
                // Run found. Skip hash?
                runLength++;
                continue;
            }

            bool wasSeen = (savedPixel == curr);
            // Store as 8 bit tag is not seen, else 2 bit tag
            if (!wasSeen) {
                if (runLength > 0) {
                    output.put(0xC0 | (runLength-1));
                    runLength = 0;
                }
                previouslySeen[index_position] = curr;
                output.put(0xFF);
                output.put(curr.red);
                output.put(curr.green);
                output.put(curr.blue);
                output.put(curr.alpha);
                lastWasIndex = false;
            }
            // else if (lastWasIndex) {
            //     // Can't store two indices in a row
            //     // Find differences:
            //     uint8_t dg = 
            //     lastWasIndex = false;
            // }
            else {
                // Store index
                output.put(index_position);
                lastWasIndex = true;
            }

            lastPixel = curr;
        }
        
    }

    // Terminate stream
    output.put(0);
    output.put(0);
    output.put(0);
    output.put(0);
    output.put(0);
    output.put(0);
    output.put(0);
    output.put(0x01);
    output.flush();
}

int main(int argc, char const *argv[]) {

    // Generate test data
    std::vector<std::vector<struct pixel>> testData(4, std::vector<struct pixel>(4));
    {
        int counter = 0;
        for (auto &&row : testData) {
            for(auto &&col : row) {
                col.red = 50*counter % 256;
                col.green = 50*counter % 256;
                col.blue = 50*counter % 256;
                col.alpha = 255;
                counter++;
            }
        }
    }
    

    const struct pixel STARTING = {0, 0, 0, 255};

    struct qoi_header header;
    header.magic;
    header.width = testData[0].size();
    header.height = testData.size();
    header.channels = 1;
    header.colorspace = 1;

    
    {
        auto out = std::ofstream("testout.qoi", std::ios::out | std::ios::binary);
        if (!out.is_open()) {
            std::cout << "Failed to open file" << std::endl;
        }
        encode(out, testData, true, 0);
    }
    

    return 0;
}
