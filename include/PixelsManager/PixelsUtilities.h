#ifndef _PIXELS_UTILITIES_H_INCLUDED_
#define _PIXELS_UTILITIES_H_INCLUDED_

#include <cmath>
#include <vector>
#include <numeric>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <exception>
#include <functional>

/**
 * @namespace PixelsUtilities
 * @brief a set of usefull methods for Pixels Managing.
 */
namespace PixelsUtilities
{    
    /*
     * Mathematics methods
     */

    double calculate_variance(std::vector<double> &set);
    uint8_t PaethPredictor(uint8_t left, uint8_t up, uint8_t upperLeft);
    long long unsigned int combination(long long unsigned int p, long long unsigned int n);
    void get_subsets(std::vector<double> &set_in, int subset_size, int start_index, std::vector<double> tmp_vec, std::vector<std::vector<double>> &out);


    /*
     * Pixels Buffer methods
     */

    void flipPixels(uint8_t *pixelsBuffer, int s_width, int s_heigth, int colorChannel);
    uint8_t *get_rgb_at(const uint8_t *rgb_in, int actual_position, int s_width, int s_height, int relative_row, int relative_col);
    uint8_t get_luminance_at(const uint8_t *gray_in, int actual_position, int s_width, int s_height, int relative_row, int relative_col);
    std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> get_rgb_possibilities(std::function<bool(const uint8_t, const uint8_t, const uint8_t)> f) noexcept;


    /**
     * @class Kmean_point
     * @brief point class definition for kmean method implementation
     * 
     */
    class Kmean_point
    {
        public :
            uint8_t r, g, b;
            int cluster;
            double minDist;

            Kmean_point() : r(0), g(0), b(0), cluster(-1), minDist(__DBL_MAX__) {}
            Kmean_point(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), cluster(-1), minDist(__DBL_MAX__) {}

            double distance(Kmean_point p) const
            {
                return (p.r - r) * (p.r - r) + (p.g - g) * (p.g - g) + (p.b - b) * (p.b - b);
            }
    };

    void kMeansClustering(std::vector<PixelsUtilities::Kmean_point> &points, int iters, int nb_clusters);
    uint8_t *get_rgb_part(const uint8_t *rgb_in, int rgb_len, int s_width, int s_height, int x_start, int y_start, int x_end, int y_end);

};

#endif //_PIXELS_UTILITIES_H_INCLUDED_