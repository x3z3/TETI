# TETI - Text Encryption Through Images (A Steganographic Model)

![](brand/banner.png?raw=true)

## Usage

### Encryption

1. Head to src/encrypt.c and enter the name of the file that you wish to encrypt the text into.
2. Enter the text in input.txt.
3. Use the command `$ make en`

> The encrypted image can be found as CrypticImage.bmp

### Decryption

1. Head to src/decrypt.c and enter the name of the file that you wish to decrypt (CrypticImage.bmp is default)
2. Use the command `$ make de`

> The Decrypted text will be printed to console

## The Magic :sparkles:

- The text is encryted in the image's pixels. A character is represented by each pixel. Shifting a pixel by +2 or -2 results in a minor change because a pixel has three different components, R, G, and B.
- This is the formula for converting a pixel to a character:
- ASCII character : `((R % 5) * 25 + (G % 5) * 5 + (B)) + 2`
- Because the calculation above yields numbers ranging from 0 to 124, adding 2 yields values ranging from 2 to 126, which encompasses all readable ASCII letters.
- ASCII 5 provides the ending character, which stands for **Enquiry** and is rarely used.

## The Algorithm :computer:

The application turns a string of characters into their respective pixel offshift values.
The ASCII value of the character is lowered by two, and the offshift values are calculated as follows:
`B = result % 5`
`G = (result / 5) % 5`
`R = (result / 25) % 5`
The pixel is then offshifted correspondingly, resulting in the above R, G, and B values when the pixel values are multiplied by 5.
