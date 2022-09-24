
#include "../../include/PixelsManager/PixelsUtilities.h"
#include "../../include/PixelsManager/PixelsManager.h"

/**
 * @brief method to get all R,G,B 3-tutple possibilities from an input value which verifies a specific condition
 *
 * @param value the input value frow which we'll generate possibilities
 * @param f condition a R,G,B 3-tuple should verify
 * @return a vector of all rgb tuple possibilities
 */
std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> PixelsUtilities::get_rgb_possibilities(std::function<bool(const uint8_t, const uint8_t, const uint8_t)> f) noexcept
{
    using rgb_color = std::tuple<uint8_t, uint8_t, uint8_t>;

    std::vector<rgb_color> output;
    int i{0}, j{0}, k{0};

    for (i = 0; i < 256; i++)
        for (j = 0; j < 256; j++)
            for (k = 0; k < 256; k++)
                if (f((uint8_t)i, (uint8_t)j, (uint8_t)k))
                    output.push_back(std::make_tuple((uint8_t)i, (uint8_t)j, (uint8_t)k));

    return output;
}

/**
 * @brief method for getting pixels values in a grayscale buffer, relatively to a specific input position
 * @details This method permit to move (while reading) in a grayscale buffer relatively to an input start position.
 * you can move in the buffer by rows, columns or both.
 *
 * @param gray_in grayscale buffer input
 * @param actual_position input position into the buffer
 * @param s_width width of the grayscale buffer(number of pixels by "line")
 * @param s_height height of the grayscale buffer(number of lines")
 * @param relative_row rows values for move
 * @param relative_col columns values for move
 * @param enable_skipping enable throwing exception if the wanted value is not in range of the gray entry
 *
 * @return value of the pixel in indicated place
 *
 * @exception std::out_of_range if the relatives input positions couldn't be attemp(out of range).
 */
uint8_t PixelsUtilities::get_luminance_at(uint8_t *gray_in, int actual_position, int s_width, int s_height, int relative_row, int relative_col)
{
    int act_row{(actual_position / s_width) + 1};
    int act_col{actual_position - ((act_row - 1) * s_width) + 1};
    
    if (act_row + relative_row > s_height || act_row + relative_row < 0)
        throw(std::out_of_range("bad relative_row input"));

    else if (act_col + relative_col > s_width || act_col + relative_col < 0)
        throw(std::out_of_range("bad relative_col input"));
    else
        return gray_in[(actual_position + (s_width * relative_row) + relative_col)];
}

/**
 * @brief method for getting rgb pixel in a rgb buffer, relatively to a specific input position
 * @details This method permit to move (while reading) in a rgb buffer relatively to an input start position.
 * you can move in the buffer by rows, columns or both.
 *
 * @param rgb_in grayscale buffer input
 * @param actual_position input position into the buffer
 * @param s_width width of the rgb buffer(number of pixels by "line")
 * @param s_height height of the rgb buffer(number of lines")
 * @param relative_row rows values for move
 * @param relative_col columns values for move
 *
 * @return value of the pixel in indicated place
 *
 * @exception std::runtime_error if the relatives positions couldn't be found(out of range).
 */
uint8_t *PixelsUtilities::get_rgb_at(uint8_t *rgb_in, int actual_pix_pos, int s_width, int s_height, int relative_row, int relative_col)
{
    int act_row{(actual_pix_pos / s_width) + 1};
    int act_col{actual_pix_pos - ((act_row - 1) * s_width) + 1};

    if (act_row + relative_row > s_height || act_row + relative_row < 0)
        throw(std::runtime_error("bad relative_row input"));

    else if (act_col + relative_col > s_width || act_col + relative_col < 0)
        throw(std::runtime_error("bad relative_col input"));

    uint8_t *rgb_pix_out = new uint8_t[3];
    rgb_pix_out[0] = rgb_in[3 * (actual_pix_pos + (relative_row * s_width) + relative_col)];
    rgb_pix_out[1] = rgb_in[3 * (actual_pix_pos + (relative_row * s_width) + relative_col) + 1];
    rgb_pix_out[2] = rgb_in[3 * (actual_pix_pos + (relative_row * s_width) + relative_col) + 2];

    return rgb_pix_out;
}

/**
 * @brief flipping bottom-left pixelsBuffer to top-left
 * @param pixelsBuffer the pixelsBuffer input
 * @param s_width the the pixels buffer image width
 * @param s_heigth the pixels buffer image height
 * @param colorChannel the number of channels in the pixel buffer
 *
 */
void PixelsUtilities::flipPixels(uint8_t *pixelsBuffer, int s_width, int s_heigth, int colorChannel)
{
    int totalLength = s_width * s_heigth * colorChannel;
    int oneLineLength = s_width * colorChannel;

    uint8_t *tmp = new uint8_t[totalLength];
    memcpy(tmp, pixelsBuffer, totalLength);
    for (int i = 0; i < s_heigth; i++)
        memcpy(pixelsBuffer + oneLineLength * i, tmp + totalLength - oneLineLength * (i + 1), oneLineLength);
    delete[] tmp;
}

