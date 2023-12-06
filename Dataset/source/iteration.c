#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "../include/inc.h"
const char *inputFolder = "/home/wtsaichu/Dokumente/Arbeitsplatz/Dataset/Dataset/Dataset_minimized/";
const char *grayscaleFolder = "/home/wtsaichu/Dokumente/Arbeitsplatz/Dataset/Dataset/Dataset_grayscale/";
const char *crop_grayscaleFolder = "/home/wtsaichu/Dokumente/Arbeitsplatz/Dataset/Dataset/Dataset_crop/";

#define LABEL_NUMBER 7
enum Label {
    C,
    FIRST,
    I,  
    OK,
    PALM,
    PALM_MOVED,
    THUMB
};



int main(){
    enum Label labels[]={
        C,
        FIRST,
        I,  
        OK,
        PALM,
        PALM_MOVED,
        THUMB
    };

    DIR *dir;
    struct dirent *entry;

    for(int i=0;i<LABEL_NUMBER;i++){
        char pathName[100];
        char outputpathName[100];
        strcpy(pathName, inputFolder);
        strcpy(outputpathName, grayscaleFolder);
        switch (labels[i])
        {
            case C:
                strcat(pathName, "c/");
                strcat(outputpathName, "c/");
                break;
            case FIRST:
                strcat(pathName, "first/");
                strcat(outputpathName, "first/");
                break;
            case I:
                strcat(pathName, "I/");
                strcat(outputpathName, "I/");
                break;
            case OK:
                strcat(pathName, "ok/");
                strcat(outputpathName, "ok/");
                break;
            case PALM:
                strcat(pathName, "palm/");
                strcat(outputpathName, "palm/");
                break;
            case PALM_MOVED:
                strcat(pathName, "palm_moved/");
                strcat(outputpathName, "palm_moved/");
                break;
            case THUMB:
                strcat(pathName, "thumb/");
                strcat(outputpathName, "thumb/");
                break;
            default:
                strcat(pathName, "c/");
                strcat(outputpathName, "c/");
                break;
        }
        int result = mkdir(outputpathName, S_IRWXU);

        if (result == 0) {
            printf("Folder created successfully.\n");
        } else {
            perror("Error creating folder");
        }

        dir = opendir(pathName);
        if (!dir){
            perror("Error opening directory");
            return 1;
        }

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) {
                char filename[100];
                switch (labels[i])
                {
                    case C:
                        strcpy(filename, "c/");
                        break;
                    case FIRST:
                        strcpy(filename, "first/");
                        break;
                    case I:
                        strcpy(filename, "I/");
                        break;
                    case OK:
                        strcpy(filename, "ok/");
                        break;
                    case PALM:
                        strcpy(filename, "palm/");
                        break;
                    case PALM_MOVED:
                        strcpy(filename, "palm_moved/");
                        break;
                    case THUMB:
                        strcpy(filename, "thumb/");
                        break;
                    default:
                        strcpy(filename, "c/");
                        break;
                }
                strcat(filename, entry->d_name);
                printf("%s\n", filename);
                
                char turn_gray_scale_command[300];
                strcpy(turn_gray_scale_command, "../bin/grayscale ");
                strcat(turn_gray_scale_command, INPUT_FOLDER_PATH);
                strcat(turn_gray_scale_command, filename);
                strcat(turn_gray_scale_command, " ");
                strcat(turn_gray_scale_command, GRAYSCALE_FOLDER_PATH);
                strcat(turn_gray_scale_command, filename);
                printf("command=%s\n", turn_gray_scale_command);
                // system(turn_gray_scale_command);

                char crop_gray_scale_command[300];
                strcpy(crop_gray_scale_command, "../bin/crop ");
                strcat(crop_gray_scale_command, GRAYSCALE_FOLDER_PATH);
                strcat(crop_gray_scale_command, filename);
                strcat(crop_gray_scale_command, " ");
                strcat(crop_gray_scale_command, CROP_GRAYSCALE_FOLDER_PATH);
                strcat(crop_gray_scale_command, filename);
                // printf("command=%s\n", crop_gray_scale_command); 
                
                
                // system(crop_gray_scale_command);
            }
        }
        closedir(dir);
    }
    return 0;
}
    

    
        
    
            