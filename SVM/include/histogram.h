// Histogram of oriented gradient
#define HISTOGRAM
#ifdef HISTOGRAM

#define CELL_SIZE 8
#define NUM_BINS 9

#define BESONDERHEIT_ZAHL (IMAGE_SIZE /CELL_SIZE)*(IMAGE_SIZE/CELL_SIZE)*NUM_BINS
#include "../include/configuration.h"
#include "../include/dataset.h"

typedef struct besonderheit{
    int dataset_zahl, besonderheit_zahl;
    __type** besonderheiten;
}BESONDERHEIT;

#ifdef DATASET

/**
 * Calculate histogram of oriented gradient
 * @param bilder  Binary image with IMAGE_SIZE
 */
__type* berechnenHOG(_type* bilder) {
    __type* hogFeatures = (__type*)malloc(sizeof(__type)*BESONDERHEIT_ZAHL);
    int cellZahlX = IMAGE_SIZE / CELL_SIZE;
    int cellZahlY = IMAGE_SIZE / CELL_SIZE;
    

    int* bins = (int*)malloc(sizeof(int)*NUM_BINS);
    for(int i = 0; i < cellZahlY; i++) {
        for (int j = 0; j < cellZahlX; j++) {
            for (int k = 0; k < NUM_BINS; k++)
                *(bins+k) = 0;
            for (int m = 0; m < CELL_SIZE; m++) {
                for (int n = 0; n < CELL_SIZE; n++) {
                    int x = j * CELL_SIZE + n;
                    int y = i * CELL_SIZE + m;

                    // Calculate gradient magnitude and orientation
                    __type gradientMagnitude = sqrt(pow(bilder[y*IMAGE_SIZE+x + 1] - bilder[y*IMAGE_SIZE+x - 1], 2) +
                                                     pow(bilder[(y + 1)*IMAGE_SIZE+x] - bilder[(y - 1)*IMAGE_SIZE+x], 2));
                    __type gradientOrientation = atan2(bilder[(y + 1)*IMAGE_SIZE+x] - bilder[(y - 1)*IMAGE_SIZE+x],
                                                        bilder[y*IMAGE_SIZE+x + 1] - bilder[y*IMAGE_SIZE+x - 1]);

                    // Map orientation to bin index
                    int binIndex = (int)((gradientOrientation + M_PI) / (2.0 * M_PI / NUM_BINS));

                    // Accumulate gradient magnitude to the corresponding bin
                    bins[binIndex] += gradientMagnitude;
                }
            }

            // Concatenate the histogram values to the feature vector
            int featureIndex = (i * cellZahlX + j) * NUM_BINS;
            for (int k = 0; k < NUM_BINS; k++) {
                hogFeatures[featureIndex + k] = bins[k];
#ifdef HISTOGRAM_ANZEIGEN
                printf("hogFeatures[%d] = %f\n", featureIndex + k, hogFeatures[featureIndex + k]);
#endif
            }
        }
    }
    return hogFeatures;
}

/**
 * Display histogram of oriented gradient features
 * @param index  Serial number in the dataset
 * @param besonderheiten  Histogram of oriented gradient features
 */
void anzeigenBesonderheit(int index, BESONDERHEIT besonderheiten){
    for(int i=0;i<besonderheiten.besonderheit_zahl;i++)
        printf("%03.1f ", besonderheiten.besonderheiten[index][i]);
    printf("\n");
}

void normalizeHOG(__type* hogFeatures) {
    __type norm = 0.0;
    for (int i = 0; i < BESONDERHEIT_ZAHL; ++i) {
        norm += hogFeatures[i] * hogFeatures[i];
    }
    norm = sqrt(norm);

    // Normalize each HOG feature by dividing by the norm
    for (int i = 0; i < BESONDERHEIT_ZAHL; ++i) {
        hogFeatures[i] /= norm;
    }
}

/**
 * Obtain the histogram of oriented gradient features from the dataset
 * @param daten  Dataset
 */
BESONDERHEIT erhaltenBesonderheiten(DATENSATZ daten){
    BESONDERHEIT besonderheiten;
    besonderheiten.dataset_zahl = daten.dataset_zahl;
    besonderheiten.besonderheit_zahl = BESONDERHEIT_ZAHL;
    besonderheiten.besonderheiten = (__type**)malloc(sizeof(__type*)*BESONDERHEIT_ZAHL);

    for(int index=0;index<besonderheiten.dataset_zahl ;index++){
        *(besonderheiten.besonderheiten+index) = berechnenHOG(*(daten.dataset+index));
#ifdef HISTOGRAM_ANZEIGEN
        anzeigenBesonderheit(index, besonderheiten);
#endif
#ifdef NORMALIZE_HOSTOGRAM
        normalizeHOG(*(besonderheiten.besonderheiten+index));
#endif
    }
    return besonderheiten;
}



#endif


#endif