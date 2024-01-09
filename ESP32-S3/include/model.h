#define MODELL
#ifdef MODELL

#include <math.h>

float sigmoid(float x) {
    return 1.0 / (1.0 + exp(-x));
}

float dotProduct(float vec1[], float vec2[], int size) {
    float result = 0.0;
    for (int i = 0; i < size; ++i) {
        result += vec1[i] * vec2[i];
        //  printf("vec1[%d] = %f, vec2[%d]=%f, result = %f \n", i , vec1[i], i, vec2[i], result);
    }
    return result;
}

void einstufen_grosse(float* besonderheiten, float* model, float beurteilung, float bias, u_int8_t positiv_typ, u_int8_t negativ_typ, float* fraktion, u_int8_t* typ){
    *fraktion = sigmoid(dotProduct(model, besonderheiten, 1296) + bias);
    // printf("frk = %f\n", *fraktion);
    if(beurteilung>(*fraktion))
        *typ = positiv_typ;
    else *typ = negativ_typ;
}

void anzeigenWahlurne(u_int8_t* wahlurne){
    printf("wahlurne : \n");
    for(int index=0;index<7;index++)
        printf(" %d ", *(wahlurne+index));
    printf("\n");
}

u_int8_t findenMax(u_int8_t* wahlurne){
    int8_t max = -1;
    u_int8_t ret_index = 0;
    for(int index=0;index<7;index++){
        if(*(wahlurne+index)>max){
            ret_index = index;
            max = *(wahlurne+index);
        }
    }
    printf("max = %d\n", ret_index);
    return ret_index;
}

#endif