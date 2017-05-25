/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember arguments
    int factor = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    // remember original height and width
    LONG og_width = bi.biWidth;
    LONG og_height = bi.biHeight;
    
    // determine original padding for scanlines
    int old_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // update width and height
    bi.biWidth *= factor;
    bi.biHeight *= factor;
    
    // determine new padding for scanlines
    int new_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // update biSizeImage
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + new_padding) * abs(bi.biHeight);
    
    // update bfSize
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // calculate offset
    long offset = og_width * sizeof(RGBTRIPLE);
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(og_height); i < biHeight; i++)
    {
        // repeat each row by factor number of times
        for (int n = 0; n < factor; n++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < og_width; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                // repeat each pixel by factor number of times
                for (int m = 0; m < factor; m++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            // add padding to new image
            for (int k = 0; k < new_padding; k++)
            {
                fputc(0x00, outptr);
            }
        // send file pointer back to beginning of the current line so long as there is one more loop through the current line
        if (n < factor-1)
        {
            fseek(inptr, -offset, SEEK_CUR);
        }
        // if last time through the current line
        else
        {
            // skip over padding of old image, if any
            fseek(inptr, old_padding, SEEK_CUR);
        }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
