
#include "../../include/PNG/Utilities.h"
#include "../../include/PixelsManager/PixelsManager.h"
#include "../../include/PixelsManager/PixelsUtilities.h"

/**
 * @brief converting a rgb input buffer to a grayscale buffer
 *
 * @details it consit for each pixel sample(red-green-blue) to determine the luminance value.
 * in the output, every rgb value should replaced by the related luminance value.
 *
 * @param rgb_in the rgb input buffer
 * @param rgb_len the rgb buffer size
 * @param mode indicate how the grayscale ouput should be, each mode implement a different luminace value calculation
 * @param gray_len a reference to the grayscale buffer ouptut size
 *
 * @return uint8_t* output converted grayscale buffer
 * @exception std::invalid_argument case none of the avaible gray convertion mode selected
 * @exception std::bad_alloc case the output buffer memory allocation failed
 * @see PixelsManager::gray_level
 */
uint8_t *PixelsManager::rgb_to_grayscale(const uint8_t *rgb_in, int rgb_len, int mode, int &gray_len)
{
    uint8_t *grayscaleBuffer = new uint8_t[(gray_len = (rgb_len / 3))]; // output buffer
    if (!grayscaleBuffer)
        throw std::bad_alloc();

    switch (mode) // each mode perform a different grayscale computing method
    {
    case gray_level::AVERAGE:
        for (std::size_t i = 0, j = 0; i < rgb_len; i += 3, j++)
            grayscaleBuffer[j] = (rgb_in[i] + rgb_in[i] + rgb_in[i + 2]) / 3;

        return grayscaleBuffer;
        break;

    case gray_level::BRIGHTER:
        for (std::size_t i = 0, j = 0; i < rgb_len; i += 3, j++)
            grayscaleBuffer[j] = floor((getMax(rgb_in[i], rgb_in[i], rgb_in[i + 2]) +
                                        getMin(rgb_in[i], rgb_in[i], rgb_in[i + 2])) /
                                       2);

        return grayscaleBuffer;
        break;

    case gray_level::LIGHTER:
        for (std::size_t i = 0, j = 0; i < rgb_len; i += 3, j++)
            grayscaleBuffer[j] = rgb_in[i] * 0.21 + rgb_in[i] * 0.71 + rgb_in[i + 2] * 0.07;

        return grayscaleBuffer;
        break;

    case gray_level::DEFAULT:
        for (std::size_t i = 0, j = 0; i < rgb_len; i += 3, j++)
            grayscaleBuffer[j] = rgb_in[i] * 0.299 + rgb_in[i] * 0.587 + rgb_in[i + 2] * 0.114;

        return grayscaleBuffer;
        break;

    default:
        throw std::invalid_argument("Invalid gray convertion mode selected");
        break;
    }
}

/**
 * @brief converting a rgb input buffer to a specific channel buffer
 * @details for separating each single color channel from the pixelBuffer
 * it consist to, for a selected channel in each pixel sample to preserve only the related channel and set other channels to 0.
 * example : p1(R = 160, G = 111, B = 180) -> p1_red(R = 160, G = 0, B = 0);
 * @param rgb_in the rgb input buffer
 * @param rgb_len the rgb buffer size
 * @param channel the wanted channel color you want to extract(either Red, Green or Blue).
 * @param ch_len a reference to the channel buffer ouptut size
 * @return uint8_t* one-channel output buffer
 *
 * @exception std::invalid_argument case none of the avaible color_channel mode for extraction is selected
 * @exception std::bad_alloc the case output buffer memory allocation failed
 * @see PixelsManager::color_channel().
 */
uint8_t *PixelsManager::rgb_to_channel(const uint8_t *rgb_in, int rgb_len, int channel, int &ch_len)
{
    uint8_t *channelBuffer = new uint8_t[(ch_len = rgb_len)]; // output channel buffer
    if (!channelBuffer)
        throw std::bad_alloc();

    memcpy(channelBuffer, rgb_in, rgb_len);

    switch (channel)
    {
    case color_channel::RED: // preserving red value and setting others to 0
        for (std::size_t i = 0; i < rgb_len; i += 3)
            channelBuffer[i + 1] = channelBuffer[i + 2] = 0x0;
        return channelBuffer;
        break;

    case color_channel::GREEN: // preserving green value and setting others to 0
        for (std::size_t i = 0; i < rgb_len; i += 3)
            channelBuffer[i] = channelBuffer[i + 2] = 0x0;
        return channelBuffer;
        break;

    case color_channel::BLUE: // preserving blue value and setting others to 0
        for (std::size_t i = 0; i < rgb_len; i += 3)
            channelBuffer[i] = channelBuffer[i + 1] = 0x0;
        return channelBuffer;
        break;

    default:
        throw std::invalid_argument("Invalid option for channel extraction");
        break;
    }
}

