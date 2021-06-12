#include "../header/utils.h"

// Loads relevant details of the image
Image import_data(char FILENAME[]) {
    Image img;

    FILE *fp = fopen(FILENAME, "rb");

    fread(&img.hf, 2 * BYTE, 1, fp);
    fseek(fp, 8, SEEK_CUR);
    fread(&img.pix_offset, 4 * BYTE, 1, fp);
    fseek(fp, 4, SEEK_CUR);
    fread(&img.w_pix, 4 * BYTE, 1, fp);
    fread(&img.h_pix, 4 * BYTE, 1, fp);
    fseek(fp, 2, SEEK_CUR);
    fread(&img.bitsperpix, 2 * BYTE, 1, fp);

    fclose(fp);

    // Testing Fields to check if BMP:
    if (img.hf[0] != 'B' && img.hf[1] != 'M') {
        printf("Incorrect Header field : %c%c\n", img.hf[0], img.hf[1]);
        printf("Terminating...\n");
        exit(0);
    }
    if (img.bitsperpix != 24) {
        printf("Bits per field incorrect : %d\n", img.bitsperpix);
        printf("Terminating...\n");
        exit(0);
    }

    // Calculate padding:
    int row_bytes = img.w_pix * 3;
    img.padding = 0;
    while ((row_bytes + img.padding) % 4 != 0) {
        img.padding++;
    }

    return img;
}

void DECRYPT(char FILENAME[], Image img) {
    FILE *fp = fopen(FILENAME, "rb");
    FILE *wfp = fopen("output.txt", "w");
    fseek(fp, img.pix_offset, SEEK_SET);

    Pixel pix;
    unsigned char ch;
    int eof_flag = 0;

    for (int j = img.h_pix - 1; j >= 0; j--) {
        for (int i = 0; i < img.w_pix; i++) {
            fread(&pix, sizeof(Pixel), 1, fp);
            ch = Pixel_to_ASCII(pix);
            fprintf(wfp, "%c", ch);
            if (ch == 5) {
                eof_flag = 1;
                break;
            }
        }
        if (eof_flag) {
            break;
        }
        fseek(fp, img.padding, SEEK_CUR);
    }
    fprintf(wfp, "%c", 5); // In case there is no terminating character
    fclose(wfp);
    fclose(fp);

    export_data();
}

void ENCRYPT(char FILENAME[], Image img) {

    // append terminating char to the end of input:
    FILE *temp = fopen("input.txt", "a");
    fprintf(temp, "%c", 5);
    fclose(temp);
    remove("CrypticImage.bmp");

    FILE *rfp = fopen(FILENAME, "rb");
    FILE *wfp = fopen("CrypticImage.bmp", "w");
    FILE *tfp = fopen("input.txt", "r");

    // Copying the header files
    unsigned char data;
    for (int i = 0; i < img.pix_offset; i++) {
        fread(&data, BYTE, 1, rfp);
        fwrite(&data, BYTE, 1, wfp);
    }

    // Copying the Pixel
    Pixel pix;
    char pad_ch = 0;
    int txt_flag = 1;
    unsigned char ch;

    for (int j = img.h_pix; j > 0; j--) {
        for (int i = 0; i < img.w_pix; i++) {
            fread(&pix, sizeof(Pixel), 1, rfp);

            // Writing the Pixel
            if (txt_flag == 1) {
                fread(&ch, BYTE, 1, tfp);
                if (ch == 5) {
                    txt_flag = 0;
                }
                pix = magic(pix, ch);
            }

            fwrite(&pix, sizeof(Pixel), 1, wfp);
        }
        fseek(rfp, img.padding, SEEK_CUR);
        fwrite(&pad_ch, BYTE, img.padding, wfp);
    }

    fclose(tfp);
    fclose(wfp);
    fclose(rfp);

    // Reset the file
    temp = fopen("input.txt", "w");
    fclose(temp);
}

Pixel magic(Pixel pix, unsigned char ch) {
    ch -= 2;
    int req_R, req_G, req_B;

    req_B = ch % 5;
    ch = ch / 5;
    req_G = ch % 5;
    ch = ch / 5;
    req_R = ch % 5;

    pix.R = metadata_modifier(pix.R, req_R);
    pix.G = metadata_modifier(pix.G, req_G);
    pix.B = metadata_modifier(pix.B, req_B);

    return pix;
}

int metadata_modifier(int color, int req) {
    int diff = req - (color % 5);
    if (diff >= 3) {
        diff -= 5;
    } else if (diff <= -3) {
        diff += 5;
    }
    int new_color = color + diff;
    if (new_color > 255) {
        new_color = new_color - 5;
        return new_color;
    } else if (new_color < 0) {
        new_color = new_color + 5;
        return new_color;
    }
}

int Pixel_to_ASCII(Pixel pix) {
    int res = 0;
    res += (pix.R % 5) * 25;
    res += (pix.G % 5) * 5;
    res += (pix.B % 5);
    res += 2;
    return res;
}

void export_data() {
    FILE *fp = fopen("output.txt", "r");
    printf("The Secret Text\n");
    unsigned char ch;
    fread(&ch, BYTE, 1, fp);
    while (ch != 5) {
        printf("%c", ch);
        fread(&ch, BYTE, 1, fp);
    }
    printf("\n");
    fclose(fp);
    remove("output.txt");
}