/**
 * @brief the paeth predictor method
 *
 * @param left the left pixel value
 * @param up the up pixem value
 * @param upperLeft the upperleft pixel value
 * @return the paeth predicted value
 */
uint8_t PixelsUtilities::PaethPredictor(uint8_t left, uint8_t up, uint8_t upperLeft)
{
    int p = (left + up - upperLeft);
    int p_left = abs(p - left), p_up = abs(p - up), p_upperLeft = abs(p - upperLeft);

    if (p_left <= p_up && p_left <= p_upperLeft)
        return left;
    else if (p_up <= p_upperLeft)
        return up;
    else
        return upperLeft;
}

/**
 * @brief Get the possible all specific size possible sets for an input set
 * @note all values into the input buffer should be different each other.
 *
 * @param set_in the input set
 * @param subset_size each output set size. if 0, we'll return all possibles subsets
 * @param start_index position for recursive call
 * @param tmp_vec temptoral vector, storing recursives reuslts
 * @param out output reference to store the result.
 *
 * @return void.
 *
 * @exception std::bad_alloc() if output buffer limit reached.
 */
void PixelsUtilities::get_subsets(std::vector<double> &set_in, int subset_size, int start_index, std::vector<double> tmp_vec, std::vector<std::vector<double>> &out)
{
    if (subset_size == 0)
    {
        out.emplace_back(std::ref(tmp_vec));
        tmp_vec.clear();
        return;
    }
    for (std::size_t i = start_index; i < set_in.size(); i++)
    {
        tmp_vec.push_back(set_in[i]);
        get_subsets(set_in, subset_size - 1, i + 1, tmp_vec, out);
        tmp_vec.pop_back();
    }
}

/**
 * @brief method for calculating the variance of an input set
 *
 * @param set_in input set
 * @return double computed variance
 */
double PixelsUtilities::calculate_variance(std::vector<double> &set_in)
{
    double c{0}, s{0};

    for (int i = 0; i < set_in.size(); ++i)
        s += set_in[i] * set_in[i];

    return ((s / set_in.size()) - std::pow(std::accumulate(set_in.begin(), set_in.end(), 0.0) / set_in.size(), 2));
}

/**
 * @brief compute nCp recursively
 *
 * @param p
 * @param n
 * @return long long unsigned int computed result
 */
long long unsigned int PixelsUtilities::combination(long long unsigned int p, long long unsigned int n)
{
    return (p == 0) ? 1 : n * PixelsUtilities::combination(p - 1, n - 1) / p;
}

/**
 * @brief kmean method(Lloyd method implementation)
 * 
 * @param points reference input vector points
 * @param iters number of iterations to perform
 * @param nb_clusters number of clusters.
 * @return void. the input points are modified, for each point is assigned a cluster.
 */
void PixelsUtilities::kMeansClustering(std::vector<PixelsUtilities::Kmean_point> &points, int iters, int nb_clusters)
{
    int n = points.size();
    std::vector<PixelsUtilities::Kmean_point> centroids;

    srand(91);
    for (int i = 0; i < nb_clusters; ++i)
        centroids.push_back(points.at(rand() % n));

    for (int i = 0; i < iters; ++i)
    {
        for (auto centroid = centroids.begin(); centroid != centroids.end(); ++centroid)
        {
            int clusterId = centroid - centroids.begin();
            
            for (auto it = points.begin(); it != points.end(); ++it)
            {
                Kmean_point p = *it;
                double dist = centroid->distance(p);
                if (dist < p.minDist)
                {
                    p.minDist = dist;
                    p.cluster = clusterId;
                }
                *it = p;
            }
        }

        std::vector<int> nPoints;
        std::vector<double> sumR, sumG, sumB;
        for (int j = 0; j < nb_clusters; ++j)
        {
            nPoints.push_back(0);
            sumR.push_back(0.0);
            sumG.push_back(0.0);
            sumB.push_back(0.0);

        }

        for(auto &itr : points)
        {
            int clusterId = itr.cluster;
            nPoints[clusterId] += 1;
            sumR[clusterId] += itr.r;
            sumG[clusterId] += itr.g;
            sumB[clusterId] += itr.b;

            itr.minDist = __DBL_MAX__;
        }

        for (auto centroid = centroids.begin(); centroid != centroids.end(); ++centroid)
        {
            int clusterId = centroid - centroids.begin();
            centroid->r = sumR[clusterId] / nPoints[clusterId];
            centroid->g = sumG[clusterId] / nPoints[clusterId];
            centroid->b = sumB[clusterId] / nPoints[clusterId];
        }
    }
}