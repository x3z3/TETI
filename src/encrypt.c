#include "../header/utils.h"

char FILENAME[] = "images/Shibe.bmp";

int main(int argc, char *argv[]) {
    Image img;
    img = import_data(FILENAME);

    ENCRYPT(FILENAME, img);
}