/**
 * @brief converting a rgb input buffer to a specific **single** channel buffer
 * @details for separating each single color channel from the pixelBuffer
 * it consist to, for a selected channel in each pixel sample to preserve and return only the related channel
 * @note suffix is means separated.
 * @param rgb_in the rgb input buffer
 * @param rgb_len the rgb buffer size
 * @param channel the wanted channel color you want to extract(either Red, Green or Blue).
 * @param ch_len a reference to the channel buffer ouptut size
 * @return uint8_t* one-separated-channel buffer
 *
 * @exception std::invalid_argument case none of the avaible color_channel mode for extraction is selected
 * @exception std::bad_alloc case the output buffer memory allocation failed
 */
uint8_t *PixelsManager::rgb_to_channel_s(const uint8_t *rgb_in, int rgb_len, int channel, int &ch_len)
{
    uint8_t *channelBuffer = new uint8_t[(ch_len = (rgb_len / 3))]; // channel output buffer
    if (!channelBuffer)
        throw std::bad_alloc();

    switch (channel)
    {
    case color_channel::RED: // copying only red values
        for (std::size_t i = 0, j = 0; i < rgb_len; i += 3, j++)
            channelBuffer[j] = rgb_in[i];
        return channelBuffer;
        break;

    case color_channel::GREEN: // copying only green values
        for (std::size_t i = 1, j = 0; i < rgb_len; i += 3, j++)
            channelBuffer[j] = rgb_in[i];
        return channelBuffer;
        break;

    case color_channel::BLUE: // copying only blue values
        for (std::size_t i = 2, j = 0; i < rgb_len; i += 3, j++)
            channelBuffer[j] = rgb_in[i];
        return channelBuffer;
        break;

    default:
        throw std::invalid_argument("Invalid option for channel selection");
        break;
    }
}

/**
 * @brief method for converting a grayscale input buffer to a binary output buffer by otsu nobuyuki method
 * @note Implementation of the Otsu Nobuyuki binarisation method -see on wikipedia-
 * according to experience result, binarisation method works better on a grayscale image.
 * then this is only working with a grayscale input.
 * @param gray_in the grayscale input buffer
 * @param gray_len the grayscale input buffer size
 * @param bin_len a reference to the otsu binarised buffer size
 *
 * @return uint8_t* output binarised buffer
 *
 * @exception std::bad_alloc case the output buffer memory allocation failed
 */
uint8_t *PixelsManager::grayscale_to_otsu(const uint8_t *gray_in, int gray_len, int &bin_len)
{
    double threshold(0), var_max(0), sum(0), sumB(0), q1(0), q2(0), u1(0), u2(0);
    double interClassVariance(0);

    int binariseLen = gray_len;                                     // output binarised buffer size
    uint8_t *binariseBuffer = new uint8_t[(bin_len = binariseLen)]; // output buffer
    if (!binariseBuffer)
        throw std::bad_alloc();

    // generating histogram
    std::vector<unsigned long> histogram;
    for (std::size_t i = 0; i <= 255; i++)
        histogram.push_back(std::count(gray_in, gray_in + gray_len, i));

    for (std::size_t i = 0; i <= 255; i++)
        sum += i * histogram[i];

    for (std::size_t i = 0; i <= 255; i++) // computing threshold
    {
        q1 += histogram[i];
        if (q1 == 0.0)
            continue;

        q2 = binariseLen - q1;
        sumB += i * histogram[i];
        u1 = sumB / q1;
        u2 = (sum - sumB) / q2;

        interClassVariance = (q1 * q2) * pow((u1 - u2), 2.0); // setting the interclass variance for the actual sample

        if (interClassVariance > var_max) // , we affect the threshold
        {
            threshold = i;
            var_max = interClassVariance;
        }
    }

    for (std::size_t i = 0; i < binariseLen; i++)
    {
        if (gray_in[i] > threshold) // 255 = total white
            binariseBuffer[i] = 255;
        else
            binariseBuffer[i] = 0; // 0 = total black
    }
    histogram.clear();
    return binariseBuffer; // return the binarised buffer
}

/**
 * @brief method for convertying a rgba buffer and get a rgb output buffer
 *
 * @param rgba_in the input rgba buffer
 * @param rgba_Len the input rgba buffer size
 * @param rgb_len a reference to the output buffer size
 * @return the rgb output buffer
 *
 * @exception std::bad_alloc case output buffer memory allocation failed
 */
