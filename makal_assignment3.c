#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_FILE_NAME 256

// Define struct for movie
struct movie
{
    char *title;
    int year;
    struct movie *next;
};

void mainMenu();
void fileSelectionMenu();

int main(void) {
    mainMenu();
    return 0;

};

// Menu Functions
void mainMenu() {
    int choice;

    do {
        printf("1. Select file to process\n2. Exit the program\nEnter a choice (1 or 2): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fileSelectionMenu();
                break;
            case 2:
                printf("Exiting the program.\n");
                break;
            default:
                fprintf(stderr, "You entered an incorrect choice. Try again.\n");
        }
    } while (choice != 2);  // Loop continues unless user chooses to exit

}

void fileSelectionMenu() {
    int choice;
    char* filename = NULL;

    do{
        printf("Which file you want to process?\n");
        printf("Enter 1 to pick the largest file\n");
        printf("Enter 2 to pick the smallest file\n");
        printf("Enter 3 to specify the name of a file\n");
        printf("Enter a choice from 1 to 3: ");
        scanf("%d", &choice);

        if (choice == 1) {
            filename = findLargestFile();
        } else if (choice == 2) {
            filename = findSmallestFile();
        } else if (choice == 3) {
            filename = specifyFileName();
        } else {
            fprintf(stderr, "You entered an incorrect choice. Try again.\n");
        }
    } while ((choice < 1 || choice > 3) || (choice == 3 && filename == NULL));
    
    if (filename != NULL) {
        printf("Now processing the chosen file named %s\n", filename);
        struct movie *movies = processFile(filename);
        free(filename);

        if (movies != NULL) {
            srand(time(NULL));
            int random_num = rand() % 100000;
            char dir_name[50];
            sprintf(dir_name, "makal.movies.%d", random_num);
            
            if (mkdir(dir_name, 0750) == 0) {
                printf("Created directory with name %s\n", dir_name);
                writeMoviesByYear(movies, dir_name);
            } else {
                perror("Error creating directory");
            }
        }
    }
}
    
// File Handling Functions
char* findLargestFile() {
    DIR *d;
    struct dirent *dir;
    struct stat file_stat;
    char *largest_file = NULL;
    off_t max_size = 0;

    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strncmp(dir->d_name, "movies_", 7) == 0 && strstr(dir->d_name, ".csv") != NULL) {
                if (stat(dir->d_name, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
                    if (file_stat.st_size > max_size) {
                        max_size = file_stat.st_size;
                        free(largest_file);
                        largest_file = strdup(dir->d_name);
                    }
                }
            }
        }
        closedir(d);
    }
    return largest_file;
}

char* findSmallestFile() {
    DIR *d;
    struct dirent *dir;
    struct stat file_stat;
    char *smallest_file = NULL;
    off_t min_size = __LONG_MAX__;

    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strncmp(dir->d_name, "movies_", 7) == 0 && strstr(dir->d_name, ".csv") != NULL) {
                if (stat(dir->d_name, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
                    if (file_stat.st_size < min_size) {
                        min_size = file_stat.st_size;
                        free(smallest_file);
                        smallest_file = strdup(dir->d_name);
                    }
                }
            }
        }
        closedir(d);
    }
    return smallest_file;
}

char* specifyFileName() {
    char filename[MAX_FILE_NAME];
    printf("Enter the complete file name: ");
    scanf("%s", filename);

    if (access(filename, F_OK) == 0) {
        return strdup(filename);
    } else {
        fprintf(stderr, "The file %s was not found. Try again.\n", filename);
        return NULL;
    }
}
// Data Processing Functions
    // processFile()
    // createMovie()
// Output Fuctions
    // writeMoviesByYear()
