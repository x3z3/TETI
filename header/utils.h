#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // Check if file exists

#define BYTE sizeof(char)

// Stores the minimal needed Header information
struct img_rel {
  char hf[2];                   // O: 00 Store Header Field [BM]
  unsigned int pix_offset;      // O: 10 Store image offset
  unsigned int w_pix;           // O: 18 Store width of img in pixels
  unsigned int h_pix;           // O: 22 Store height of img in pixels
  unsigned short bitsperpix;    // O: 28 Store bits per pixel
  int padding;                  //   Stores the number of bytes of padding
};

struct pixel {
  unsigned char B;
  unsigned char G;
  unsigned char R;
};

extern struct img_rel load_details(char FILENAME[]);
extern void decrypt_func(char FILENAME[], struct img_rel img);
extern void encrypt_func(char FILENAME[], struct img_rel img);
int ASCII_convert(struct pixel pix);
void print_output();
int adjust_value(int color, int req);
struct pixel crypt(struct pixel pix, unsigned char ch);