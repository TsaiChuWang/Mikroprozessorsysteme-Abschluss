
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <png.h>
#include <stdint.h>


const char *inputFolder = "/home/wtsaichu/Dokumente/Arbeitsplatz/Dataset/Dataset/Dataset_minimized/";
const char *outputFolder = "/home/wtsaichu/Dokumente/Arbeitsplatz/Dataset/Dataset/Dataset_grayscale/";

#define LABEL_NUMBER 7
enum Label {
    C,
    FIRST,
    I,  
    OK,
    PALM,
    PALM_MOVED,
    THUMB
};

char* transferFilePath(enum Label label, char* png){
    char* filename = (char*)malloc(sizeof(char)*100);

    strcpy(filename, outputFolder);
    switch (label)
    {
        case C:
            strcat(filename, "c/");
            break;
        case FIRST:
            strcat(filename, "first/");
            break;
        case I:
            strcat(filename, "I/");
            break;
        case OK:
            strcat(filename, "ok/");
            break;
        case PALM:
            strcat(filename, "palm/");
            break;
        case PALM_MOVED:
            strcat(filename, "palm_moved/");
            break;
        case THUMB:
            strcat(filename, "thumb/");
            break;
        default:
            strcat(filename, "c/");
            break;
    }
    strcat(filename, png);
    *(filename+(strlen(filename)-1))='y';
    *(filename+(strlen(filename)-2))='r';
    *(filename+(strlen(filename)-3))='g';
    // printf("name=%s\n", filename);

    return filename;
}

void saveToGryFile(const char *filen, uint8_t **array, int width, int height) {
    char filename[100];
    strcpy(filename,outputFolder);
    strcat(filename,filen);
    *(filename+(strlen(filename)-1))='y';
    *(filename+(strlen(filename)-2))='r';
    *(filename+(strlen(filename)-3))='g';
    printf("name=%s\n", filename);
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening .gry file for writing");
        return;
    }

    // Write the width and height to the file
    fwrite(&width, sizeof(int), 1, file);
    fwrite(&height, sizeof(int), 1, file);

    // Write each pixel value to the file
    for (int i = 0; i < height; i++) {
        fwrite(array[i], sizeof(uint8_t), width, file);
    }

    fclose(file);
}

void read_png_file(const char *file) {
    char filename[100];
    strcpy(filename,inputFolder);
    strcat(filename,file);
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error opening PNG file");
        return;
    }

    // Initialize PNG structures
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        perror("Error creating PNG read struct");
        return;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        perror("Error creating PNG info struct");
        return;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        perror("Error during PNG file read");
        return;
    }

    // Initialize PNG file I/O
    png_init_io(png, fp);
    png_read_info(png, info);

    // Get image information
    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);

    // Ensure the image is in RGBA format
    png_set_filler(png, 0, PNG_FILLER_AFTER);
    png_set_gray_to_rgb(png);

    // Read the image
    png_bytep row_pointers[height];
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);
    uint8_t** gry_image=(uint8_t**)malloc(sizeof(uint8_t*)*height);

    // Print the pixel values (R, G, B)
    for (int y = 0; y < height; y++) {
        *(gry_image+y) = (uint8_t*)malloc(sizeof(uint8_t)*width);
        for (int x = 0; x < width; x++) {
            png_byte *pixel = &row_pointers[y][x * 4];
            uint8_t gray = (uint8_t)(0.299 * pixel[0] + 0.587 * pixel[1] + 0.114 * pixel[2]);
            gry_image[y][x] = gray;
            printf("Pixel at (%d, %d): R=%d, G=%d, B=%d, gray = %d\n", x, y, pixel[0], pixel[1], pixel[2], gray);
        }
    }
    saveToGryFile(file, gry_image, width, height);
    // printf("cpath =%s\n",transferFilePath(C, filename));
    // Clean up
    // free(gry_image);
    printf("name=%s\n", filename);
    fclose(fp);
    png_destroy_read_struct(&png, &info, NULL);
}

int main(int argc, char *argv[]) {
    read_png_file(argv[1]);
    return 0;
}
