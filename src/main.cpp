#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>
#include <vector>
#include <string>
#include <semaphore.h>
#include <exception>
#include <algorithm>
#include <sys/stat.h>
 
#include "../include/PNG/PNG.h"
#include "../include/PNG/Utilities.h"
#include "../include/PixelsManager/PixelsManager.h"

using namespace std; 
 
#include <fstream>
#include <ctime>
#include <vector>
#include <tuple>
#include <map>
#include <cmath>
#include <dirent.h>

std::mutex locker;   
DIR *dir;
dirent *ent;
struct stat st, st1; 
std::ofstream out_def("./out.txt", std::ios::app);

void gen(int id)
{
    while ((ent = readdir(dir)))
    {
        std::string name = ent->d_name;
        try
        {
            PNG p("./new_f/" + name);
            p.save("./new_o/_def" + name, PNG::COMPRESS::BEST);
            stat(("./new_f/" + name).c_str(), &st);
            stat(("./new_o/_def" + name).c_str(), &st1);

            locker.lock();
            out_def << (double)st.st_size/(1024*1024) << " " << (double)st1.st_size/(1024*1024) << "\n";
            locker.unlock();

        }
        catch(const std::exception &e)
        {
            std::cout << e.what() << std::endl;
            continue;
        }

        std::cout << name << " computed | \n";
    }
}


struct command
{
    private:
        int gr;
};


    /*
        list of CLI options :
            sub param are indented; a subparam must directly follow the param

        Input & Output : 
            -in : specify input file name -- many input files names must be separation with comma. exple io_image.exe -in path1,path2,path3.
            -out : specify output file name -- many input files names must be separation with comma. exple io_image.exe -out path1,path2,path3. the number of output files paths must be the same as the number of input files names
        
        Infos & Analysis
            -info : get several informations about input files -- file name; widthxheight; color mode; color depth; interlacing; raw pixels
            -hist : compute the color histogram of an input image
            -nb : compute the colors number of an input image
            -redun_clr : compute the most redundants colors in an image
                redun_nb : specify the number of redundant colors wanted
            -dom_clr : compute the most dominants colors inside an input image by kmean algorithms
                -dom_nb :  specify the number of dominant colors wanted
                -iters : specify the number of iterations of the kmean algorithms
        
        Actions :
            -ds : downlscale an input image : 
                scale : specify the scale
            -edetect : perform edge detection algorithm
                iters : specify the number of iterations
            -cpng : compress an input image
                clevel : specify the level of compression
               
            Blur
            -------------------------------------------------------------------------
            -mblur : perform mean blur algorithm on an input image 
                nrad : specify the neighbours radius
            -gblur : perform gaussian blur algorithm on an input image
                -nrad : specify the neighbours radius
                -sigma : specify the sigma parameter
            -pgbblur : perform a partial gaussian blur by binarised image
                -nrad : specify the neighbours radius
                -sigma : specify the sigma parameter
                -binpath : specify the binary image path
            -pgcblur : perform a partial gaussian blur by coordinates specification
                -nrad : specify the neighbours radius
                -sigma : specify the sigma parameter
                -coords : specify the coordinates of the square where perform blur -- the format must be xend-yend-yend-yend

            Convert
            -------------------------------------------------------------------------
            -rgba_rgb : perform a rgba to rgb color conversion
            -rgb_gs : perform a rgb to grayscale conversion
                -gl : specify the gray level
            -gs_bin  perform a grayscale to binary conversion(using the otsu nobuyuki algorithm)
    */

std::string get_infos(const std::string &path_in)
{
    std::string output;

    try
    {
    PNG p(path_in);
    output.append("Dimensions : " + std::to_string(p.get_width()) + "x" + std::to_string(p.get_height()) + "px\n");
    output.append("Color Mode : " + std::to_string(p.get_colorMode()) + " BitDepth : " + std::to_string(p.get_bitDepth()) + " \n");
    output.append("Interlace  : " + std::to_string(p.get_interlacing()) + " \n");
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << "\n";
    }
    return output;
}

void compressPNG(const std::string &path_in, const std::string &path_out, int comp_lvl)
{
    PNG p(path_in);
    switch (comp_lvl)
    {
    case 0:
        p.save(path_out, PNG::COMPRESS::NO);
        break;

    case 1:
        p.save(path_out, PNG::COMPRESS::SPEED);
        break;
    
    case 2:
        p.save(path_out, PNG::COMPRESS::DEFAULT);
        break;

    case 3:
        p.save(path_out, PNG::COMPRESS::BEST);
        break;

    default:
        p.save(path_out, PNG::COMPRESS::DEFAULT);
        break;
    };

    std::cout << "success";
}

int main(int argc, char *argv[])
{
    // creating commands vector, exluding the program name(considered as well as a command by the os)
    std:vector<std::string> commands;
    for (int i = 0; i < argc-1; ++i)
        commands.push_back(std::string(argv[i+1]));

    // lambda for splitting the input paths string in sub path, separated by 'sep', default = comma(,)
    auto split_path = [](std::vector<std::__cxx11::string>::iterator path_itr, char sep = ',') -> std::vector<std::string>
    {
        std::vector<std::string> splited_path;
        std::size_t start = 0;
        std::size_t end = (*path_itr).find(sep, start);
        while (end != (*path_itr).npos)
        {
            splited_path.emplace_back(std::string((*path_itr).begin() + start, (*path_itr).begin() + end));
            start = end + 1;
            end = (*path_itr).find(sep, start);
        }
        splited_path.emplace_back(std::string((*path_itr).begin() + start, (*path_itr).end()));

        return splited_path;
    };

    auto in_path_itr = std::find(commands.begin(), commands.end(), "-in");
    if(in_path_itr == commands.end() or ++in_path_itr == commands.end()) // after this, the input path iterator is located at the real input path argument position
        throw std::runtime_error("no input image path has beeen specified, arborting!");

    if ((*in_path_itr)[0] == '-')
        throw std::runtime_error("the argument '-in' must be followed by an input path");

    auto out_path_itr = std::find(commands.begin(), commands.end(), "-out"); 
    if(out_path_itr == commands.end() or ++out_path_itr == commands.end())
        throw std::runtime_error("no output image path has beeen specified, arborting!");

    if((*out_path_itr)[0] == '-')
        throw std::runtime_error("the argument '-out' must be followed by an output path");

    auto paths_in = split_path(in_path_itr);    // contain all inputs paths
    auto paths_out = split_path(out_path_itr);  // contain all outputs paths
    if (paths_out.size() != paths_in.size())
        throw std::runtime_error("the number of output paths must be the same as input paths");

    // now we start pixels operations...
    for (std::size_t file_i = 0; file_i < paths_in.size(); ++file_i)
    {
        for (int com_i = 0; com_i < commands.size(); ++com_i)
        {
            if (commands[com_i] == "-infos")
            {
                std::cout << file_i + 1 << "------------------------\n";
                std::cout << get_infos(paths_in[file_i]);
            }

            else if(commands[com_i] == "-cpng")
            {
                if (com_i == commands.size() - 1) // if it is the last parameter
                    throw std::runtime_error("the argument '-cpng' must be followed by the compress level(integer)");

                if (not std::all_of(commands[com_i + 1].begin(), commands[com_i + 1].end(), [](const char c){ return (bool)std::isdigit(c);})) // if the folowing arg is not an integer
                    throw std::runtime_error("the argument '-cpng' must be followed by the compress level(integer)");

                compressPNG(paths_in[file_i], paths_out[file_i], std::stoi(commands[com_i + 1]));
            }
        }
    }

    return 0;
}