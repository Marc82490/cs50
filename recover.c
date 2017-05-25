#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // verify command line argument
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }
    
    // remember argument
    char *infile = argv[1];
    
    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // declare buffer array
    unsigned char buffer[512];
    
    // set counter for image name
    int counter = 0;
    
    // name output file
    char filename[8];
    sprintf(filename, "%03i.jpg", counter);
                
    // open output file
    FILE *outptr = fopen(filename, "w");
    if (outptr == NULL)
    {
        fprintf(stderr, "Could not open output file.\n");
        return 3;
    }
    
    // read in raw data
    while (fread(buffer, 1, 512, inptr) == 512)
    {
        // check first 4 values of the buffer to see if they're the start of a .jpg
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
            // if first jpeg
            if (counter == 0)
            {
                // write to output
                fwrite(buffer, 1, 512, outptr);
                
                // increment counter to avoid this path again
                counter++;
            }
            else
            {
                // close the current output file
                fclose(outptr);
                
                // name new output file
                sprintf(filename, "%03i.jpg", counter);
                
                // open output file
                outptr = fopen(filename, "w");
                
                // write to output
                fwrite(buffer, 1, 512, outptr);
                
                // increment counter to change name of next new jpeg
                counter++;
            }
        // if not new jpeg
        else
        {
            // if not first jpeg, must have an output file open
            if (counter != 0)
            {
                fwrite(buffer, 1, 512, outptr);
            }
            // if not jpeg, current block is garbage data
            else
            {
                continue;
            }
        }
    }
    
    // close output file
    fclose(outptr);
    
    // close input file
    fclose(inptr);
    
    return 0;
}
