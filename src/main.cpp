

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
    try 
    {
        PNG origin("./origin.png");
        auto rgb = origin.get_raw_pixels();

        int gray_out {0};
        auto gray = PixelsManager::rgb_to_grayscale(rgb, origin.get_raw_pix_size(), PixelsManager::gray_level::AVERAGE, gray_out);
        PNG p1(gray, origin.get_width(), origin.get_height(), 8, 0);
        p1.save("gray.png");

        std::cout << " width is  :  " << origin.get_width() << " height : " << origin.get_height() << std::endl;
        // int bin_out {0};
        // auto binarised = PixelsManager::grayscale_to_otsu(gray, gray_out, bin_out);

        auto blured = PixelsManager::blur(rgb, origin.get_raw_pix_size(), origin.get_width(), origin.get_height(), 10);

        PNG p2(blured, origin.get_width(), origin.get_height(), 8, 2);
        p2.save("blurred.png");
    } catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}