#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

#include "../include/inc.h"

void read_png_file(const char *filename, png_bytep **row_pointers, int *width, int *height) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) abort();

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) abort();

    png_infop info = png_create_info_struct(png);
    if (!info) abort();

    if (setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    png_read_info(png, info);

    *width = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);
    int color_type = png_get_color_type(png, info);
    int bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16)
        png_set_strip_16(png);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * (*height));
    for (int y = 0; y < *height; y++)
        (*row_pointers)[y] = (png_byte *)malloc(png_get_rowbytes(png, info));

    png_read_image(png, *row_pointers);

    fclose(fp);
}

void write_png_file(const char *filename, png_bytep *row_pointers, int width, int height) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) abort();

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) abort();

    png_infop info = png_create_info_struct(png);
    if (!info) abort();

    if (setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    png_set_IHDR(
        png,
        info,
        width, height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    fclose(fp);
}

void crop_image(png_bytep *src_rows, int src_width, int src_height,
                png_bytep *dest_rows, int dest_x, int dest_y, int dest_width, int dest_height) {
    for (int y = 0; y < dest_height; y++) {
        png_bytep src_row = src_rows[y + dest_y] + dest_x * 4;
        png_bytep dest_row = dest_rows[y];

        memcpy(dest_row, src_row, dest_width * 4);
    }
}

int main(int argc, char *argv[]) {
    char* input_filename;
    char* output_filename;
#ifdef TEST
        input_filename = PNG_GRAYSCALE_FILE_PATH;
        output_filename =  PNG_CROP_FILE_PATH;
#else
        input_filename = argv[1];
        output_filename =  argv[2];       
#endif
    int width, height;
    png_bytep *row_pointers;

    read_png_file(input_filename, &row_pointers, &width, &height);

    // Define the region of interest (ROI) by specifying a rectangle
    int dest_x = 160;  // starting x-coordinate of the rectangle
    int dest_y = 0;   // starting y-coordinate of the rectangle
    int dest_width = 240;  // width of the rectangle
    int dest_height = 240; // height of the rectangle

    // Create a new image buffer for the cropped image
    png_bytep *cropped_rows = (png_bytep *)malloc(sizeof(png_bytep) * dest_height);
    for (int y = 0; y < dest_height; y++)
        cropped_rows[y] = (png_byte *)malloc(dest_width * 4);

    // Crop the image using the defined ROI
    crop_image(row_pointers, width, height, cropped_rows, dest_x, dest_y, dest_width, dest_height);

    // Write the cropped image to a new PNG file
    write_png_file(output_filename, cropped_rows, dest_width, dest_height);

    // Free allocated memory
    for (int y = 0; y < height; y++)
        free(row_pointers[y]);
    free(row_pointers);

    for (int y = 0; y < dest_height; y++)
        free(cropped_rows[y]);
    free(cropped_rows);

    return 0;
}
