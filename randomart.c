#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include "libhsv.h"

int** create2DArray(int rows, int cols) {
    int** array = (int**)malloc(rows * sizeof(int*));
    for(int i = 0; i < rows; i++) {
        array[i] = (int*) calloc(cols, sizeof(int));
    }
    return array;
}

void free2DArray(int** array, int rows) {
    for(int i = 0; i < rows; i++) {
        free(array[i]);
    }
    free(array);
}

void roll(int* i, int min, int max) {
    *i = (*i + (max - min)) % max;
}

void generateRandomArt(char *data, int size, int **art, int width, int height) {
    int row = width / 2;
    int col = height / 2;

    for(int i = 0; i < size; i++) {
        for(int b = 0; b < 4; b++) {
            char bits = data[i] >> b * 2;
            switch(bits & 0x03) {
                case 0b00:
                    row++;
                    break;
                case 0b01:
                    row--;
                    break;
                case 0b10:
                    col++;
                    break;
                case 0b11:
                    col--;
                    break;
                default:
                    break;
            }
            roll(&row, 0, width - 1);
            roll(&col, 0, height - 1);
            art[row][col]++;
        }
    }
}

int findMax(int **art, int width, int height) {
    int max = art[0][0];
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            int temp;
            if((temp = art[i][j]) > max) max = temp;
        }
    }

    return max;
}

static char *table = " .`:,;\'_^\"></-!~=)(|j?}{ ][ti+l7v1%yrfcJ32uIC$zwo96sngaT5qpkYVOL40&mG8*xhedbZUSAQPFDXWK#RNEHBM@";

void printAsciiArt(int **art, int width, int height) {
    // len = 95
    int tableLength = strlen(table);

    int max = findMax(art, width, height);
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            int current = art[i][j];
            float f = (float) current / (float) max;
            printf("%c ", table[(int) (f * tableLength)]);
        }
        printf("\n");
    }
}

static int minHue = 150;
static int maxHue = 175;

void printColorArt(int **art, int width, int height) {
    int max = findMax(art, width, height);
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            setTerminalColor((int) ((maxHue - minHue) * ((float) art[i][j] / (float) max)) + minHue, 255, 255);
            printf("  ");
        }
        resetTerminalColor();
        printf("\n");
    }

}

static const char *helpStr =    "Syntax for randomart:\n"
                                "%s [options] data\n\n"
                                "Options:\n"
                                "\t   --help                 Prints this message\n"
                                "\t-w --width       <width>  Specifies the width            (default: 9)\n"
                                "\t-h --height      <height> Specifies the height           (default: 9)\n"
                                "\t   --ascii                Ascii output\n"
                                "\t   --ascii-table <table>  Specifies the table used by the ascii generation\n"
                                "\t   --min-hue              Minimum hue for colored output (default: 211)\n"
                                "\t   --max-hue              Maximum hue for colored output (default: 247)\n\n"
                                "(c) by Paul Stier, 2023\n";

int main(int argc, char **argv) {
    if(argc < 2) {
        printf(helpStr, argv[0]);
        return 1;
    }

    int asciiFlag = 0;
    const struct option longOpts[] = {
        {"help", no_argument, 0, 0},
        {"width", required_argument, 0, 'w'},
        {"height", required_argument, 0, 'h'},
        {"ascii", no_argument, &asciiFlag, 1},
        {"min-hue", required_argument, 0, 0},
        {"max-hue", required_argument, 0, 0},
        {"ascii-table", required_argument, 0, 0},
        {0, 0, 0, 0},
    };

    int optIndex;
    int opt;
    
    int rows = 9;
    int cols = 9;
    while((opt = getopt_long(argc, argv, "w:h:", longOpts, &optIndex)) != -1) {
        switch (opt)
        {
        case 0:
            if (longOpts[optIndex].flag != 0)
                break;
            
            struct option* opt = (struct option *)&(longOpts[optIndex]);
            if(strcmp(opt->name, "help") == 0) {
                printf(helpStr, argv[0]);
                return 0;
            } else if(strcmp(opt->name, "min-hue") == 0) {
                minHue = atof(optarg) / 360. * 255.;
            } else if(strcmp(opt->name, "max-hue") == 0) {
                maxHue = atof(optarg) / 360. * 255.;
            } else if(strcmp(opt->name, "ascii-table") == 0) {
                table = optarg;
            }

            break;
        case 'w':
            cols = atoi(optarg);
            break;
        case 'h':
            rows = atoi(optarg);
            break;
        default:
            break;
        }
    }

    int** art = create2DArray(rows, cols);
    generateRandomArt(argv[argc - 1], strlen(argv[argc - 1]), art, rows, cols);
    if(!asciiFlag) {
        printColorArt(art, rows, cols);
    } else {
        printAsciiArt(art, rows, cols);
    }
    free2DArray(art, rows);

    return 0;
}