uint8_t *PixelsManager::rgba_to_rgb(const uint8_t *rgba_in, int rgba_len, int &rgb_len)
{
    uint8_t *rgb_output = new uint8_t[(rgb_len = (3 * (rgba_len / 4)))];
    if (!rgb_output)
        throw std::bad_alloc();

    for (std::size_t i = 0, j = 0; i < rgba_len; i += 4, j += 3)
    {
        rgb_output[j] = rgba_in[i];
        rgb_output[j + 1] = rgba_in[i + 1];
        rgb_output[j + 2] = rgba_in[i + 2];
    }
    return rgb_output;
}

/**
 * @brief method for mixing many buffers channels into a single one, note that the channels must have the same size
 *
 * @param ch_nb the number of channels for mixing
 * @param ch_size the size of each channel buffer
 * @param ... the list of channels buffers to mix
 * @return either nullptr if there is an error or the ouput buffer.
 *
 * example : buffers - gray : {g1, g2, g3, g4} - alpha {a1, a2, a2, a4} - output : {g1, a1, g2, a2, g3, a3, g4, a4};
 * @exception std::bad_alloc case output buffer memory allocation failed
 * @exception std::invalid_argument Invalid channels number, a least of 02 is required
 */
uint8_t *PixelsManager::mixChannels(int ch_nb, int ch_size, ...)
{
    if (ch_nb <= 0 || ch_nb == 1) // limits cases, no treatment needed
        throw std::invalid_argument("Invalid channels input number, a least of 02 is required");
    else
    {
        va_list ap;
        va_start(ap, ch_size);

        uint8_t *outBuffer = new uint8_t[ch_size * ch_nb]; // output buffer
        if (!outBuffer)
            throw std::bad_alloc();

        uint8_t *actBuffer(nullptr);
        for (std::size_t i = 0; i < ch_nb; i++) // copying each channel buffer in its position in the out buffer
        {
            actBuffer = va_arg(ap, uint8_t *);
            for (std::size_t j = 0; j < ch_size; j++)
                outBuffer[i + j * ch_nb] = actBuffer[j];
        }

        va_end(ap);
        return outBuffer;
    }
}

/**
 * @brief Method for converting an input grayscale buffer to an rgb buffer using a look up table(lut)
 * 
 * 
 * @details This function implement a method for convertying a grayscale input buffer to a rgb buffer from a Look up table.
 * It consist for each luminance value in the grayscale buffer to :
 *   
 *   ~ Generate all rgb tuple possibilities that we could have by applying a rgb to grayscale convertying method (average(Gray = (R+G+B)/3) for this example)
 *   
 *   ~ For each rgb tuple generated, we compute distances to all colors in the lust
 *  
 *   ~ Then the corresponding rgb tuple in output buffer is the checked rgb tuple which is more close to a lut color.
 * 
 * @param gray_in grayscale buffer input
 * @param gray_len grayscale bufer input size
 * @param rgb_lut input rgb lut buffer
 * @param lut_len lut rgb buffer size
 * 
 * @return uint8_t* colorised buffer
 * @exception std::bad_alloc if the output buffer memory allocation failed.
 */
