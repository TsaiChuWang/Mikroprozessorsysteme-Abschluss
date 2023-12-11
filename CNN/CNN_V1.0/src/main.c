#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/vector.h"
#include "../include/dataio.h"
#include "../include/network.h"
#include "../include/model.h"
#include <time.h>
#include <dirent.h>

#define FOLDER_WEG "/home/wtsaichu/Dokumente/Arbeitsplatz/Dataset/Dataset/Dataset_V1.1/Binary/"
#define FOLDER_TEXT_WEG "/home/wtsaichu/Dokumente/Arbeitsplatz/Dataset/Dataset/Dataset_V1.1/Text/"
#define EINSTUFUNG_ZAHL 7

// #define ANZEIGEN
#define NICHT_GEFUNDEN -1
#define EQAUL 0 

#define IMAGE_SIZE 28
#define TRAIN_SIZE 14000
#define TEST_SIZE 14000
enum labels{
    c = 0,
    first = 1,
    I = 2,
    ok = 3,
    palm = 4,
    palm_moved = 5,
    thumb = 6
};

void showImage(int index, _vector2D_t* image, _vector2D_t label){
    printf("\n\nImage : %d = %d \n", index,  (int)label);
    for(int i=0;i<IMAGE_SIZE;i++){
        for(int j=0;j<IMAGE_SIZE;j++)
            printf("%d", (int)*(image+(i*IMAGE_SIZE+j)));
        printf("\n");
    }
    printf("\n\n\n\n");
}

void list_items(char* folder_path,int typ, int* count) {
    DIR *dir;
    struct dirent *entry;

    // Open the directory
    dir = opendir(folder_path);

    // Check if the directory opened successfully
    if (dir == NULL) {
        fprintf(stderr, "Error opening directory %s\n", folder_path);
        exit(EXIT_FAILURE);
    }
    int c = 0;
    // Read and print each entry in the directory
    while ((entry = readdir(dir)) != NULL) {
        // char* alleweg = (char*)malloc(sizeof(char)*(strlen(folder_path)+strlen(entry->d_name)+1));
        char* alleweg = (char*)malloc(sizeof(char)*90);
        strcpy(alleweg, folder_path);
        strcat(alleweg, "/");
        strcat(alleweg, entry->d_name);
        // printf("alle_weg = %s, typ = %d %d \n",alleweg, typ, strlen(folder_path)+strlen(entry->d_name));
        // printf("%s\n", entry->d_name);
        *count++;
        free(alleweg);
    }
    // Close the directory
    closedir(dir);
}

_vector2D_t* readPixel(char* file_path){
    _vector2D_t* data = (_vector2D_t*)malloc(sizeof(_vector2D_t)*IMAGE_SIZE*IMAGE_SIZE);
    // printf("path = %s\n", file_path);
    FILE* file = fopen(file_path, "r");

    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", file_path);
        exit(EXIT_FAILURE);
    }

    int c;
    int count = 0;
    char buffer[1024];  // Adjust the buffer size as needed

    // Read each line from the file
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Process the line
        buffer[strlen(buffer)-1]='\0';
        // printf("Line: %s = %3d = %f \n",buffer, atoi(buffer), (_vector2D_t)atoi(buffer));
        if(atoi(buffer) == 0)
            *(data+count) = (_vector2D_t)0;
        else *(data+count) = (_vector2D_t)1;
        // *(data+count) = (_vector2D_t)atoi(buffer);
        count++;
    }

    // printf("len = %d \n", count);

    fclose(file);
    return data;
}

int kartieren_Etikett(char* originelle_etikett){
    if(strcmp(originelle_etikett, "c") == 0)
        return 0;
    if(strcmp(originelle_etikett, "first") == 0)
        return 1;
    if(strcmp(originelle_etikett, "I") == 0)
        return 2;
    if(strcmp(originelle_etikett, "ok") == 0)
        return 3;
    if(strcmp(originelle_etikett, "palm") == 0)
        return 4;
    if(strcmp(originelle_etikett, "palm_moved") == 0)
        return 5;
    if(strcmp(originelle_etikett, "thumb") == 0)
        return 6;
    return NICHT_GEFUNDEN;  
}
char** list_folders(const char* root_path, int* num_folders) {
    DIR *dir;
    struct dirent *entry;
    int folder_count = 0;

    // Open the directory
    dir = opendir(root_path);

    // Check if the directory opened successfully
    if (dir == NULL) {
        fprintf(stderr, "Error opening directory %s\n", root_path);
        exit(EXIT_FAILURE);
    }

    // Count the number of subdirectories
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
            folder_count++;
        }
    }

    // Allocate memory for folder names
    char** folders = (char**)malloc(folder_count * sizeof(char*));

    // Rewind the directory stream
    rewinddir(dir);

    int index = 0;
    // Store folder names
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
            folders[index] = strdup(entry->d_name);
            index++;
        }
    }

    // Close the directory
    closedir(dir);

    *num_folders = folder_count;
    return folders;
}

