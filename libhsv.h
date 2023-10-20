#pragma once

#include <stdio.h>

void _printColor(int r, int g, int b) {
    printf("\x1b[48;2;%i;%i;%im", r, g, b);
}

void resetTerminalColor() {
        printf("\x1b[0m");
}

void setTerminalColor(unsigned char h, unsigned char s, unsigned char v)
{
    unsigned char region, remainder, p, q, t;
    
    if (s == 0)
    {
        _printColor(v, v, v);
        return;
    }
    
    int r = 0;
    int g = 0;
    int b = 0;

    region = h / 43;
    remainder = (h - (region * 43)) * 6; 
    
    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;
    
    switch (region)
    {
        case 0:
            r = v; g = t; b = p;
            break;
        case 1:
            r = q; g = v; b = p;
            break;
        case 2:
            r = p; g = v; b = t;
            break;
        case 3:
            r = p; g = q; b = v;
            break;
        case 4:
            r = t; g = p; b = v;
            break;
        default:
            r = v; g = p; b = q;
            break;
    }
    
    _printColor(r, g, b);
}