uint8_t *PixelsManager::lut_to_rgb(const uint8_t *gray_in, int gray_len, const uint8_t *rgb_lut, int lut_len)
{
    using rgb_color = std::tuple<uint8_t, uint8_t, uint8_t>;

    // container storing luminance value and vector of possible rgb tuples
    static std::map<uint8_t, std::vector<rgb_color>> luminance_possibilities;

    // for avoiding generation of rgb tuples of non-existing luminance in the input buffer(from 0 to 255), we'll skip all those with 0 occurrences
    const auto &histogram{PixelsManager::getHistogram(gray_in, gray_len)};

    // getting rgb tuple possibilities list
    for (std::size_t i = 0; i < 256; ++i)
    {
        if (histogram[i] == 0)
            continue;
        else if (luminance_possibilities.count(i) == 1)
            continue;
        else
            luminance_possibilities[i] = PixelsUtilities::get_rgb_possibilities([=](const uint8_t r, const uint8_t g, const uint8_t b) -> bool
                                                                                { return (((float)(r + g + b) / 3) == i) ? true : false; });
    }

    // output buffer
    uint8_t *rgb_out = new uint8_t[gray_len * 3];
    if (!rgb_out)
        throw std::bad_alloc();

    // for each luminance in the grayscale buffer, we'll compute distances of each possibility rgb tuple already to each color in the lut.
    std::map<uint8_t, std::vector<std::vector<double>>> abs_dists;
    for (std::size_t i = 0, inc = 0; i < gray_len; ++i, inc += 3) // each luminance value in the grayscale buffer
    {
        // for each rgb possibility, lambda for computing distance to a color in lut entry
        auto compute_dists = [&](const rgb_color &actual_color) -> void
        {
            std::vector<double> dist;

            for (std::size_t k = 0; k < lut_len; k += 3)
                dist.push_back(std::sqrt(std::pow(std::get<0>(actual_color) - rgb_lut[k], 2) + std::pow(std::get<1>(actual_color) - rgb_lut[k + 1], 2) + std::pow(std::get<2>(actual_color) - rgb_lut[k + 2], 2)));
            
            if ((abs_dists[gray_in[i]].max_size() - abs_dists[gray_in[i]].size()) == 1)
            {
                auto act_hist = PixelsManager::getHistogram(gray_in, gray_len);
                auto down_value = std::find(act_hist.begin(), act_hist.end(), 1);
                abs_dists[gray_in[(down_value - act_hist.begin())]].pop_back();
            }

            abs_dists[gray_in[i]].push_back(std::ref(dist));
            dist.clear();
        };

        // calling the lambda only if it wasn't already called
        if (abs_dists[gray_in[i]].empty())
            for_each(luminance_possibilities[gray_in[i]].begin(), luminance_possibilities[gray_in[i]].end(), compute_dists);

        // we get an iterator to the vector which comtent the lowest distance between the computed vectors distances
        auto min_ = std::min_element(abs_dists[gray_in[i]].begin(), abs_dists[gray_in[i]].end(), [](const std::vector<double> &dists_1, const std::vector<double> &dists_2) -> bool
                                     { 
                                                                            auto min_1 = std::min_element(dists_1.begin(), dists_1.end());
                                                                            auto min_2 = std::min_element(dists_2.begin(), dists_2.end());
                                                                            return ((*min_1) < (*min_2)); });

        // copying values to the input buffer
        std::tie(rgb_out[inc], rgb_out[inc + 1], rgb_out[inc + 2]) = luminance_possibilities[gray_in[i]][static_cast<int>(min_ - abs_dists[gray_in[i]].begin())];
        abs_dists.clear();
    }
    return rgb_out;
}

/**
 * @brief Method for convertying an input grayscale buffer to an output rgb buffer form a lut(look up table), multithreading ver.
 * 
 * @details Same as the PixelsManager::grayscale_to_rgb method, but this offer better speed execution by using multi-threading
 * For multi-threading, the method separates the input buffer into equals parts, then the convertion algorithm is called on each part.
 * @see PixelsManager::grayscale_to_rgb
 * 
 * 
 * @param gray_in input grayscale buffer
 * @param gray_len input grayscale buffer size
 * @param rgb_lut input rgb lut buffer
 * @param lut_len lut rgb buffer size
 * 
 * @param thread_number the number of threads to start.
 * @note default value for thread_number is the number of CPUs avaible on executing computer
 * @warning this method doesn't guarantee that the specified thread number will be effective. 
 * 
 * @return uint8_t* colorised buffer
 * @exception std::bad_alloc if the output buffer memory allocation failed.
 */
uint8_t *PixelsManager::lut_to_rgb_thread(const uint8_t *gray_in, int gray_len, const uint8_t *rgb_lut, int lut_len, int thread_number = std::thread::hardware_concurrency())
{
    // threads related declarations. _s suufix means plural
    std::vector<std::thread> task_s;
    std::vector<std::future<uint8_t *>> future_s;
    std::vector<std::promise<uint8_t *>> promise_s(thread_number);

    std::vector<uint8_t *> result_s; // vector for storing computed results

    // cause the programmer defines his own wanted threads number, and this method separates the input buffer in equals parts for computing,
    // we'll round(down) the threads number to correspond as dividing equally the input buffer
    const bool is_dividor{(gray_len % thread_number) == 0};
    int effective_threads{thread_number};
    if (!is_dividor)
    {
        for (std::size_t i = thread_number; i >= 1; --i)
            if ((gray_len % i) == 0)
            {
                effective_threads = i;
                break;
            }
    }

    uint8_t *rgb_out = new uint8_t[3 * gray_len]; // output buffer
    if (!rgb_out)
        throw std::bad_alloc();

    int length{gray_len / effective_threads};

    // thread creation lambda method
    auto thread_create_s = [&](const uint8_t *gray_in, int gray_len, const uint8_t *rgb_lut, int lut_len, std::promise<uint8_t *> &p)
    {
        p.set_value(PixelsManager::lut_to_rgb(gray_in, gray_len, rgb_lut, lut_len));
    };

    // creating and storing threads in out task list
    for (std::size_t i = 0; i < effective_threads; ++i) 
        task_s.emplace_back(std::thread(thread_create_s, gray_in + (i * length), length, rgb_lut, lut_len, std::ref(promise_s[i])));

    int i{0};
    for (i = 0; i < effective_threads; ++i) // futures results for each thread
        future_s.emplace_back(promise_s[i].get_future());

    for (i = 0; i < effective_threads; ++i) // waiting for all threads to finish
        task_s[i].join();

    for (i = 0; i < effective_threads; ++i) // storing results
        result_s.push_back(future_s[i].get());

    for (i = 0; i < effective_threads; ++i)
        std::memcpy(rgb_out + (i * length * 3), result_s[i], length * 3); // copying results to output buffer

    for (i = 0; i < effective_threads; ++i) // free allocated memory
        delete[] result_s[i];

    return rgb_out;
}

