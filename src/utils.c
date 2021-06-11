#include "../header/utils.h"

// Loads relevant details of the image
struct img_rel load_details(char FILENAME[]) {
  struct img_rel img;

  FILE *fp = fopen(FILENAME,"rb");

  fread(&img.hf, 2*BYTE, 1, fp);
  fseek(fp,8,SEEK_CUR);
  fread(&img.pix_offset, 4*BYTE, 1, fp);
  fseek(fp,4,SEEK_CUR);
  fread(&img.w_pix, 4*BYTE, 1, fp);
  fread(&img.h_pix, 4*BYTE, 1, fp);
  fseek(fp,2,SEEK_CUR);
  fread(&img.bitsperpix, 2*BYTE, 1, fp);

  fclose(fp); 

  // Testing Fields to check if BMP:
  if (img.hf[0] != 'B' && img.hf[1] != 'M') {
    printf("Incorrect Header field : %c%c\n",img.hf[0],img.hf[1]);
    printf("Terminating...\n");
    exit(0);
  }
  if (img.bitsperpix != 24) {
    printf("Bits per field incorrect : %d\n",img.bitsperpix);
    printf("Terminating...\n");
    exit(0);
  }

  // Calculate padding:
  int row_bytes = img.w_pix*3;
  img.padding = 0;
  while((row_bytes+img.padding)%4 != 0) {
    img.padding++;
  }

  return img;
}

void decrypt_func(char FILENAME[], struct img_rel img) {
  FILE *fp = fopen(FILENAME,"rb");
  FILE *wfp = fopen("output.txt","w");
  fseek(fp, img.pix_offset, SEEK_SET);
  
  struct pixel pix;
  unsigned char ch;
  int eof_flag = 0;

  for (int j=img.h_pix-1; j>=0; j--) {
    for (int i=0; i<img.w_pix; i++) {
      fread(&pix, sizeof(struct pixel), 1, fp);
      ch = ASCII_convert(pix);
      fprintf(wfp,"%c",ch);
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
  fprintf(wfp,"%c",5); // In case there is no terminating character
  fclose(wfp);
  fclose(fp);

  print_output();
  
}

void encrypt_func(char FILENAME[], struct img_rel img) {
  
  // append terminating char to the end of input:
  FILE *temp = fopen("input.txt","a");
  fprintf(temp,"%c",5);
  fclose(temp);
  remove("NewImage.bmp");

  FILE *rfp = fopen(FILENAME,"rb");
  FILE *wfp = fopen("NewImage.bmp","w");
  FILE *tfp = fopen("input.txt","r");
 
  // Copying the header files
  unsigned char data;
  for (int i=0; i<img.pix_offset; i++) {
    fread(&data, BYTE, 1, rfp);
    fwrite(&data, BYTE, 1, wfp);
  }

  // Copying the pixel
  struct pixel pix;
  char pad_ch = 0;
  int txt_flag = 1;
  unsigned char ch;
  
  for (int j=img.h_pix; j>0; j--) {
    for (int i=0; i<img.w_pix; i++) {
      fread(&pix, sizeof(struct pixel), 1, rfp);
      
      // Writing the pixel
      if (txt_flag == 1) {
        fread(&ch,BYTE,1,tfp);
        if (ch == 5) {
          txt_flag = 0;
        }
        pix = crypt(pix, ch);
      }

      fwrite(&pix, sizeof(struct pixel), 1, wfp);
    }
    fseek(rfp, img.padding, SEEK_CUR);
    fwrite(&pad_ch, BYTE, img.padding, wfp);
    
  }

  fclose(tfp);
  fclose(wfp);
  fclose(rfp);
  
  // Reset the file
  temp = fopen("input.txt","w");
  fclose(temp);

}

struct pixel crypt(struct pixel pix, unsigned char ch) {
  ch -= 2;
  int req_R,req_G,req_B;

  req_B = ch%5;
  ch = ch/5;
  req_G = ch%5;
  ch = ch/5;
  req_R = ch%5;

  pix.R = adjust_value(pix.R,req_R);
  pix.G = adjust_value(pix.G,req_G);
  pix.B = adjust_value(pix.B,req_B);

  return pix;
}
 
int adjust_value(int color, int req) {
  int diff = req - (color%5);
  if (diff >= 3) {
    diff -= 5;
  }
  else if (diff <= -3) {
    diff += 5;
  }
  int new_color = color + diff;
  if (new_color > 255) {
    new_color = new_color - 5;
    return new_color;
  }
  else if (new_color < 0) {
    new_color = new_color + 5;
    return new_color;
  }
}

int ASCII_convert(struct pixel pix) {
  int res = 0;
  res+= (pix.R%5)*25;
  res+= (pix.G%5)*5;
  res+= (pix.B%5);
  res += 2;
  return res;
}

void print_output() {
  FILE *fp = fopen("output.txt","r");
  printf("Output : \n");
  unsigned char ch;
  fread(&ch, BYTE, 1, fp);
  while(ch != 5) {
    printf("%c",ch);
    fread(&ch, BYTE, 1, fp);
  }
  printf("\n");
  fclose(fp);
  remove("output.txt");
}
