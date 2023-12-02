//
// Created by MAC on 2019/12/31.
//

#include <stdio.h>
#include <string.h>
#include "../include/vector.h"
#include "../include/dataio.h"
#include "../include/network.h"
#include "../include/model.h"

#define ParamError do{printf("Parameter syntax error!\n");return 0;}while(0)

const char * chs = ".`\\\":I!>+~[{)|/frnvzYJL0Zwpbh#W%@";
int test_num  = 4000; // 测试样本数
int show_flag = 0;

Vector1D labels_test;
Vector2D images_test;

void convInput_ARRAY(CVLayer * input, _type** image);
void conv_ARRAY(CVLayer * input, CVLayer * filter[], CVLayer * conv);
void maxPooling_ARRAY(CVLayer * conv, CVLayer * pool) ;
void forePropagation_ARRAY(Network * CNN, _type** image);

int main(int argc, char * argv[]) {

    for (int i = 1; i < argc; ++ i) {
        if (strcmp(argv[i], "--show") == 0) {
            show_flag = 1;
        } else if (strcmp(argv[i], "--num") == 0) {
            if (i == argc - 1) ParamError;
            for (int j = 0; j < strlen(argv[i + 1]); ++ j)
                if (! (argv[i + 1][j] >= '0' && argv[i + 1][j] <= '9')) ParamError;
            test_num = atoi(argv[++ i]);
        }
    }

    // 初始化CNN存储结构
    Network * CNN = loadNetwork("../model.sav");
    
    int image[28][28] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 84, 185, 159, 151, 60, 36, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 222, 254, 254, 254, 254, 241, 198, 198, 198, 198, 198, 198, 198, 198, 170, 52, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 67, 114, 72, 114, 163, 227, 254, 225, 254, 254, 254, 250, 229, 254, 254, 140, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 66, 14, 67, 67, 67, 59, 21, 236, 254, 106, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 83, 253, 209, 18, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 233, 255, 83, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 129, 254, 238, 44, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 59, 249, 254, 62, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 133, 254, 187, 5, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 205, 248, 58, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 126, 254, 182, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 75, 251, 240, 57, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 221, 254, 166, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 203, 254, 219, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 38, 254, 254, 77, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 224, 254, 115, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 133, 254, 254, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 61, 242, 254, 254, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 121, 254, 254, 219, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 121, 254, 207, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    _type** image_ = (_type**)malloc(sizeof(_type*)*28);
    for(int index=0;index<28;index++){
        *(image_+index) = (_type*)malloc(sizeof(_type)*28);
        for(int jndex=0;jndex<28;jndex++){
            image_[index][jndex] = (_type)image[index][jndex];
        }
    }

    forePropagation_ARRAY(CNN, image_);
    int ans = -1; _type sign = -1;
    for (int i = 0; i < class_num; ++ i) {
        if (CNN->fc_output->values[i] > sign) {
            sign = CNN->fc_output->values[i];
            ans = i;
        }
    }
    printf("ans = %d\n", ans);
    return 0;
}

void convInput_ARRAY(CVLayer * input, _type** image) {
    int x = 0;
    for (int k = 0; k < input->H; ++ k)
        for (int i = 0; i < input->L; ++ i)
            for (int j = 0; j < input->W; ++ j)
                input->values[k][i][j] = image[i][j];
}

/**
 * input与filter做卷积
 * @param input     输入层
 * @param filter    卷积核数组
 * @param conv      卷积层存储结果
 * 默认步长值为1
 */
void conv_ARRAY(CVLayer * input, CVLayer * filter[], CVLayer * conv) {
    for (int p = 0; p < conv->H; ++ p) {
        for (int i = 0; i < conv->L; ++ i) {
            for (int j = 0; j < conv->W; ++ j) {
                conv->values[p][i][j] = 0;
                for (int k = 0; k < input->H; ++ k)
                    for (int a = 0; a < filter[0]->L; ++ a)
                        for (int b = 0; b < filter[0]->W; ++ b)
                            conv->values[p][i][j] += input->values[k][i + a][j + b] * filter[p]->values[k][a][b];
                conv->values[p][i][j] = ConvActivate(conv->values[p][i][j] + filter[p]->bias);
            }
        }
    }
}

/**
 * shape=2x2, stride=2 最大池化
 * @param conv 卷积层输出
 * @param pool 池化层输出
 */
// TODO: 池化窗口大小不通用
void maxPooling_ARRAY(CVLayer * conv, CVLayer * pool) {
    for (int k = 0; k < conv->H; ++ k)
        for (int i = 0; i < conv->L; i += 2)
            for (int j = 0; j < conv->W; j += 2)
                pool->values[k][i / 2][j / 2] = _max(
                    _max(conv->values[k][i][j], conv->values[k][i][j + 1]),
                    _max(conv->values[k][i + 1][j],conv->values[k][i + 1][j + 1])
                );
}

void forePropagation_ARRAY(Network * CNN, _type** image){
    convInput_ARRAY(CNN->input_layer, image);
    conv(CNN->input_layer, CNN->filter, CNN->conv_layer);
    maxPooling(CNN->conv_layer, CNN->pool_layer);
    FCInput(CNN->pool_layer, CNN->fc_input);
    FCMultiply(CNN->fc_input, CNN->fc_weight, CNN->fc_output);
    softmax(CNN->fc_output);
}