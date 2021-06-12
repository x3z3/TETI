#include "../header/utils.h"

char FILENAME[] = "CrypticImage.bmp";

int main(int argc, char *argv[]) {
    Image img;
    img = import_data(FILENAME);

    DECRYPT(FILENAME, img);
}