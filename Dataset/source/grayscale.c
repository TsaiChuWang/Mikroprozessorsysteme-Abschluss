#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <png.h>

#include "../include/inc.h"

void read_png_file(const char *filename, uint8_t ***pixels, int *width, int *height) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening PNG file for reading");
        exit(EXIT_FAILURE);
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(file);
        fprintf(stderr, "Error creating PNG read struct\n");
        exit(EXIT_FAILURE);
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(file);
        fprintf(stderr, "Error creating PNG info struct\n");
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(file);
        fprintf(stderr, "Error during PNG file read\n");
        exit(EXIT_FAILURE);
    }

    png_init_io(png, file);
    png_read_info(png, info);

    *width = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (bit_depth != 8) {
        fprintf(stderr, "Unsupported bit depth in PNG file\n");
        exit(EXIT_FAILURE);
    }

    if (color_type != PNG_COLOR_TYPE_GRAY) {
        fprintf(stderr, "Error: Not a grayscale PNG\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for pixel values
    *pixels = malloc(*height * sizeof(uint8_t *));
    for (int i = 0; i < *height; i++) {
        (*pixels)[i] = malloc(*width * sizeof(uint8_t));
    }

    png_bytep row_pointers[*height];
    for (int i = 0; i < *height; i++) {
        row_pointers[i] = malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);

    // Copy pixel values to the allocated memory
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            (*pixels)[i][j] = row_pointers[i][j];
        }
        free(row_pointers[i]);
    }

    fclose(file);
    png_destroy_read_struct(&png, &info, NULL);
}

void write_png_file(const char *filename, uint8_t **pixels, int width, int height) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening PNG file for writing");
        exit(EXIT_FAILURE);
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(file);
        fprintf(stderr, "Error creating PNG write struct\n");
        exit(EXIT_FAILURE);
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, NULL);
        fclose(file);
        fprintf(stderr, "Error creating PNG info struct\n");
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(file);
        fprintf(stderr, "Error during PNG file write\n");
        exit(EXIT_FAILURE);
    }

    png_init_io(png, file);

    // Set the image information
    png_set_IHDR(
        png,
        info,
        width, height,
        8, // 8 bits per channel
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

int main(int argc, char *argv[]) {
    // const char* input_filename, output_filename;

#ifdef TEST
        const char* input_filename = PNG_FILE_PATH;
        const char* output_filename =  PNG_GRAYSCALE_FILE_PATH;
#else
        const char* input_filename = argv[1];
        const char* output_filename =  argv[2];       
#endif

    uint8_t **pixels;
    int width, height;

    // Read the grayscale PNG file and get pixel values
    read_png_file(input_filename, &pixels, &width, &height);

    // Write pixel values to a new PNG file
    write_png_file(output_filename, pixels, width, height);

    // Clean up memory
    for (int i = 0; i < height; i++) 
        free(pixels[i]);
    
    free(pixels);

    printf("Conversion completed successfully. form %s to %s \n", input_filename, output_filename);

    return EXIT_SUCCESS;
}