/**
 * @brief Method for getting the histogram for an input buffer
 *
 * @param gray_in the input grayscale buffer
 * @param gray_len the input buffer size
 * @return vector<int> representing the histogram output.
 * @note the method return the number of occurrences for values from 0 to 255.
 */
std::vector<int> PixelsManager::getHistogram(const uint8_t *buffer_in, int buffer_len) noexcept
{
    std::vector<int> histogram;
    for (std::size_t i = 0; i <= 255; i++)
        histogram.push_back(std::count(buffer_in, buffer_in + buffer_len, i));
    return histogram;
}

/**
 * @brief method for getting a fix number of colors that are most repetitives from an input rgb buffer
 *
 * @param rgb_in the input rgb buffer
 * @param rgb_len the rgb buffer size
 * @param nb_colors_out the wanted number of color in the output
 *
 * @return a buffer containing the number of desired colors or nullptr if error occurred
 * @exception std::bad_alloc case output buffer memory allocation failed
 * @exception std::runtime_error case wanted color number is greater than avaibles
 */
uint8_t *PixelsManager::get_high_occ_colors(const uint8_t *rgb_in, int rgb_len, int nb_colors_out)
{
    using rgb_color = std::tuple<uint8_t, uint8_t, uint8_t>;

    // convertying the input rgb buffer into a buffer of 3-tuple <R,G,B>
    rgb_color *rgb_arr = new rgb_color[(rgb_len / 3)];

    if (!rgb_arr)
        throw std::bad_alloc();

    for (std::size_t i = 0, j = 0; i < (rgb_len / 3); ++i, j += 3)
        rgb_arr[i] = std::make_tuple(rgb_in[j], rgb_in[j + 1], rgb_in[j + 2]);

    // calculate the number of occurrences of each rgb tuple
    std::map<rgb_color, int> occ;
    for (std::size_t i = 0; i < (rgb_len / 3); ++i)
        ++occ[rgb_arr[i]];

    const int color_number{(int)occ.size()};
    if (nb_colors_out > color_number)
        throw std::runtime_error("Wanted dominants colors is grater than avaible colors");

    // unfortunately, maps stores values by sorting by key(rgb tuple), but we want sorting by occurrences
    // then we'll put each rgb-tuple and occurrences, stores it in a vector of pairs, and sort vector by occurrences
    std::vector<std::pair<rgb_color, int>> vec;
    std::copy(occ.begin(), occ.end(), std::back_inserter<std::vector<std::pair<rgb_color, int>>>(vec));
    sort(vec.begin(), vec.end(), [](const std::pair<rgb_color, int> &a, const std::pair<rgb_color, int> &b) -> bool
                                 {
                                    return (a.second > b.second); // sorting by decreasing occurrences number
                                 });

    uint8_t *out_buffer = new uint8_t[nb_colors_out * 3]; // output buffer
    if (!out_buffer)
        throw std::bad_alloc();

    for (std::size_t i = 0, j = 0; i < nb_colors_out; i++, j += 3) // copying dominants colors in the output buffer
        std::tie(out_buffer[j], out_buffer[j + 1], out_buffer[j + 2]) = vec[i].first;

    delete[] rgb_arr;
    occ.clear();
    vec.clear();

    return out_buffer;
}

/**
 * @brief method for getting the nmber of color inside an input rgb buffer
 *
 * @param rgb_in input rgb buffer
 * @param rgb_len rgb buffer size
 *
 * @return int the exact number of differents colors inside the rgb buffer
 * @exception std::bad_alloc if no memory space avaible for computing
 */
int PixelsManager::get_nb_colors(const uint8_t *rgb_in, int rgb_len)
{
    using rgb_color = std::tuple<uint8_t, uint8_t, uint8_t>;

    // convertying the input rgb buffer into a buffer of 3-tuple <R,G,B>
    rgb_color *rgb_arr = new rgb_color[(rgb_len / 3)];

    if (!rgb_arr)
        throw std::bad_alloc();

    for (std::size_t i = 0, j = 0; i < (rgb_len / 3); ++i, j += 3)
        rgb_arr[i] = std::make_tuple(rgb_in[j], rgb_in[j + 1], rgb_in[j + 2]);

    // calculate the number of occurrences of each rgb tuple
    std::map<rgb_color, int> occ;
    for (std::size_t i = 0; i < (rgb_len / 3); ++i)
        ++occ[rgb_arr[i]];

    delete[] rgb_arr;

    return occ.size();
}

