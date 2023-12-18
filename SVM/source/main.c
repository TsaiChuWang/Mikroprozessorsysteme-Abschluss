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

#ifdef PAIR
    _type* predict = (_type*)malloc(sizeof(_type)*prufung_daten.dataset_zahl);
    memset(predict, NICHT_GEFUNDEN, prufung_daten.dataset_zahl);

    float** tickets = (float**)malloc(sizeof(float*)*prufung_daten.dataset_zahl);
    for(int i=0;i<prufung_daten.dataset_zahl;i++){
        *(tickets+i) = (float*)malloc(sizeof(float)*7);        
        memset(*(tickets+i), 0 ,7);
    }
    for(int type_1 = 0; type_1 <7;type_1++){
        for(int type_2 = 0; type_2 <7;type_2++){
            if(type_1 == type_2)
                continue;

            SVM svm;
            initializeSVM(&svm);
                
            Sample trainingData[4000];
            int index = 0;
            for(int i = 0;i<14000;i++){
                int l;
                if(*(ausbildung_daten.labels+i)== type_1)
                    l=1;
                else if(*(ausbildung_daten.labels+i)== type_2)
                    l=-1;
                else continue;

                if(*(ausbildung_daten.labels+i)== type_1 || *(ausbildung_daten.labels+i)== type_2){
                    Sample s = {
                        .label = l,
                        .hogFeatures = ausbildung_besonderheiten.besonderheiten[i]
                    };
                    trainingData[index] = s;
                    index++;
                    // printf("index = %d\n", index);
                }

            }
            
            trainSVM(&svm, trainingData);

            float zmax= -1,zmin = 2, zavg =0, zvar = 0, zcount = 0;
            float* predict = (float*)malloc(sizeof(float)*prufung_daten.dataset_zahl);
            for(int i=0;i<prufung_daten.dataset_zahl;i++){
                __type* testHogFeatures = *(prufung_besonderheiten.besonderheiten+i);
                *(predict+i) = classifySample(testHogFeatures, &svm);
                float predictedClass = *(predict+i);

                if(*(prufung_daten.labels+i)== type_1){
                    if(predictedClass > zmax)
                            zmax = predictedClass;
                        if(predictedClass < zmin)
                            zmin = predictedClass;
                        zavg += predictedClass;
                }
            }
            zavg = zavg/2000;
            for(int i=0;i<prufung_daten.dataset_zahl;i++){
                float predictedClass = *(predict+i);
                zvar += (predictedClass - zavg); 
            }
            zvar = zvar/2000;
            if(zvar<0)
                zvar = (-1*zvar);
            
            int sum = 0;
            for(int i=0;i<prufung_daten.dataset_zahl;i++){
                __type* testHogFeatures = *(prufung_besonderheiten.besonderheiten+i);
               float predictedClass = *(predict+i);
                if( predictedClass >=(zavg - zvar) && predictedClass <= (zavg + zvar) && *(prufung_daten.labels+i)== type_1){
                    sum++;
                    tickets[i][type_1] += 1;
                }

                if(predictedClass <(zavg - zvar) && predictedClass > (zavg + zvar) && *(prufung_daten.labels+i)== type_2){
                    sum++;
                    tickets[i][type_2] += 0.014;
                }

                if(*(prufung_daten.labels+i)== type_2 || *(prufung_daten.labels+i)== type_1)
                    printf("Predicted Class = 0: %f : %d real calss = %d, acc = %f, var = %f\n", predictedClass,(predictedClass >=(zavg - zvar) && predictedClass <= (zavg + zvar)), *(prufung_daten.labels+i), (__type)sum/i, (predictedClass - zavg));
            }
            printf("Is %d true or %d not = %f\n", type_1, type_2,(__type)sum/4000);
            printf("zmax = %f, zmin = %f, zavg = %f, zvar = %f \n", zmax, zmin, zavg, zvar);
        }
    }

    int o = 0;
    for(int i=0;i<prufung_daten.dataset_zahl;i++){
        printf("for true label : %d :", *(prufung_daten.labels+i));
        float max = -1;
        float mindex = -1;
        for(int j=0;j<7;j++){
            printf(" %.5f ", tickets[i][j]);
            if(tickets[i][j]> max){
                max = tickets[i][j];
                mindex = j;
            } 
        }
        printf("pred = %d\n", mindex);
        if(*(prufung_daten.labels+i)==mindex)
            o++;
    }
    printf("acc = %f\n", (float)o/14000);
#endif

#ifdef SINGLE

    SVM svm_1;
    initializeSVM(&svm_1);
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
    float zmax= -1,zmin = 2, zavg =0, zvar = 0, zcount = 0;

    float* predict = (float*)malloc(sizeof(float)*prufung_daten.dataset_zahl);
    for(int i=0;i<prufung_daten.dataset_zahl;i++){
        __type* testHogFeatures = *(prufung_besonderheiten.besonderheiten+i);
        *(predict+i) = classifySample(testHogFeatures, &svm_1);
        float predictedClass = *(predict+i);

        if(*(prufung_daten.labels+i)== 0){
           if(predictedClass > zmax)
                zmax = predictedClass;
            if(predictedClass < zmin)
                zmin = predictedClass;
            zavg += predictedClass;
        }
    }
    zavg = zavg/2000;
    for(int i=0;i<prufung_daten.dataset_zahl;i++){
        float predictedClass = *(predict+i);
        zvar += (predictedClass - zavg); 
    }
    zvar = zvar/2000;
    printf("zvar = %f\n", zvar);
    if(zvar<0)
        zvar = (-1*zvar);
    for(int i=0;i<prufung_daten.dataset_zahl;i++){
        float predictedClass = *(predict+i);
        if( predictedClass >=(zavg - zvar) && predictedClass <= (zavg + zvar) && *(prufung_daten.labels+i)== 0){
            sum++;
            zcount += 1;
        }
        else if((predictedClass< (zavg - zvar) || predictedClass > (zavg + zvar)) && *(prufung_daten.labels+i)> 0)
            sum++;
        if(*(prufung_daten.labels+i)== 0){
            printf("Predicted Class = 0: %f : %d real calss = %d, acc = %f, var = %f\n", predictedClass,(predictedClass >=(zavg - zvar) && predictedClass <= (zavg + zvar)), *(prufung_daten.labels+i), (__type)sum/i, (predictedClass - zavg));
        //     if(predictedClass > zmax)
        //         zmax = predictedClass;
        //     if(predictedClass < zmin)
        //         zmin = predictedClass;
        //     zavg += predictedClass;
        //     zvar += (predictedClass - 0.4542843935);
        }
            
        // if(predictedClass ==0 && *(prufung_daten.labels+i)== 1){
        //     sum++;
        // }
            
        // if(predictedClass ==0 && *(prufung_daten.labels+i)> 0){
            
            // sum++;
        // }

        // if(*(prufung_daten.labels+i)<2)
        //     printf("Predicted Class = 0: %d real calss = %d, acc = %f\n", predictedClass, *(prufung_daten.labels+i), (__type)sum/2000);
            
    }
    // printf("sum = %f\n",(__type)sum/prufung_daten.dataset_zahl);
    printf("sum = %f\n",(__type)sum/14000);
    printf("zcount = %f\n", zcount/2000);
    printf("zmax = %f, zmin = %f, zavg = %f, zvar = %f \n", zmax, zmin, zavg, zvar);
    printf("Bias : %f\n", svm_1.bias);

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