#include "../header/utils.h"

char FILENAME[] = "NewImage.bmp";

int main(int argc, char *argv[]) {
    struct img_rel img;
    img = load_details(FILENAME);

    decrypt_func(FILENAME, img);
}