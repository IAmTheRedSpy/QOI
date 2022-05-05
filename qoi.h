#pragma once
#include <cinttypes>

// As per specification document
struct qoi_header
{
    char        magic[4];   // magic bytes "qoif"
    uint32_t    width;      // image width in pixels (BE)
    uint32_t    height;     // image height in pixels (BE)
    uint8_t     channels;   // 3 = RGB, 4 = RGBA
    uint8_t     colorspace; // 0 = sRGB with linear alpha
                            // 1 = all channels linear
};

struct pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};
bool operator==(const struct pixel& lhs, const struct pixel& rhs);
typedef std::vector<std::vector<struct pixel>> pixelmatrix;

inline int hash(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
inline int hash(struct pixel p);

void op_rgb (std::ostream& output, struct pixel& curr);
void op_rgba (std::ostream& output, struct pixel& curr);
void op_index (std::ostream& output, size_t index_position);
void op_run (std::ostream& output, uint8_t& runLength);
