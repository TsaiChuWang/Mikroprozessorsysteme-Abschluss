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



int test(Network * CNN) {
    int sum = 0;
    for (int i = 0; i < test_num; ++ i) {
        int ans = predict(CNN, i), fact = (int)labels_test.data[i];
        if (show_flag) {
            printf("Index: %5d\n", i);
            int x = 0;
            for (int a = 0; a < 28; ++ a) {
                for (int b = 0; b < 28; ++ b)
//                    printf("%3d ", (int)(images_test.data[i][x ++] * 255));
                    printf("%c ", chs[(int)(images_test.data[i][x ++] * 32)]);
                printf("\n");
            }
            printf("Ground Truth: %2d, Result Predicted: %2d: ", fact, ans);
            printf(ans == fact ? "CORRECT\n": "  WRONG\n");
            if (i != test_num - 1) {
                getchar();
                for (int j = 0; j < 3999; ++ j) printf("\b");
            }
        }
        if (ans == fact) sum ++;
    }
    return sum;
}

/**
 * 将从文件中读出的图像reshape到输入层
 * @param index  样本序号
 * @param A      神经网络的输入层
 * @param isTest 0代表训练，1代表测试
 */
void convInput_ARRAY(CVLayer * input, _type** image);

void forePropagation_ARRAY(Network * CNN, _type** image) {
    convInput_ARRAY(CNN->input_layer, image);
    // conv(CNN->input_layer, CNN->filter, CNN->conv_layer);
    // maxPooling(CNN->conv_layer, CNN->pool_layer);
    // FCInput(CNN->pool_layer, CNN->fc_input);
    // FCMultiply(CNN->fc_input, CNN->fc_weight, CNN->fc_output);
    // softmax(CNN->fc_output);
}

int predict(Network * CNN, _type** image) {
    forePropagation_ARRAY(CNN, image);
    // argmax获得预测的类别序号
    int ans = -1; _type sign = -1;
    // for (int i = 0; i < class_num; ++ i) {
    //     if (CNN->fc_output->values[i] > sign) {
    //         sign = CNN->fc_output->values[i];
    //         ans = i;
    //     }
    // }
    return ans;
}

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

    // 读取数据集
    // labels_test = read_Mnist_Label("../dataset/t10k-labels.idx1-ubyte");
    // images_test = read_Mnist_Images("../dataset/t10k-images.idx3-ubyte");

    // 对图片像素进行归一化
    // for (int i = 0; i < images_test.rows; ++ i)
    //     for (int j = 0; j < images_test.cols; ++ j)
    //         images_test.data[i][j] /= 255.0;
    // int sum = test(CNN);
    // printf("Total: %5d, Correct: %5d, Precision: %f\n",
    //     test_num, sum, 1.0 * sum / test_num);

    //         for (int i = 0; i < images_test.rows; ++ i){
    //     printf("\n");
    //     for (int j = 0; j < images_test.cols; ++ j)
    //     {
            
    //         // printf(" %03.1f ", images_test.data[i][j]);
    //         images_test.data[i][j] /= 255.0;
    //     }
    // }
    // int sum = test(CNN);
    // printf("Total: %5d, Correct: %5d, Precision: %f\n",
    //     test_num, sum, 1.0 * sum / test_num);
    
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

    _type** image_ = (double**)malloc(sizeof(double*)*28);
    for(int index=0;index<28;index++){
        *(image_+index) = (double*)malloc(sizeof(double)*28);
        for(int jndex=0;jndex<28;jndex++){
            image_[index][jndex] = (double)image[index][jndex];
        }
    }

    int sum = 0;
    convInput_ARRAY(CNN->input_layer, image);

    // int ans = predict(CNN, image_);//, fact = (int)labels_test.data[0];
    // printf("label =%d\n", *(labels_test.data));
//     return sum;

    // 释放内存
    delete_p(CNN);
    // delete(labels_test);
    // delete(images_test);
    return 0;
}

void convInput_ARRAY(CVLayer * input, _type** image) {
    int x = 0;
    for (int k = 0; k < input->H; ++ k)
        for (int i = 0; i < input->L; ++ i)
            for (int j = 0; j < input->W; ++ j)
                input->values[k][i][j] = image[i][j];
}