/**
 * @brief convert rgb input buffer to hsl_buffer
 *
 * @param rgb_in rgb input buffer
 * @param rgb_len rgb buffer size
 *
 * @return double* hsl converted buffer
 */
double *PixelsManager::rgb_to_hsl(const uint8_t *rgb_in, int rgb_len)
{
    double *hsl_out = new double[rgb_len];

    double r{0}, g{0}, b{0}, c_max{0}, c_min{0}, dc{0};
    double H{0}, S{0}, L{0};

    for (std::size_t j = 0; j < rgb_len; j += 3)
    {
        r = static_cast<double>(rgb_in[j]) / 255;
        g = static_cast<double>(rgb_in[j + 1]) / 255;
        b = static_cast<double>(rgb_in[j + 2]) / 255;

        c_max = getMax<double>(r, g, b);
        c_min = getMin<double>(r, g, b);
        dc = c_max - c_min;

        if (dc == 0)
            H = 0;
        else if (c_max == r)
            H = fmod(((g - b) / dc), 6);
        else if (c_max == g)
            H = ((b - r) / dc) + 2;
        else if (c_max == b)
            H = ((r - g) / dc) + 4;
        H *= 60;

        if (H < 0)
            H += 360;

        L = (c_max + c_min) / 2;

        if (c_max == 1)
            S = 0;
        else
            S = dc / (1 - std::fabs((2 * L) - 1));

        hsl_out[j] = H;
        hsl_out[j + 1] = S;
        hsl_out[j + 2] = L;
    }
    return hsl_out;
}

/**
 * @brief method for converting input hsl buffer into rgb buffer
 *
 * @param hsl_in hsl input buffer
 * @param hsl_len hsl buffer size
 * @return uint8_t* rgb converted buffer
 */
uint8_t *PixelsManager::hsl_to_rgb(const double *hsl_in, int hsl_len)
{
    double c{0}, m{0}, x{0};
    double h{0}, s{0}, l{0};
    double r{0}, g{0}, b{0};
    double hh{0};

    uint8_t *rgb_out = new uint8_t[hsl_len];
    for (std::size_t j = 0; j < hsl_len; j += 3)
    {
        h = hsl_in[j];
        s = hsl_in[j + 1];
        l = hsl_in[j + 2];

        if (h >= 360)
            h = 359;

        hh = h / 60;
        c = (1 - std::fabs((2 * l) - 1)) * s;
        m = l - (c / 2);
        x = c * (1 - std::abs(fmod(hh, 2.0) - 1));

        r = g = b = 0;

        if (h >= 0 && h < 60)
        {
            r = c;
            g = x;
        }
        else if (h >= 60 && h < 120)
        {
            r = x;
            g = c;
        }
        else if (h >= 120 && h < 180)
        {
            g = c;
            b = x;
        }
        else if (h >= 180 && h < 240)
        {
            g = x;
            b = c;
        }
        else if (h >= 240 && h < 300)
        {
            r = x;
            b = c;
        }
        else if (h >= 300 && h < 360)
        {
            r = c;
            b = x;
        }

        rgb_out[j] = (r + m) * 255.0;
        rgb_out[j + 1] = (g + m) * 255.0;
        rgb_out[j + 2] = (b + m) * 255.0;
    }
    return rgb_out;
}

/**
 * @brief method for getting most dominants colors in an image, by kmean clustering algorithm implementation.
 * 
 * @param rgb_in input rgb buffer
 * @param rgb_len input rgb buffer size
 * @param nb_colors wanted dominants color number
 * @param iters number of ietrations for kmean clustering
 * @param nb_colors_out effective colors out
 * @note nb_colors_out will contain the effective colors in the output buffer, cause after kmean computes specified iterations, it can be a cluster
 * which is empty (has no elements). 
 * @warning because input cluster positions are generated randomly, this method doesn't guarantee the same result at each execution.
 * 
 * @return uint8_t* output buffer of the effective dominants colors.
 * @exception std::runtime_error if wanted numbers of colors are greater than avaible colors.
 */
