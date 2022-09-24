#ifndef _PNG_H_INCLUDED_
#define _PNG_H_INCLUDED_

#include <cmath>
#include <vector>
#include <cstdio>
#include <fstream>
#include <GL/gl.h>
#include <iostream>
#include <stdexcept>

#include "Chunks/IHDR_CHUNK.h"
#include "Chunks/PHYS_CHUNK.h"
#include "Chunks/IDAT_CHUNK.h"
#include "Chunks/IEND_CHUNK.h"

/**
 * 
 * @brief PNG class, contain PNG signature and CHUNKS. 
 * 
 */
class PNG
{
    public :
        PNG(const PNG &png);
        PNG(const std::string &path);
        PNG(const uint8_t *pixelBuffer, int s_width, int s_height, int bitDepth, int colorMode);
        PNG(const uint8_t *pixelBuffer, int s_width, int s_height, int bitDepth, int colorMode, unsigned int ppuX, unsigned int ppuY, uint8_t unitSpecifier);
        ~PNG();

        // accessors
        int get_width() const noexcept;
        int get_height() const noexcept;
        int get_raw_pix_size() const noexcept;
        uint8_t get_bitDepth() const noexcept;
        uint8_t get_colorMode() const noexcept;
        uint8_t get_interlacing() const noexcept;

        uint8_t *get_raw_pixels() const;

        void save(const std::string &path);
        static void glScreenshot(const std::string &png_dir, int x, int y, int width, int height, int bitDepth, int colorMode, int colorChannel, unsigned int ppuX, unsigned int ppuY, uint8_t unitSpecifier);

        PNG &operator=(const PNG &png_src);

    private : 
        uint8_t *m_signature = nullptr; /**< the default signature of all PNG files*/
        uint8_t *m_pixelBuffer = nullptr; /**< the raw pixels buffer that should contain the PNG file*/

        /** PNG CHUNKS objets : criticals(IHDR, IDAT, IEND) Optionals(pHYs)*/
        IHDR_CHUNK *m_IHDR = nullptr;
        PHYS_CHUNK *m_pHYs = nullptr;
        IDAT_CHUNK *m_IDAT = nullptr;
        IEND_CHUNK *m_IEND = nullptr;
        
        uint8_t *unfilter_line(const uint8_t *line_in, int lineLength, uint8_t filterMode, bool is_prev_line, const uint8_t *unfiltered_prev_line, uint8_t colorChannel);
        uint8_t *readPixels(const std::string &path, int &s_width, int &s_height, uint8_t &bitDepth, uint8_t &colorMode, uint8_t &colorChannel, int &pixelsBufferLen, int &ppuX, int &ppuY, uint8_t &unitSpecifier);
};



#endif //_PNG_H_INCLUDED_