# TETI - Text Encryption Through Images (A Steganographic Model).

![](brand/banner.gif)

## Running TETI

### Encrypt

1. Head to src/main_encrypt.c and enter the name of the file that you wish to encrypt the text into.
2. Enter the text in input.txt.
3. Use the command $ make encrypt
4. Run encrypt with $ ./encrypt
   The encrypted image can be found as NewImage.bmp

### Decrypt

1. Head to src/main_decrypt.c and enter the name of the file that you wish to decrypt (NewFile.txt is default)
2. Use the command $ make decrypt
3. Run decrypt with $ ./decrypt
   The Decrypted text will be printed to console

## How is it encrypted

- Text is encryted through the pixels of the image. Each pixel represents a character. Since there are 3 different components, R, G and B in a pixel, shifting a pixel by +2 or -2 ends up with a minimal change.
- The formula to convert a pixel to a character is given by this:
- ASCII character : ((R%5)*25 + (G%5)*5 + (B)) + 2
- Since the formula above gives us numbers from 0 to 124, adding 2 gives us numbers from 2-126 which covers all the printable ASCII characters.
- The terminating character is given by ASCII 5 which stands for Enquiry, which is not used for common purposes.

## **Encryption**

The program takes a string of characters, and converts them into their pixel offshift values
The character's ASCII is reduced by 2, and is converted into the offshift values by:
`B = result % 5`
`G = (result/5) % 5`
`R = (result/25) % 5`
Then the pixel is offshifted accordingly to make it so that the pixel values when modded by 5 gives the above R, G and B values.
