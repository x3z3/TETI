#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTE sizeof(char)

// Stores the minimal needed Header information

typedef struct Image Image;
struct Image {
    char hf[2];                // O: 00 Store Header Field [BM]
    unsigned int pix_offset;   // O: 10 Store image offset
    unsigned int w_pix;        // O: 18 Store width of img in pixels
    unsigned int h_pix;        // O: 22 Store height of img in pixels
    unsigned short bitsperpix; // O: 28 Store bits per pixel
    int padding;               //   Stores the number of bytes of padding
};

typedef struct Pixel Pixel;
struct Pixel {
    unsigned char B;
    unsigned char G;
    unsigned char R;
};

extern Image import_data(char FILENAME[]);
void export_data();

extern void ENCRYPT(char FILENAME[], Image img);
extern void DECRYPT(char FILENAME[], Image img);

int Pixel_to_ASCII(Pixel pix);
Pixel magic(Pixel pix, unsigned char ch);
int metadata_modifier(int color, int req);