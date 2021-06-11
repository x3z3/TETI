#include "../header/utils.h"

char FILENAME[] = "images/Yarty.bmp";

int main(int argc, char* argv[]) {
  struct img_rel img;
  img = load_details(FILENAME);

  encrypt_func(FILENAME, img);
}