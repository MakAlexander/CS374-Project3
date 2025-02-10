/*
Movie File Processor Program
My program reads CSV files containing movie data, processes the data into a linked list of movie structures,
and allows the user to interact with the data through a menu.

The user can:
1. Select a file to process based on size (largest, smallest) or specify a file name.
2. Process the selected file to create a directory named "makal.movies.[random_number]".
3. Parse movies by their release year and write each year's movies to separate txt files within the created directory.
4. Exit the program.

Each new directory has 'rwxr-x---' permissions, while the output txt files for each year have 'rw-r-----' permissions.
*/


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>

#define MAX_FILE_NAME 256
#define MIN_FILE_SIZE 2000000000 // =(2 gb). Hoping no CSV file will be larger than that

// Define struct for movie
struct movie
{
    char *title;
    int year;
    struct movie *next;
};

void mainMenu();
void fileSelectionMenu();
char* findLargestFile();
char* findSmallestFile();
char* specifyFileName();
struct movie* processFile(const char* filename);
void writeMoviesByYear(struct movie *head, const char *dirName);

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
    } while (choice != 2);

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
    
    // Creating new directories for the movies
        if (movies != NULL) {
            srand(time(NULL));
            int random_num = rand() % 100000;
            char moviesDir[20];
            sprintf(moviesDir, "makal.movies.%d", random_num);
            
            if (mkdir(moviesDir, 0750) == 0) {           // rwxr-x--- permission
                printf("Created directory with name %s\n", moviesDir);
                writeMoviesByYear(movies, moviesDir);
            } else {
                perror("Error creating directory");
            }
        }
    }
}
    
// File Handling Functions
char* findLargestFile() {
    DIR *dir;
    struct dirent *movie_file;
    struct stat file_stat;
    char *largest_file = NULL;
    off_t max_size = 0;

    dir = opendir(".");
    if (dir) {
        while ((movie_file = readdir(dir)) != NULL) {
            if (strncmp(movie_file->d_name, "movies_", 7) == 0 && strstr(movie_file->d_name, ".csv") != NULL) {
                if (stat(movie_file->d_name, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
                    if (file_stat.st_size > max_size) {
                        max_size = file_stat.st_size;
                        free(largest_file);
                        largest_file = strdup(movie_file->d_name);
                    }
                }
            }
        }
        closedir(dir);
    }
    return largest_file;
}

char* findSmallestFile() {
    DIR *dir;
    struct dirent *movie_file;
    struct stat file_stat;
    char *smallest_file = NULL;
    off_t min_size = MIN_FILE_SIZE;

    dir = opendir(".");
    if (dir) {
        while ((movie_file = readdir(dir)) != NULL) {
            if (strncmp(movie_file->d_name, "movies_", 7) == 0 && strstr(movie_file->d_name, ".csv") != NULL) {
                if (stat(movie_file->d_name, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
                    if (file_stat.st_size < min_size) {
                        min_size = file_stat.st_size;
                        free(smallest_file);
                        smallest_file = strdup(movie_file->d_name);
                    }
                }
            }
        }
        closedir(dir);
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
struct movie* createMovie(char *line) {
    struct movie *newMovie = malloc(sizeof(struct movie));
    char *token;
    char *saveptr;

    token = strtok_r(line, ",", &saveptr);
    newMovie->title = strdup(token);

    token = strtok_r(NULL, ",", &saveptr);
    newMovie->year = atoi(token);

    newMovie->next = NULL;
    return newMovie;
}

struct movie* processFile(const char* filename) {
    int file_descriptor = open(filename, O_RDONLY);
    if (file_descriptor == -1) {
        perror("Error opening file");
        return NULL;
    }

    FILE *file = fdopen(file_descriptor, "r");
    if (!file) {
        perror("Error converting file descriptor to file pointer");
        close(file_descriptor);
        return NULL;
    }

    char *line = NULL;
    size_t len = 0;
    struct movie *head = NULL, *tail = NULL;
    getline(&line, &len, file);

    while (getline(&line, &len, file) != -1) {
        struct movie *newMovie = createMovie(line);
        if (!head) {
            head = newMovie;
            tail = newMovie;
        } else {
            tail->next = newMovie;
            tail = newMovie;
        }
    }

    free(line);
    fclose(file);
    return head;
}
// Output
void writeMoviesByYear(struct movie *head, const char *dirName) {
    while (head) {
        char filePath[MAX_FILE_NAME];
        sprintf(filePath, "%s/%d.txt", dirName, head->year);

        int file_descriptor = open(filePath, O_WRONLY | O_CREAT | O_APPEND, 0640); // rw-r----- permission
        if (file_descriptor != -1) {
            dprintf(file_descriptor, "%s\n", head->title);
            close(file_descriptor);
        } else {
            perror("Error creating year file");
        }
        head = head->next;
    }
}
