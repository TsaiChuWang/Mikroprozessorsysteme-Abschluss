
#ifndef DATASET
#define DATASET

#include "../include/configuration.h"

#define SECHS_UND_NEUNZIG

#ifdef SECHS_UND_NEUNZIG

#define IMAGE_SIZE 96
#define ORDNER_BILDER_WEG "/home/wtsaichu/Dokumente/Arbeitsplatz/Dataset/Dataset/Dataset_V1.0/Binary/"
#define ORDNER_TEXT_WEG "/home/wtsaichu/Dokumente/Arbeitsplatz/Dataset/Dataset/Dataset_V1.0/Text/"

#else

#define IMAGE_SIZE 28
#define ORDNER_BILDER_WEG "/home/wtsaichu/Dokumente/Arbeitsplatz/Dataset/Dataset/Dataset_V1.1/Binary/"
#define ORDNER_TEXT_WEG "/home/wtsaichu/Dokumente/Arbeitsplatz/Dataset/Dataset/Dataset_V1.1/Text/"

#endif

#define EINSTUFUNG_ZAHL 7
#define AUSBILDUNG_LANGE 14000
typedef struct datensatz
{
    int dataset_zahl, dataset_size;
    _type* labels;
    _type** dataset;
}DATENSATZ;


/**
 * Mapping the class to the code
 * @param originelle_etikett  Name of the class
 */
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

/**
 * List the folders(named in type) in the dataset root folder
 * @param wurzel_weg  Dataset folder path
 */
char** listen_ordner(const char* wurzel_weg) {
    DIR *ordner;
    struct dirent *eintrag;

    ordner = opendir(wurzel_weg);

    if (ordner == NULL) 
        fprintf(stderr, "Error opening directory %s\n", wurzel_weg);

    char** folders = (char**)malloc(EINSTUFUNG_ZAHL * sizeof(char*));

    rewinddir(ordner);

    int index = 0;
    while ((eintrag = readdir(ordner)) != NULL) {
        if (eintrag->d_type == DT_DIR && eintrag->d_name[0] != '.') {
            folders[index] = strdup(eintrag->d_name);
            index++;
        }
    }
    closedir(ordner);
    return folders;
}

/**
 * Read pixels and store in array
 * @param datei_weg  Txt path of pixel
 */
_type* reden_pixel(char* datei_weg){
    _type* data = (_type*)malloc(sizeof(_type)*IMAGE_SIZE*IMAGE_SIZE);
    FILE* datei = fopen(datei_weg, "r");

    if (datei == NULL)
        fprintf(stderr, "Error opening file: %s\n", datei_weg);

    int zahl = 0;
    char puffer[1024];  

    // Read each line from the file
    while (fgets(puffer, sizeof(puffer), datei) != NULL) {
        puffer[strlen(puffer)-1]='\0';  // Process the line
        if(atoi(puffer) == 0)
            *(data+zahl) = (_type)0;
        else *(data+zahl) = (_type)1;
        zahl++;
    }
    fclose(datei);

    return data;
}

/**
 * Initialize dataset
 */