char** erhalten_originelle_Etikett_von_Daten(){
    DIR *dir = opendir(FOLDER_WEG);

    if (dir == NULL) 
        perror("Unable to open directory");
    
    char** labels = (char**)malloc(sizeof(char*)*EINSTUFUNG_ZAHL);
    int index=0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) 
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                // printf("%s\n", entry->d_name);
                *(labels+index) = (char*)malloc(sizeof(char)*256);
                *(labels+index) = entry->d_name;
                index++;
            }
    }
#ifdef ANZEIGEN
    for(int i=0;i<EINSTUFUNG_ZAHL;i++)
        printf("%s : %d \n", *(labels+i), kartieren_Etikett(*(labels+i)));
#endif
    // Close the directory
    closedir(dir);

    return labels;
}

#define ParamError do{ \
    printf("Parameter syntax error!\n"); \
    return 0; \
}while(0)

int train_num = 6000; // 训练样本数
int test_num  = TEST_SIZE; // 测试样本数
int epoch_num = 300; // 训练轮数
int batch_size = 1;

Vector1D labels_train;
Vector2D images_train;
Vector1D labels_test;
Vector2D images_test;

void shuffle(Vector1D labels, Vector2D images) {
    srand((unsigned int)time(NULL));  // Seed for random number generation

    for (int i = labels.lens - 1; i > 0; --i) {
        int j = rand() % (i + 1);

        // Swap array[i] and array[j]
        int temp = labels.data[i];
        labels.data[i] = labels.data[j];
        labels.data[j] = temp;

        _vector2D_t* temp_ = images.data[i];
        images.data[i] = images.data[j];
        images.data[j] = temp_;
    }
}
void train(Network * CNN, Alpha * alpha, const char * fileName) {
    double best_score = 0, new_score;
    printf("Begin Training ...\n");
    for (int step = 0; step < epoch_num; ++ step) {
        _type err = 0;
        // for (int m = 0; m < train_num / batch_size; m ++) {
            // printf("epoch_num = %d, m =%d/%d\n", step, m, train_num / batch_size);
            // int i = rand() % (train_num / batch_size);
            // for (int k = 0; k < class_num; ++ k)
            //     CNN->fc_output->deltas[k] = 0;
            // for (int j = 0; j < batch_size; ++ j) {
            //     forePropagation(CNN, i * batch_size + j, images_train);
        //         int label = (int)labels_train.data[i * batch_size + j];
        //         for (int k = 0; k < class_num; ++ k)
        //             CNN->fc_output->deltas[k] += CNN->fc_output->values[k];
        //         CNN->fc_output->deltas[label] -= 1.0;
        //         err -= log(CNN->fc_output->values[label]);
            // }
        //     for (int k = 0; k < class_num; ++ k)
        //         CNN->fc_output->deltas[k] /= batch_size;
        //     backPropagation(CNN, step, alpha);
        // }
        // new_score = test(CNN);
        // printf("step: %3d loss: %.5f prec: %.5f\n",
        //     step, err / train_num, new_score);
        // if (new_score > best_score) {
        //     saveNetwork(CNN, fileName);
        //     best_score = new_score;
        // }
    }
    printf("Best Score: %f\nmodel saved to model.sav!\n", best_score);
}

/**
 * 将从文件中读出的图像reshape到输入层
 * @param index  样本序号
 * @param A      神经网络的输入层
 * @param isTest 0代表训练，1代表测试
 */
void convInput_(int index, CVLayer * input, Vector2D images) {
    int x = 0;
    // printf("input->H = %d, input->L = %d input->W = %d index = %d \n", input->H, input->L, input->W, index);
    for (int k = 0; k < input->H; ++ k)
        for (int i = 0; i < input->L; ++ i)
            for (int j = 0; j < input->W; ++ j){
                // printf("input->values[%d][%d][%d] = images.data[%d][%d] = %f\n", k ,i, j, index, x, images.data[index][x]);
    //             // printf()
                x+=1;
            }
            // printf("x = %d\n", x);
}

/**
 * shape=2x2, stride=2 最大池化
 * @param conv 卷积层输出
 * @param pool 池化层输出
 */
