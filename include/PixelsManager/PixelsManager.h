#ifndef _PIXELS_MANAGER_H_INCLUDED_
#define _PIXELS_MANAGER_H_INCLUDED_

#include <map>
#include <cmath>
#include <tuple>
#include <thread>
#include <future>
#include <vector>
#include <cstring>
#include <numeric>
#include <cstdarg>
#include <iostream>
#include <algorithm>
#include <exception>
#include <functional>

/**
 * @namespace PixelsManager 
 * @brief a set of usefull method for managing, transforming and manipulating pixels buffer
 */
namespace PixelsManager
{
    /*
     * Converting methods
     */
    
    double *rgb_to_hsl(const uint8_t *rgb_in, int rgb_len);
    uint8_t *hsl_to_rgb(const double *hsl, int hsl_len);


    /*
     * Algorithms
     */

    uint8_t *grayscale_to_otsu(const uint8_t *gray_in, int gray_len, int &bin_len); // Otsu Nobuyuki Algorithm for Binarisation
    uint8_t *get_dominants_colors_kmean(const uint8_t *rgb_in, int rgb_len, int nb_colors, int iters, int &nb_colors_out); // Kmean clustering Algorithm for color detection

    /*
     * Color Mode Management
     */

    uint8_t *rgb_to_grayscale(const uint8_t *rgb_in, int rgb_len, int mode, int &gray_len); 
    uint8_t *rgb_to_channel(const uint8_t *rgb_in, int rgb_len, int channel, int &ch_len);
    uint8_t *rgb_to_channel_s(const uint8_t *rgb_in, int rgb_len, int channel, int &ch_len);
    uint8_t *rgba_to_rgb(const uint8_t *rgba_buffer, int bufferLen, int &rgb_len);
    uint8_t *mixChannels(int nb_ch, int size_ch, ...);


    /*
     * Color buffer Analysis 
     */

    inline std::vector<int> getHistogram(const uint8_t *buffer_in, int buffer_len) noexcept;
    uint8_t *get_high_occ_colors(const uint8_t *rgb_in, int rgb_len, int nb_colors);
    int get_nb_colors(const uint8_t *rgb_in, int rgb_len);


    /*
     * Features in development
     */

    uint8_t *lut_to_rgb(const uint8_t *gray_in, int gray_len, const uint8_t *rgb_lut, int lut_len);
    uint8_t *lut_to_rgb_thread(const uint8_t *gray_in, int gray_len, const uint8_t *rgb_lut, int lut_len, int thread_number);
    uint8_t *overscreen_color(const uint8_t *rgb_in, int rgb_len, const uint8_t *rgb_toscreen, int rgb_toscreen_len);
    
    /**
     * @namespace gray_level
     */
    namespace gray_level
    {
        /**
         * @enum set of grayscale levels for computing rgb to grayscale method
         */
        enum gray_level { BRIGHTER = 0x0, LIGHTER = 0x1, AVERAGE = 0x2, DEFAULT = 0x4};
    }

    /**
     * @namespace color_channel 
     */
    namespace color_channel
    {   
        /** 
         *@enum set of possible channels that can be got when getting a single channel 
         */
        enum color_channel { RED = 0x5, GREEN = 0x6, BLUE=0x7 };
    }
};

/**
 * @brief Get the Max object
 * 
 * @tparam T 
 * @param n1 
 * @param n2 
 * @param n3 
 * @return T 
 */
template <typename T>
inline T getMax(const T &n1, const T &n2, const T &n3)
{
    return std::max(std::max(n1, n2), std::max(n2, n3));
}

/**
 * @brief Get the Min object
 * 
 * @tparam T 
 * @param n1 
 * @param n2 
 * @param n3 
 * @return T 
 */
template <typename T>
inline T getMin(const T &n1, const T &n2, const T &n3)
{
    return std::min(std::min(n1, n2), std::min(n2, n3));
}


#endif //_PIXELS_MANAGER_H_INCLUDED_