#include "../header/utils.h"

char FILENAME[] = "images/Yarty.bmp";

int main(int argc, char *argv[]) {
    Image img;
    img = import_data(FILENAME);

    ENCRYPT(FILENAME, img);
}