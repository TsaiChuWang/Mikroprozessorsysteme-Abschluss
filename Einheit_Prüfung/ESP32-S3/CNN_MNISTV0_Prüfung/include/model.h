#ifndef CNN_MODEL_H
#define CNN_MODEL_H

// #include "driver/sdmmc_host.h"
// #include "driver/sdmmc_defs.h"
// #include "sdmmc_cmd.h"
#include <stdlib.h>
#include "../include/network.h"
#include "../include/memtool.h"

#define filter_num 5
#define class_num 10

// 定义Network结构
typedef struct Network {
    CVLayer * input_layer;
    CVLayer * filter[filter_num];
    CVLayer * conv_layer;
    #define ConvActivate ReLU
    CVLayer * pool_layer;
    #define poolActivate sigmoid
    FCLayer * fc_input;
    FCLayer * fc_weight;
    FCLayer * fc_output;
    void (*destroy)(struct Network *);
} Network;

void killNetwork(Network * this) {
    delete_p(this->input_layer);
    delete_p(this->conv_layer);
    delete_p(this->pool_layer);
    delete_p(this->fc_input);
    delete_p(this->fc_weight);
    delete_p(this->fc_output);
    for (int i = 0; i < filter_num; ++ i)
        delete_p(this->filter[i]);
}

Network * loadNetwork(const char * fileName) {
    FILE * fp = fopen(fileName, "rb");
    Network * CNN = (Network *)malloc(sizeof(Network));
    CNN->destroy = killNetwork;
    CNN->input_layer = Convol2D_(1, 28, 28);
    for (int i = 0; i < filter_num; ++ i)
        CNN->filter[i] = loadFilter(fp);
    CNN->conv_layer = Convol2D_(5, 24, 24);
    CNN->pool_layer = Convol2D_(5, 12, 12);
    CNN->fc_input = FCLayer_(720);
    CNN->fc_weight = loadFCWeight(fp);
    CNN->fc_output = FCLayer_(10);
    fclose(fp);
    return CNN;
}

#endif //CNN_MODEL_H