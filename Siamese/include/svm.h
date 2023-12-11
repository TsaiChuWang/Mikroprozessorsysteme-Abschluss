#define SVM_H
#ifdef SVM_H

#define WIDERHOLUNG_ZAHL 1000
#define LERNRATE 0.01

#ifdef BESONDERHEIT_ZAHL
#define HOG_FEATURE_SIZE BESONDERHEIT_ZAHL
#else
#include "../include/configuration.h"
#include "../include/dataset.h"
#include "../include/histogram.h"
#define HOG_FEATURE_SIZE (IMAGE_SIZE /CELL_SIZE)*(IMAGE_SIZE/CELL_SIZE)*NUM_BINS
#endif

#define NUM_CLASSES 7
#define HOG_FEATURE_SIZE 1296
#define NUM_SAMPLES_PER_CLASS 2000

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HOG_FEATURE_SIZE 1296
#define NUM_SAMPLES 14000
#define LEARNING_RATE 0.0000005
#define EPOCHS 1000

typedef struct {
    __type weights[HOG_FEATURE_SIZE];
    __type bias;
} SVM;

typedef struct {
    int label;
    __type* hogFeatures;
} Sample;

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

__type dotProduct(__type vec1[], __type vec2[], int size) {
    __type result = 0.0;
    for (int i = 0; i < size; ++i) {
        result += vec1[i] * vec2[i];
        //  printf("vec1[%d] = %f, vec2[%d]=%f, result = %f \n", i , vec1[i], i, vec2[i], result);
    }
    return result;
}

void initializeSVM(SVM *svm) {
    // Initialize SVM parameters
    for (int i = 0; i < HOG_FEATURE_SIZE; ++i) {
        svm->weights[i] = ((__type)rand() / RAND_MAX) * 2.0 - 1.0;
        // svm->weights[i] = 0.0;
    }
    svm->bias = 0.0;
}

void trainSVM(SVM *svm, Sample trainingData[]) {
    // Training the SVM model
    __type sum = 0;
    for (int epoch = 0; epoch < EPOCHS; ++epoch) {
        for (int i = 0; i < NUM_SAMPLES; ++i) {
            Sample *sample = &trainingData[i];
            // printf("dotProduct(svm->weights, sample->hogFeatures, HOG_FEATURE_SIZE) = %f\n", dotProduct(svm->weights, sample->hogFeatures, HOG_FEATURE_SIZE));
            __type predicted = dotProduct(svm->weights, sample->hogFeatures, HOG_FEATURE_SIZE) + svm->bias;
            __type error = (sample->label == 1) ? 1.0 : -1.0;
            // printf("")
            // printf("label = %d\n", sample->label);
            for (int j = 0; j < HOG_FEATURE_SIZE; ++j) {
                svm->weights[j] += LEARNING_RATE * error * sample->hogFeatures[j];
                // printf("sample->hogFeatures = %f\n", sample->hogFeatures[j]);
            }

            svm->bias += LEARNING_RATE * error;
            // printf("error = %f\n",error);
            // printf("svm->bias = %f\n",svm->bias);
            sum += svm->bias;
        }
        // printf("sum = %f, avg = %f\n",sum ,sum/NUM_SAMPLES);
    }
    
}

int classifySample(__type testHogFeatures[HOG_FEATURE_SIZE], SVM *svm) {
    // Classifying a test sample using the trained SVM model
    __type score = dotProduct(svm->weights, testHogFeatures, HOG_FEATURE_SIZE) + svm->bias;
    printf("score = %f\n", score);
    // printf("svm->bias = %f\n", svm->bias);
    //  printf("dotProduct(svm->weights, testHogFeatures, HOG_FEATURE_SIZE) = %f\n", dotProduct(svm->weights, testHogFeatures, HOG_FEATURE_SIZE));
    printf("decision = %f\n", sigmoid(score));
    return (score > 0) ? 1 : 0;
}

// int main() {
//     SVM svm;
//     initializeSVM(&svm);

//     Sample trainingData[NUM_SAMPLES];
//     // Initialize and load your training data (hogFeatures and labels)

//     // Training
//     trainSVM(&svm, trainingData);

//     // Testing (classify a test sample)
//     __type testHogFeatures[HOG_FEATURE_SIZE];
//     // Initialize your test HOG features

//     int predictedClass = classifySample(testHogFeatures, &svm);

//     printf("Predicted Class: %d\n", predictedClass);

//     return 0;
// }

#endif