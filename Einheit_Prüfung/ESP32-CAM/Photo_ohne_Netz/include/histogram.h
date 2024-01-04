// Histogram of oriented gradient
#define HISTOGRAM
#ifdef HISTOGRAM

#include "../include/configuration.h"

/**
 * Calculate histogram of oriented gradient
 * @param bilder  Binary image with BILDGRÖSSE
 */
float* berechnenHOG(__uint8_t* bilder) {
    float* hogMerkmale = (float*)malloc(sizeof(float)*BESONDERHEIT_ZAHL);
    uint8_t cellZahlX = BILDGRÖSSE / CELL_SIZE;
    uint8_t cellZahlY = BILDGRÖSSE / CELL_SIZE;
    
    int* bins = (int*)malloc(sizeof(int)*ZAHL_BINS);
    for(int i = 0; i < cellZahlY; i++) {
        for (int j = 0; j < cellZahlX; j++) {
            for (int k = 0; k < ZAHL_BINS; k++)
                *(bins+k) = 0;
            for (int m = 0; m < CELL_SIZE; m++) {
                for (int n = 0; n < CELL_SIZE; n++) {
                    int x = j * CELL_SIZE + n;
                    int y = i * CELL_SIZE + m;
                    
                    // Berechnen Sie die Größe und Ausrichtung des Gradienten
                    float gradientMagnitude = sqrt(pow(bilder[y*BILDGRÖSSE+x + 1] - bilder[y*BILDGRÖSSE+x - 1], 2) +
                                                     pow(bilder[(y + 1)*BILDGRÖSSE+x] - bilder[(y - 1)*BILDGRÖSSE+x], 2));
                    float gradientOrientation = atan2(bilder[(y + 1)*BILDGRÖSSE+x] - bilder[(y - 1)*BILDGRÖSSE+x],
                                                        bilder[y*BILDGRÖSSE+x + 1] - bilder[y*BILDGRÖSSE+x - 1]);
                    // Map orientation to bin index
                    int binIndex = (int)((gradientOrientation + M_PI) / (2.0 * M_PI / ZAHL_BINS));

                    // Accumulate gradient magnitude to the corresponding bin
                    bins[binIndex] += gradientMagnitude;
                }
            }

            // Concatenate the histogram values to the feature vector
            uint8_t featureIndex = (i * cellZahlX + j) * ZAHL_BINS;
            for (int k = 0; k < ZAHL_BINS; k++) {
                hogMerkmale[featureIndex + k] = bins[k];
#ifdef HISTOGRAM_ANZEIGEN
                printf("hogMerkmale[%d] = %f\n", featureIndex + k, hogMerkmale[featureIndex + k]);
#endif
            }
        }
    }
    free(bins); bins = NULL;
    return hogMerkmale;
}

/**
 * Display histogram of oriented gradient features
 * @param besonderheiten  Histogram of oriented gradient features
 */
void anzeigenBesonderheit(float* besonderheiten){
    for(int i=0;i<BESONDERHEIT_ZAHL;i++)
        printf("%03.1f ", *(besonderheiten+i));
    printf("\n");
}

// void normalizeHOG(__type* hogMerkmale) {
//     __type norm = 0.0;
//     for (int i = 0; i < BESONDERHEIT_ZAHL; ++i) {
//         norm += hogMerkmale[i] * hogMerkmale[i];
//     }
//     norm = sqrt(norm);

//     // Normalize each HOG feature by dividing by the norm
//     for (int i = 0; i < BESONDERHEIT_ZAHL; ++i) {
//         hogMerkmale[i] /= norm;
//     }
// }

// /**
//  * Obtain the histogram of oriented gradient features from the dataset
//  * @param daten  Dataset
//  */
// BESONDERHEIT erhaltenBesonderheiten(DATENSATZ daten){
//     BESONDERHEIT besonderheiten;
//     besonderheiten.dataset_zahl = daten.dataset_zahl;
//     besonderheiten.besonderheit_zahl = BESONDERHEIT_ZAHL;
//     besonderheiten.besonderheiten = (__type**)malloc(sizeof(__type*)*BESONDERHEIT_ZAHL);

//     for(int index=0;index<besonderheiten.dataset_zahl ;index++){
//         *(besonderheiten.besonderheiten+index) = berechnenHOG(*(daten.dataset+index));
// #ifdef HISTOGRAM_ANZEIGEN
//         anzeigenBesonderheit(index, besonderheiten);
// #endif
// #ifdef NORMALIZE_HOSTOGRAM
//         normalizeHOG(*(besonderheiten.besonderheiten+index));
// #endif
//     }
//     return besonderheiten;
// }



// #endif


#endif