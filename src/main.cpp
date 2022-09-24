#include <iostream>
#include <fstream>
#include <ctime>
#include <tuple>
#include <numeric>

#include "../include/PNG/PNG.h"
#include "../include/PNG/Utilities.h"
#include "../include/PixelsManager/PixelsManager.h"
#include "../include/PixelsManager/PixelsUtilities.h"

using namespace std;

#include <iostream>
#include <string>
#include <list>

using namespace std;


int main(int argc, char *argv[])
{
    PNG origin("./origin.png");
    auto rgb = origin.get_raw_pixels();

    int gray_out {0};
    auto gray = PixelsManager::rgb_to_grayscale(rgb, origin.get_raw_pix_size(), PixelsManager::gray_level::AVERAGE, gray_out);
    PNG p1(gray, origin.get_width(), origin.get_height(), 8, 0);
    p1.save("gray.png");

    int bin_out {0};
    auto binarised = PixelsManager::grayscale_to_otsu(gray, gray_out, bin_out);

    PNG p2(binarised, origin.get_width(), origin.get_height(), 8, 0);
    p2.save("binarised.png");
    return 0;
}