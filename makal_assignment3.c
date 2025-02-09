#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie
{
    char *title;
    int year;
    struct movie *next;
};

void mainMenu();

int main() {
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
                //fileSelectionMenu();
                break;
            case 2:
                printf("Exiting the program.\n");
                break;
            default:
                fprintf(stderr, "You entered an incorrect choice. Try again.\n");
        }
    } while (choice != 2);  // Loop continues unless user chooses to exit

}

// fileSelectionMenu()
// File Handling Functions
    // largestFile()
    // smallestFile()
    // byFileName()
// Data Processing Functions
    // processFile()
    // createMovie()
// Output Fuctions
    // writeMoviesByYear()