uint8_t *PixelsManager::get_dominants_colors_kmean(const uint8_t *rgb_in, int rgb_len, int nb_colors, int iters, int &nb_colors_out)
{   
    //for avoinding repetitions of colors when computing, we firstly extract colors in rgb input buffer(no repetitions) 
    int total_colors_nb = PixelsManager::get_nb_colors(rgb_in, rgb_len);
    uint8_t *rgb_buffer = PixelsManager::get_high_occ_colors(rgb_in, rgb_len, total_colors_nb); //grtting all colors without redundance

    // start by convertying input rgb buffer to points for kmean clustering
    std::vector<PixelsUtilities::Kmean_point> colors;
    for(int i = 0; i < total_colors_nb * 3; i += 3)
        colors.emplace_back(PixelsUtilities::Kmean_point(rgb_buffer[i], rgb_buffer[i + 1], rgb_buffer[i + 2]));

    // kmean clustering method, note that this method only modify the points buffer, precisely the cluster id of each point
    PixelsUtilities::kMeansClustering(colors, iters, nb_colors);

    // converting points buffer into a map, key = cluster ID, value = list of colors inside the actual cluster. 
    std::map<int, std::vector<std::tuple<uint8_t, uint8_t, uint8_t>>> clusterID_colors;
    for(int i = 0; i < nb_colors; ++i)
        for(std::size_t j = 0; j < colors.size(); ++j)
            if(colors[j].cluster == i)
                clusterID_colors[i].emplace_back(std::make_tuple(colors[j].r, colors[j].g, colors[j].b));

    int inc = 0;
    nb_colors_out = 0;
    uint8_t *rgb_out = new uint8_t[nb_colors * 3]; // output 

    if(nb_colors > total_colors_nb)
        throw(std::runtime_error("Wanted numbers of colors are greater than avaible colors"));

    // Note : PixelsManager::get_high_occ_colors returns a rgb buffer in which rgb's are stored from highest to fewest occurrences number.
    // for each cluster, we search in the paired color list the first which match the rgb buffer, in which in which rgb's are stored from highest to fewest occurrences number.
    // after finding a match, we skip and search again in the next cluster, so that we'll end up with colors with highest occurrences of each clusters.
    for(auto &id_color_pair : clusterID_colors)
    {
        for (std::size_t j = 0; j < total_colors_nb * 3; j += 3)
        {
            auto color_tofind = std::make_tuple(rgb_buffer[j], rgb_buffer[j + 1], rgb_buffer[j + 2]);
            auto found = std::find(id_color_pair.second.begin(), id_color_pair.second.end(), color_tofind);

            if (found == id_color_pair.second.end())
                continue;
            else
            {
                std::tie(rgb_out[inc], rgb_out[inc + 1], rgb_out[inc + 2]) = *found;
                ++nb_colors_out;
                break;
            }
        }
    }
    delete[] rgb_buffer; 

    return rgb_out;
}

/**
 * @brief Method for overscreening a specified color or a set or color in an image.
 * @details for an input rgb buffer, the method converts it to a grayscale buffer, then override the gray values by the specified input colors at the same position. 
 * 
 * @param rgb_in input rgb buffer
 * @param rgb_len input rgb buffer size
 * @param rgb_toscreen input rgb overscreen buffer
 * @param rgb_toscreen_len input rgb oversreen buffer size
 * @return uint8_t* oversreened buffer
 */
uint8_t *PixelsManager::overscreen_color(const uint8_t *rgb_in, int rgb_len, const uint8_t *rgb_toscreen, int rgb_toscreen_len)
{
    // converting rgb input buffer to grayscale buffer
    int gray_len{0};
    uint8_t *gray = PixelsManager::rgb_to_grayscale(rgb_in, rgb_len, PixelsManager::gray_level::AVERAGE, gray_len);

    // converting grayscale buffer into rgb form
    uint8_t *rgb_gray = PixelsManager::mixChannels(3, gray_len, gray, gray, gray);

    // overriding gray values
    for (std::size_t inc = 0; inc < rgb_toscreen_len; inc += 3)
    {
        std::vector<int> pos;
        for (std::size_t i = 0; i < rgb_len; i += 3)
            if (rgb_toscreen[inc] == rgb_in[i] && rgb_toscreen[inc + 1] == rgb_in[i + 1] && rgb_toscreen[inc + 2] == rgb_in[i + 2])
                pos.push_back(i);

        if (pos.empty())
            throw(std::runtime_error("A specified color is not found for exposure"));
        else
            for (auto pos_ : pos)
            {
                rgb_gray[pos_] = rgb_toscreen[inc];
                rgb_gray[pos_ + 1] = rgb_toscreen[inc + 1];
                rgb_gray[pos_ + 2] = rgb_toscreen[inc + 2];
            }
        pos.clear();
    }

    delete[] gray;
    return rgb_gray;
}


