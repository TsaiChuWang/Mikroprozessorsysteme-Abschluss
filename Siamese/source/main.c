#include "../include/configuration.h"
#include "../include/dataset.h"
#include "../include/histogram.h"
#include "../include/svm.h"
// #include "../include/siamese.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char * argv[]) {
    DATENSATZ ausbildung_daten= erhaltenDatensatz();
    mischen(ausbildung_daten);
#ifdef DATENSATZ_ANZEIGEN
    anzeigen_datensatz(ausbildung_daten);
#endif

    DATENSATZ prufung_daten= erhaltenDatensatz();
    mischen(prufung_daten);
#ifdef DATENSATZ_ANZEIGEN
    anzeigen_datensatz(prufung_daten);
#endif

    BESONDERHEIT ausbildung_besonderheiten = erhaltenBesonderheiten(ausbildung_daten);
    BESONDERHEIT prufung_besonderheiten = erhaltenBesonderheiten(prufung_daten);
        
    SVM svm_1;
    initializeSVM(&svm_1);
    Sample trainingData_1[7][14000];
    for(int i = 0;i<7;i++){
        for(int j =0;j<14000;j++){
            int l;
            if(*(ausbildung_daten.labels+(i*2000+j))== 0)
                l=1;
            else l=-1;
            Sample s = {
                .label = l,
                .hogFeatures = ausbildung_besonderheiten.besonderheiten[(i*2000+j)]
            };
            trainingData_1[i][j] = s;
        }
    }
    trainSVM(&svm_1, trainingData_1);

    // SVM svm_4567;
    // initializeSVM(&svm_4567);
    // Sample trainingData_4567[7][2000];
    // for(int i = 0;i<7;i++){
    //     for(int j =0;j<2000;j++){
    //         int l;
    //         if(*(ausbildung_daten.labels+(i*2000+j))>3)
    //             l=1;
    //         else l=-1;
    //         Sample s = {
    //             .label = l,
    //             .hogFeatures = ausbildung_besonderheiten.besonderheiten[(i*2000+j)]
    //         };
    //         trainingData[i][j] = s;
    //     }
    // }
    // trainSVM(&svm_4567, trainingData_4567);

    int dum = 0;
    for(int i=0;i<prufung_daten.dataset_zahl;i++){
        __type* testHogFeatures = *(prufung_besonderheiten.besonderheiten+i);
        int predictedClass = classifySample(testHogFeatures, &svm_1);
        printf("Predicted Class = 0: %d real calss = %d, acc = %f\n", predictedClass, *(prufung_daten.labels+i), (__type)dum/i);
        if( predictedClass ==1 && *(prufung_daten.labels+i)== 0)
            dum++;
        if(predictedClass ==0 && *(prufung_daten.labels+i)> 0)
            dum++;
    }
    printf("dum = %f\n",(__type)dum/prufung_daten.dataset_zahl);

    // NeuralNetwork network;
    // initializeNetwork(&network);
    // Sample samples[14000]; 
    // for(int i =0;i<14000;i++)
    //     samples[i] = {
    //         .input = *(ausbildung_besonderheiten.besonderheiten+i);
    //     };
    // trainSiameseNetwork(&network, samples);

    return SUCCESS;
}