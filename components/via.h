#pragma once
/* Timing info

Each horizontal raster is 228 VIA clocks
262 rasters in full frame

3 rasters V-Sync
37 rasters V-Blank
192 rasters of image
30 rasters of overscan

Each line consists of

68 clocks h-blank
160 clocks of image

@60fps = 3584160 Hz clock

*/

#include <inttypes.h>

class Via
{
public:
private:
    //uint8_t pixels[228*262];

    // palette
};