/**
 * @brief Method for bluring an input rgb buffer
 * @details Method based on default neighbours mean
 *  
 * @param rgb_in the input rgb buffer
 * @param rgb_len input rgb buffer size
 * @param width the width of the rgb buffer
 * @param height the height of the rgb buffer
 * @param side_neigbours number of neighbours to use.
 * @note side_neighbours indicates the radius in which neighbours wi'll be took. 
 * 
 * @exception std::bad_alloc if output memory allocation failed
 * @return uint8_t* the blurred buffer
 */
uint8_t *PixelsManager::blur(const uint8_t *rgb_in, int rgb_len, int s_width, int s_height, int side_neigbours)
{
    uint8_t *blur_out = new uint8_t[s_width * s_height * 3]; // output buffer
    if(!blur_out)
        throw std::bad_alloc();

    for(int i = 0; i < s_height; ++i)
    {
        for(int j = 0; j < s_width; ++j)
        {
            int s_r = 0, s_g = 0, s_b = 0, inc = 0;
            for(int k = -side_neigbours; k <= side_neigbours; ++k)
            {
                for(int l = -side_neigbours; l <= side_neigbours; ++l)
                {
                    if (i + k < 0 || i + k >= s_height)
                        continue;
                    if (j + l < 0 || j + l >= s_width)
                        continue;   
                    
                    s_r += rgb_in[((i + k) * s_width * 3) + ((j + l) * 3)];
                    s_g += rgb_in[((i + k) * s_width * 3) + ((j + l) * 3) + 1];
                    s_b += rgb_in[((i + k) * s_width * 3) + ((j + l) * 3) + 2];
                    ++inc;
                }
            }

            blur_out[(i * s_width * 3) + (j * 3)] =  s_r / inc;       
            blur_out[(i * s_width * 3) + (j * 3) + 1] = s_g / inc;       
            blur_out[(i * s_width * 3) + (j * 3) + 2] = s_b / inc;       
        }
    }

    return blur_out; 
}


/**
 * @brief Method for bluring an input rgb buffer
 * @details A non-linear, edge-preserving, and noise-reducing smoothing filter for images. 
 * It replaces the intensity of each pixel with a weighted average of intensity values from nearby pixels. 
 * This weight is based on a Gaussian distribution.
 *  
 * @param rgb_in the input rgb buffer
 * @param rgb_len input rgb buffer size
 * @param width the width of the rgb buffer
 * @param height the height of the rgb buffer
 * @param side_neigbours number of neighbours to use.
 * @param sigma sigma parameter of a gaussian repartition, more it'll be weak, more distance will influence weight.
 * @note side_neighbours indicates the radius in which neighbours wi'll be took. 
 * 
 * @exception std::bad_alloc if output memory allocation failed
 * @return uint8_t* the blurred buffer
 */
uint8_t *PixelsManager::gaussian_blur(const uint8_t *rgb_in, int rgb_len, int s_width, int s_height, int side_neigbours, float sigma)
{
    uint8_t *blur_out = new uint8_t[s_width * s_height * 3]; // output buffer
    if(!blur_out)
        throw std::bad_alloc();

    // computing all pixels weights by distance in a radius of neighbours numbers input
    std::map<int, float> pos_weight;
    for(int i = -side_neigbours; i <= side_neigbours; ++i)
        for(int j = -side_neigbours; j <= side_neigbours; ++j)
            pos_weight[i+j] = (1 / (sigma * sigma * 2 * 3.14)) * exp((-pow((i - ((float)side_neigbours / 2)), 2.0) - (std::pow((j - ((float)side_neigbours / 2)), 2.0))) / (2 * sigma * sigma));
    
    for(int i = 0; i < s_height; ++i)
    {
        for(int j = 0; j < s_width; ++j)
        {
            float s_r = 0, s_g = 0, s_b = 0, inc = 0;
            for(int k = -side_neigbours; k <= side_neigbours; ++k)
            {
                for(int l = -side_neigbours; l <= side_neigbours; ++l)
                {
                    if (i + k < 0 || i + k >= s_height)
                        continue;
                    if (j + l < 0 || j + l >= s_width)
                        continue;   

                    int pos = k + l;
                    s_r += rgb_in[((i + k) * s_width * 3) + ((j + l) * 3)] * pos_weight[pos];
                    s_g += rgb_in[((i + k) * s_width * 3) + ((j + l) * 3) + 1] * pos_weight[pos];
                    s_b += rgb_in[((i + k) * s_width * 3) + ((j + l) * 3) + 2] * pos_weight[pos];
                    inc = inc + pos_weight[pos];
                }
            }
            blur_out[(i * s_width * 3) + (j * 3)] =  s_r / inc;   
            blur_out[(i * s_width * 3) + (j * 3) + 1] = s_g / inc;       
            blur_out[(i * s_width * 3) + (j * 3) + 2] = s_b / inc;
        }
    }
    return blur_out; 
}