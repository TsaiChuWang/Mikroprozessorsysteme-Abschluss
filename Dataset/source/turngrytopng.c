#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <png.h>

void writePngFile(const char *filename, uint8_t **pixels, int width, int height) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening PNG file for writing");
        return;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(file);
        fprintf(stderr, "Error creating PNG write struct\n");
        return;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, NULL);
        fclose(file);
        fprintf(stderr, "Error creating PNG info struct\n");
        return;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(file);
        fprintf(stderr, "Error during PNG file write\n");
        return;
    }

    png_init_io(png, file);

    // Set the image information
    png_set_IHDR(
        png,
        info,
        width, height,
        8,             // 8 bits per channel
        PNG_COLOR_TYPE_GRAY,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    // Write the image data
    png_bytep row_pointers[height];
    for (int i = 0; i < height; i++) {
        row_pointers[i] = pixels[i];
    }
    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    // Clean up
    png_destroy_write_struct(&png, &info);
    fclose(file);
}

void readGryFile(const char *filename, uint8_t ***pixels, int *width, int *height) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening .gry file for reading");
        return;
    }

    // Read width and height from the file
    fread(width, sizeof(int), 1, file);
    fread(height, sizeof(int), 1, file);

    // Allocate memory for pixel values
    *pixels = malloc(*height * sizeof(uint8_t *));
    for (int i = 0; i < *height; i++) {
        (*pixels)[i] = malloc(*width * sizeof(uint8_t));
    }

    // Read pixel values from the file
    for (int i = 0; i < *height; i++) {
        fread((*pixels)[i], sizeof(uint8_t), *width, file);
    }

    fclose(file);
}

int main() {
    // Read the .gry file or generate pixel data as needed
    const char *filename = "../test/test_640240.gry";
    uint8_t **pixels;
    int width, height;

    // Read the .gry file
    readGryFile(filename, &pixels, &width, &height);
    // Example 2D array representing pixel values
    // int width = 3;
    // int height = 2;

    // uint8_t **pixels = malloc(height * sizeof(uint8_t *));
    int count=0;
    for (int i = 0; i < height; i++) {
    //     pixels[i] = malloc(width * sizeof(uint8_t));
        for (int j = 0; j < width; j++) {
    //         // Populate the array with sample pixel values
            printf(" %d ", pixels[i][j]);
            count++;
        }
        printf("\n");
    }
    printf("cout=%d\n", count);

    // Save the 2D array to a .gry file
    writePngFile("../test/output_640240.png", pixels, width, height);

    // Clean up memory
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);

    return 0;
}
