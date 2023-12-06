#include <stdio.h>
#include <stdint.h>

#define WIDTH_START 160
#define WIDTH_END 480

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

uint8_t** spilit(uint8_t ***pixels, int *width, int *height){
    uint8_t** resize_array= (uint8_t**)malloc(*height * sizeof(uint8_t *));
    for (int i = 0; i < *height; i++) {
        *(resize_array+i)=(uint8_t*)malloc((*width/2) * sizeof(uint8_t *));
        for (int j = 0; j < *width; j++)
            if(j>=WIDTH_START && j<WIDTH_END)
                resize_array[i][j-WIDTH_START]=(*pixels)[i][j];
    }
    return resize_array;
}

void saveToGryFile(const char *filename, uint8_t **array, int width, int height) {
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

int main() {
    const char *filename = "../test/test_640240.gry";
    uint8_t **pixels;
    int width, height;

    // Read the .gry file
    readGryFile(filename, &pixels, &width, &height);

    // Access and use the pixel values as needed
    uint8_t** resize_array = spilit(&pixels, &width, &height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width/2; j++)
            printf(" %d ", resize_array[i][j]);
        printf("\n");
    }
    saveToGryFile("../test/test_320240.gry", resize_array, width, height);
    // Free allocated memory
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);

    return 0;
}