// TODO: 池化窗口大小不通用
void maxPooling_(CVLayer * conv, CVLayer * pool) {
    for (int k = 0; k < conv->H; ++ k)
        for (int i = 0; i < conv->L; i += 2)
            for (int j = 0; j < conv->W; j += 2)
                pool->values[k][i / 2][j / 2] = _max(
                    _max(conv->values[k][i][j], conv->values[k][i][j + 1]),
                    _max(conv->values[k][i + 1][j],conv->values[k][i + 1][j + 1])
                );
}

/**
 * input与filter做卷积
 * @param input     输入层
 * @param filter    卷积核数组
 * @param conv      卷积层存储结果
 * 默认步长值为1
 */
void conv_(CVLayer * input, CVLayer * filter[], CVLayer * conv) {
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
 * 池化层的输出reshape到全连接神经网络
 * @param pool    池化层的输出
 * @param fcInput 全连接层的输入
 */
void FCInput_(CVLayer * pool, FCLayer * fcInput) {
    int x = 0;
    for (int k = 0; k < pool->H; ++ k)
        for (int i = 0; i < pool->L; ++ i)
            for (int j = 0; j < pool->W; ++ j)
                fcInput->values[x ++] = poolActivate(pool->values[k][i][j]);
}
/**
 * 全连接层参数矩阵乘法
 * @param input  输入
 * @param weight 参数矩阵
 * @param output 输出
 * @return
 */
void FCMultiply_(FCLayer * input, FCLayer * weight, FCLayer * output) {
    for (int i = 0; i < output->L; ++ i) {
        output->values[i] = 0;
        for (int j = 0; j < input->L; ++ j)
            output->values[i] += weight->weights[i][j] * input->values[j];
        output->values[i] += weight->bias[i];
    }
}

int predict(Network * CNN, int t) {
    forePropagation(CNN, t, images_test);
    // argmax获得预测的类别序号
    int ans = -1; _type sign = -1;
    for (int i = 0; i < class_num; ++ i) {
        // printf("CNN->fc_output->values[%d] = %f, sign = %f\n", i,CNN->fc_output->values[i], sign);
        if (CNN->fc_output->values[i] > sign) {
            sign = CNN->fc_output->values[i];
            ans = i;
        }
    }
    return ans;
}

double test(Network * CNN) {
    int sum = 0;
    for (int i = 0; i < test_num; ++ i){
        // printf("predict(CNN, i) = %d\n", predict(CNN, i));
        // printf("predict(CNN, %d) = %d , (int)labels_test.data[%d] = %d equal = %d sum = %d \n", i, predict(CNN, i), i, (int)labels_test.data[i], predict(CNN, i) == (int)labels_test.data[i], sum);
        if (predict(CNN, i) == (int)labels_test.data[i]) sum ++;
        // printf("test[%d] = %d\n", i, (int)labels_test.data[i]);
    }
    return 1.0 * sum / test_num;
}

int main(int argc, char * argv[]) {

    srand((unsigned)time(NULL));

    for (int i = 1; i < argc; ++ i) {
        if (!strcmp(argv[i], "--epoch")
            || !strcmp(argv[i], "--test_num")
            || !strcmp(argv[i], "--train_num")) {
            if (i == argc - 1) ParamError;
            for (int j = 0; j < strlen(argv[i + 1]); ++ j)
                if (! (argv[i + 1][j] >= '0' && argv[i + 1][j] <= '9')) ParamError;
            if (!strcmp(argv[i], "--epoch")) epoch_num = atoi(argv[++ i]);
            else if (!strcmp(argv[i], "--test_num")) test_num = atoi(argv[++ i]);
            else if (!strcmp(argv[i], "--train_num")) train_num = atoi(argv[++ i]);
        } else
        if (!strcmp(argv[i], "other")) {
            (void)"pass";
        }
    }

    // 初始化学习率
    Alpha * alpha = ExpDecayLR(0.03, 100, 0.003);

    // 初始化CNN存储结构
    Network * CNN = Network_();

    // 读取数据集
    // labels_train = read_Mnist_Label("../dataset/train-labels.idx1-ubyte");
    // labels_train = erhaltenlabel();
    // images_train = read_Mnist_Images("../dataset/train-images.idx3-ubyte");


    labels_train = Vector1D_();
    _vector1D_t* labels_train_data = (_vector1D_t*)malloc(sizeof(_vector1D_t)*(TRAIN_SIZE));

    images_train = Vector2D_();
    int typ_length, train_length = 0;
    _vector2D_t ** images_train_data = (_vector2D_t**)malloc(sizeof(_vector2D_t*)*(TRAIN_SIZE));

    char** etikett =  list_folders(FOLDER_WEG, &typ_length);
    for(int index=0;index<EINSTUFUNG_ZAHL;index++){
        char* weg = (char*)malloc(sizeof(char)*(sizeof(FOLDER_WEG)+sizeof(*(etikett+index))+1));
        strcpy(weg, FOLDER_WEG);
        strcat(weg, *(etikett+index));
#ifdef ANZEIGEN
        printf("%d : %s \n", index, weg);
#endif
        DIR *dir;
        struct dirent *entry;
        dir = opendir(weg); // Open the directory

        if (dir == NULL) {  // Check if the directory opened successfully
            fprintf(stderr, "Error opening directory %s\n", weg);
            exit(EXIT_FAILURE);
        }
        // Read and print each entry in the directory
        while ((entry = readdir(dir)) != NULL) {
            if(strcmp(entry->d_name, ".") == EQAUL)
                continue;
            if(strcmp(entry->d_name, "..") == EQAUL)
                continue;
            
            char* alleweg = (char*)malloc(sizeof(char)*106);
            strcpy(alleweg, weg);
            strcat(alleweg, "/");
            strcat(alleweg, entry->d_name);
            
            char* textweg = (char*)malloc(sizeof(char)*106);
            strcpy(textweg, FOLDER_TEXT_WEG);
            strcat(textweg, *(etikett+index));
            strcat(textweg, "/");
            strcat(textweg, entry->d_name);
            *(textweg+strlen(textweg)-1) = 't';
            *(textweg+strlen(textweg)-2) = 'x';
            *(textweg+strlen(textweg)-3) = 't';
            
            // if(kartieren_Etikett(*(etikett+index)) == NICHT_GEFUNDEN)
            //     printf("%d\n", kartieren_Etikett(*(etikett+index)));
#ifdef ANZEIGEN
            printf("alle_weg = %s(%d), typ = %d text_weg = %s %d\n",alleweg, strlen(alleweg), kartieren_Etikett(*(etikett+index)), textweg, strlen(textweg));
#endif
            *(labels_train_data+train_length) = kartieren_Etikett(*(etikett+index));
            *(images_train_data+train_length) = readPixel(textweg);
            // FILE* file = fopen(textweg, "r");
            
            train_length++;
            free(alleweg);
            free(textweg);
            // fclose(file);
        }
        // Close the directory
        closedir(dir);
        free(weg);
    }

    labels_train.lens = train_length;
    labels_train.data = labels_train_data;

    images_train.rows = train_length;
    images_train.cols = IMAGE_SIZE * IMAGE_SIZE;
    images_train.data = images_train_data;

    printf("\nlength = %d\n", labels_train.lens);
    printf("rows = %d, columns = %d \n",images_train.rows,images_train.cols);
    
    shuffle(labels_train, images_train);

    // for(int index=0;index<images_train.rows;index++)
    //     showImage(index, *(images_train.data+index), *(labels_train.data+index));


    labels_test = Vector1D_();
    _vector1D_t* labels_test_data = (_vector1D_t*)malloc(sizeof(_vector1D_t)*(TRAIN_SIZE));

    images_test = Vector2D_();
    int test_typ_length, test_length = 0;
    _vector2D_t ** images_test_data = (_vector2D_t**)malloc(sizeof(_vector2D_t*)*(TRAIN_SIZE));

    char** etikett_test =  list_folders(FOLDER_WEG, &test_typ_length);
    for(int index=0;index<EINSTUFUNG_ZAHL;index++){
        char* weg = (char*)malloc(sizeof(char)*(sizeof(FOLDER_WEG)+sizeof(*(etikett_test+index))+1));
        strcpy(weg, FOLDER_WEG);
        strcat(weg, *(etikett_test+index));
#ifdef ANZEIGEN
        printf("%d : %s \n", index, weg);
#endif
        DIR *dir;
        struct dirent *entry;
        dir = opendir(weg); // Open the directory

        if (dir == NULL) {  // Check if the directory opened successfully
            fprintf(stderr, "Error opening directory %s\n", weg);
            exit(EXIT_FAILURE);
        }
        // Read and print each entry in the directory
        while ((entry = readdir(dir)) != NULL) {
            if(strcmp(entry->d_name, ".") == EQAUL)
                continue;
            if(strcmp(entry->d_name, "..") == EQAUL)
                continue;
            
            char* alleweg = (char*)malloc(sizeof(char)*106);
            strcpy(alleweg, weg);
            strcat(alleweg, "/");
            strcat(alleweg, entry->d_name);
            
            char* textweg = (char*)malloc(sizeof(char)*106);
            strcpy(textweg, FOLDER_TEXT_WEG);
            strcat(textweg, *(etikett+index));
            strcat(textweg, "/");
            strcat(textweg, entry->d_name);
            *(textweg+strlen(textweg)-1) = 't';
            *(textweg+strlen(textweg)-2) = 'x';
            *(textweg+strlen(textweg)-3) = 't';
            
            // if(kartieren_Etikett(*(etikett+index)) == NICHT_GEFUNDEN)
            //     printf("%d\n", kartieren_Etikett(*(etikett+index)));
#ifdef ANZEIGEN
            printf("alle_weg = %s(%d), typ = %d text_weg = %s %d\n",alleweg, strlen(alleweg), kartieren_Etikett(*(etikett+index)), textweg, strlen(textweg));
#endif
            *(labels_test_data+ test_length) = kartieren_Etikett(*(etikett_test+index));
            *(images_test_data+ test_length) = readPixel(textweg);
            // FILE* file = fopen(textweg, "r");
            
             test_length++;
            free(alleweg);
            free(textweg);
            // fclose(file);
        }
        // Close the directory
        closedir(dir);
        free(weg);
    }

    labels_test.lens =  test_length;
    labels_test.data = labels_test_data;

    images_test.rows =  test_length;
    images_test.cols = IMAGE_SIZE * IMAGE_SIZE;
    images_test.data = images_test_data;

    labels_test.lens = test_length;
    labels_test.data = labels_test_data;

    images_test.rows = test_length;
    images_test.cols = IMAGE_SIZE * IMAGE_SIZE;
    images_test.data = images_test_data;

    printf("\nlength = %d\n", labels_test.lens);
    printf("rows = %d, columns = %d \n",images_test.rows,images_test.cols);
    shuffle(labels_test, images_test);

    // // 训练
    // train(CNN, alpha, "../model.sav");
    if(images_train.data == NULL)
        printf("OAO\n");

    double best_score = 0, new_score;
    printf("Begin Training ...\n");
    for (int step = 0; step < epoch_num; ++ step) {
        _type err = 0;
        for (int m = 0; m < train_num / batch_size; m ++) {
            int i = rand() % (train_num / batch_size);
            for (int k = 0; k < class_num; ++ k)
                CNN->fc_output->deltas[k] = 0;
            for (int j = 0; j < batch_size; ++ j) {
                forePropagation(CNN, i * batch_size + j, images_train);
                int label = (int)labels_train.data[i * batch_size + j];
                // printf("label = %d\n",label);
                for (int k = 0; k < class_num; ++ k){
                    CNN->fc_output->deltas[k] += CNN->fc_output->values[k];
                    // printf(" CNN->fc_output->deltas[k] = %f\n",  CNN->fc_output->deltas[k]);
                    // printf(" + CNN->fc_output->values[%d] = %d= CNN->fc_output->deltas[%d] = %d\n", k, (int)CNN->fc_output->values[k], k, (int)CNN->fc_output->deltas[k]);
                }
                    
                CNN->fc_output->deltas[label] -= 1.0;
                // for (int k = 0; k < class_num; ++ k){
                //     printf(" CNN->fc_output->deltas[%d] = %f label = %d \n", k, CNN->fc_output->deltas[k],label);
                // }
                if(CNN->fc_output->values[label] != (_type)0)
                    err -= log(CNN->fc_output->values[label]);
                // printf("CNN->fc_output->values[label] = %f %f label = %d\n", log(CNN->fc_output->values[label]), CNN->fc_output->values[label],label);
                // printf("err = %f\n", err);
            }
            for (int k = 0; k < class_num; ++ k){
                CNN->fc_output->deltas[k] /= batch_size;
                printf("CNN->fc_output->deltas[%d] = %f\n", k, CNN->fc_output->deltas[k]);
            }
                
            backPropagation(CNN, step, alpha);
        }

        new_score = test(CNN);
        printf("step: %3d loss: %.5f prec: %.5f\n", step, err / train_num, new_score);
        if (new_score > best_score) {
            saveNetwork(CNN, "../model.sav");
            best_score = new_score;
        }
    }
    printf("Best Score: %f\nmodel saved to model.sav!\n", best_score);
    // for(int index=0;index<test_length;index++)
    //     printf(" %d ",(int)*(labels_test.data+index));
    printf("\n");
    // for(int index=0;index<images_test.rows;index++)
    //     showImage(index, *(images_test.data+index), *(labels_test.data+index));


    // 释放内存
    // delete_p(CNN);
    // delete(labels_train);
    // delete(images_train);
    // delete(labels_test);
    // delete(images_test);
    return 0;
}