DATENSATZ erhaltenDatensatz(){
    DATENSATZ daten;
    daten.dataset_zahl = AUSBILDUNG_LANGE;
    daten.dataset_size = IMAGE_SIZE * IMAGE_SIZE;
    daten.labels = (_type*)malloc(sizeof(_type)*AUSBILDUNG_LANGE);
    daten.dataset = (_type**)malloc(sizeof(_type*)*AUSBILDUNG_LANGE);

    int ausbildung_zahl = 0;
    char** etikett =  listen_ordner(ORDNER_TEXT_WEG);
#ifdef DATENSATZ_ANZEIGEN
    for(int index=0;index<EINSTUFUNG_ZAHL;index++)
        printf("type [%d] = %s\n", index, *(etikett+index));
    printf("\n");
#endif

    for(int index=0;index<EINSTUFUNG_ZAHL;index++){
        char* weg = (char*)malloc(sizeof(char)*(sizeof(ORDNER_TEXT_WEG)+sizeof(*(etikett+index))+1));
        strcpy(weg, ORDNER_TEXT_WEG);
        strcat(weg, *(etikett+index));
#ifdef DATENSATZ_ANZEIGEN
        printf("%d : %s \n", index, weg);
#endif

        DIR *ordner;
        struct dirent *eintrag;
        ordner = opendir(weg); // Open the directory

        if (ordner == NULL)
            fprintf(stderr, "Error opening directory %s\n", weg);
        
        while ((eintrag = readdir(ordner)) != NULL) {
            if(strcmp(eintrag->d_name, ".") == EQAUL)
                continue;
            if(strcmp(eintrag->d_name, "..") == EQAUL)
                continue;
            
            char* textweg = (char*)malloc(sizeof(char)*106);
            strcpy(textweg, ORDNER_TEXT_WEG);
            strcat(textweg, *(etikett+index));
            strcat(textweg, "/");
            strcat(textweg, eintrag->d_name);
            
#ifdef DATENSATZ_ANZEIGEN
            printf("Eintrag [%d]: %s(%d) with label %d \n", ausbildung_zahl, textweg, strlen(textweg), kartieren_Etikett(*(etikett+index)));
#endif
            *(daten.labels+ausbildung_zahl) = kartieren_Etikett(*(etikett+index));
            *(daten.dataset+ausbildung_zahl) = reden_pixel(textweg);

            free(textweg);
            ausbildung_zahl++;
        }

        closedir(ordner);
    }
    return daten;
}

/**
 * Print spilit line with length of terminal
 */
void drucken_trenne_line(){
    int terminalWidth;
    FILE* terminalSize = popen("tput cols", "r");
    if (terminalSize == NULL) {
        perror("Error getting terminal width");
        exit(EXIT_FAILURE);
    }

    fscanf(terminalSize, "%d", &terminalWidth);
    pclose(terminalSize);

    for (int i = 0; i < terminalWidth; ++i) {
        printf("-");
    }
    printf("\n");
}

/**
 * Read pixels and store in array
 * @param index  Index of data
 * @param daten  Dataset
 */
void anzeigen_bilder(int index, DATENSATZ* daten){
    drucken_trenne_line();
    printf("Image[%d] with label %d\n\n", index, *(daten->labels+index));
    for(int i=0;i<IMAGE_SIZE;i++){
        for(int j=0;j<IMAGE_SIZE;j++)
            printf("%d", *((*(daten->dataset+index))+(i*IMAGE_SIZE+j)));
        printf("\n");
    }
    drucken_trenne_line();
}

/**
 * Display all the dataset
 * @param daten  Dataset
 */
void anzeigen_datensatz(DATENSATZ daten){
    drucken_trenne_line();
    printf("Dataset with number : %d, Image : %d * %d, and %d class\n", daten.dataset_zahl, IMAGE_SIZE, IMAGE_SIZE, EINSTUFUNG_ZAHL);
    for(int index=0;index<daten.dataset_zahl;index++)
        anzeigen_bilder(index, &daten);
    drucken_trenne_line();
}

/**
 * Shuffle the dataset
 * @param daten  Dataset
 */
void mischen(DATENSATZ daten) {
    srand((unsigned int)time(NULL));  // Seed for random number generation

    for (int i = daten.dataset_zahl - 1; i > 0; --i) {
        int temp_index = rand() % (i + 1);

        _type temp_label = daten.labels[i];
        daten.labels[i] = daten.labels[temp_index];
        daten.labels[temp_index] = temp_label;

        _type* temp_bilder = daten.dataset[i];
        daten.dataset[i] = daten.dataset[temp_index];
        daten.dataset[temp_index] = temp_bilder;
    }
}

#endif