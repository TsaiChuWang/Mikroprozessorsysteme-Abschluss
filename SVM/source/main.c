#include "../include/configuration.h"
#include "../include/dataset.h"
#include "../include/histogram.h"
#include "../include/svm.h"
// #include "../include/siamese.h"
#include <stdlib.h>
#include <stdio.h>

// #define EACH
// #define TREE
#define SINGLE

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

#ifdef EACH
    SVM svms[7];
    for(int type = 0; type <7;type++){
        initializeSVM(&svms[type]);
        Sample trainingData[7][2000];
        for(int i = 0;i<7;i++){
            for(int j =0;j<2000;j++){
                int l;
                if(*(ausbildung_daten.labels+(i*2000+j))== type)
                    l=1;
                else l=-1;
                Sample s = {
                    .label = l,
                    .hogFeatures = ausbildung_besonderheiten.besonderheiten[(i*2000+j)]
                };
                trainingData[i][j] = s;
            }
        }
        trainSVM(&svms[type], trainingData);
        printf("finish train svm classfication [%d]\n", type);
    }

    int sum = 0;
    for(int i=0;i<prufung_daten.dataset_zahl;i++){
        __type* testHogFeatures = *(prufung_besonderheiten.besonderheiten+i);
        for(int type = 0; type <7;type++){
            float predictedClass = classifySample(testHogFeatures, &svms[type]);
            if(type == *(prufung_daten.labels+i) && (predictedClass>0.0)){
                printf("Predict class : %d with score : %f is fit : %d\n", type, predictedClass, predictedClass>0.0?1:0);
                sum++;
            }
               
        }
        printf("True label =%d\n\n\n", *(prufung_daten.labels+i));
    }
    printf("sum = %f %\n",(__type)sum/prufung_daten.dataset_zahl*100);
#endif

#ifdef TREE
    _type* predict = (_type*)malloc(sizeof(_type)*prufung_daten.dataset_zahl);
    memset(predict, NICHT_GEFUNDEN, prufung_daten.dataset_zahl);

    for(int type = 0; type <7;type++){
        SVM svm;
        initializeSVM(&svm);

        Sample trainingData[7][2000];
        for(int i = 0;i<7;i++){
            for(int j =0;j<2000;j++){
                int l;
                if(*(ausbildung_daten.labels+(i*2000+j))== type)
                    l=1;
                else l=-1;
                Sample s = {
                    .label = l,
                    .hogFeatures = ausbildung_besonderheiten.besonderheiten[(i*2000+j)]
                };
                trainingData[i][j] = s;
            }
        }
    }
#endif

#ifdef SINGLE

    SVM svm_1;
    initializeSVM(&svm_1);
    // Sample trainingData_1[7][2000];
    // for(int i = 0;i<7;i++){
    //     for(int j =0;j<2000;j++){
    //         int l;
    //         if(*(ausbildung_daten.labels+(i*2000+j))== 0)
    //             l=1;
    //         else if(*(ausbildung_daten.labels+(i*2000+j))== 1)
    //             l=-1;
    //         else l=0;
    //         Sample s = {
    //             .label = l,
    //             .hogFeatures = ausbildung_besonderheiten.besonderheiten[(i*2000+j)]
    //         };
    //         trainingData_1[i][j] = s;
    //     }
    // }

    Sample trainingData_1[14000];
    for(int i = 0;i<14000;i++){
        int l;
            if(*(ausbildung_daten.labels+(i))== 0)
                l=1;
            else if(*(ausbildung_daten.labels+(i))== 1)
                l=-1;
            else l=0;
            Sample s = {
                .label = l,
                .hogFeatures = ausbildung_besonderheiten.besonderheiten[i]
            };
            trainingData_1[i] = s;
    }
    trainSVM(&svm_1, trainingData_1);

    int sum = 0;
    for(int i=0;i<prufung_daten.dataset_zahl;i++){
        __type* testHogFeatures = *(prufung_besonderheiten.besonderheiten+i);
        int predictedClass = classifySample(testHogFeatures, &svm_1);
        
        if( predictedClass ==1 && *(prufung_daten.labels+i)== 0){
            sum++;
        }
            
        // if(predictedClass ==0 && *(prufung_daten.labels+i)> 0){
            // printf("Predicted Class = 0: %d real calss = %d, acc = %f\n", predictedClass, *(prufung_daten.labels+i), (__type)sum/i);
            // sum++;
        // }

        if(*(prufung_daten.labels+i)<2)
            printf("Predicted Class = 0: %d real calss = %d, acc = %f\n", predictedClass, *(prufung_daten.labels+i), (__type)sum/2000);
            
    }
    // printf("sum = %f\n",(__type)sum/prufung_daten.dataset_zahl);
    printf("sum = %f\n",(__type)sum/2000);

#endif


    
    // SVM svm_1;
    // initializeSVM(&svm_1);
    // Sample trainingData_1[7][14000];
    // for(int i = 0;i<7;i++){
    //     for(int j =0;j<14000;j++){
    //         int l;
    //         if(*(ausbildung_daten.labels+(i*2000+j))== 0)
    //             l=1;
    //         else l=-1;
    //         Sample s = {
    //             .label = l,
    //             .hogFeatures = ausbildung_besonderheiten.besonderheiten[(i*2000+j)]
    //         };
    //         trainingData_1[i][j] = s;
    //     }
    // }
    // trainSVM(&svm_1, trainingData_1);

    // int dum = 0;
    // for(int i=0;i<prufung_daten.dataset_zahl;i++){
    //     __type* testHogFeatures = *(prufung_besonderheiten.besonderheiten+i);
    //     int predictedClass = classifySample(testHogFeatures, &svm_1);
    //     printf("Predicted Class = 0: %d real calss = %d, acc = %f\n", predictedClass, *(prufung_daten.labels+i), (__type)dum/i);
    //     if( predictedClass ==1 && *(prufung_daten.labels+i)== 0)
    //         dum++;
    //     if(predictedClass ==0 && *(prufung_daten.labels+i)> 0)
    //         dum++;
    // }
    // printf("dum = %f\n",(__type)dum/prufung_daten.dataset_zahl);


    return SUCCESS;
}