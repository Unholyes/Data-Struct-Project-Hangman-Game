#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define WIDTH 50  // Console width for centering
#define MAX_WORDS 5  // Maximum words per category

// Struct for categories
typedef struct {
    char categoryName[20];
    char words[MAX_WORDS][20];
} Category;

// Function prototypes
void displayMenu();
void startGame();
void viewScoreboard();
void printCentered(const char *text, int newLine);
void printHangman(int attempt);
void waitForKeypress();
char* getRandomWord(Category selectedCategory);

int main() {
    int choice;
    
    do {
        displayMenu();
        printf("\n");
        printCentered("Enter your choice:",0);
        fflush(stdout);  // Flush output buffer to prevent newline issue  
        scanf(" %d", &choice);
        getchar(); // Clear newline character from buffer

        switch (choice) {
            case 1:
                startGame();
                break;
            case 2:
                viewScoreboard();
                break;
            case 3:
                printCentered("Exiting game. Goodbye!",1);
                break;
            default:
                printCentered("Invalid choice! Please try again.",1);
        }
    } while (choice != 3);

    return 0;
}

// Function to display the main menu with ASCII Art
void displayMenu() {
    system("cls"); // Clear console (Windows). Use system("clear") for Linux/Mac.

    printf("\n");
    printCentered("======================================",1);
    printCentered("  _    _                                         ",1);
    printCentered(" | |  | |                                        ",1);
    printCentered(" | |__| | __ _ _ __   __ _ _ __ ___   __ _ _ __  ",1);
    printCentered(" |  __  |/ _` | '_ \\ / _` | '_ ` _ \\ / _` | '_ \\ ",1);
    printCentered(" | |  | | (_| | | | | (_| | | | | | | (_| | | | |",1);
    printCentered(" |_|  |_|\\__,_|_| |_|\\__, |_| |_| |_|\\__,_|_| |_|",1);
    printCentered("                     __/ |                      ",1);
    printCentered("                    |___/                       ",1);
    printCentered("======================================",1);
    
    printf("\n");
    printCentered("1. Start Game",1);
    printCentered("2. View Scoreboard",1);
    printCentered("3. Exit",1);
    printf("\n");
}

// Function to center text
void printCentered(const char *text, int newLine) { 
    int columns = 80;  // Assume a default width
    int textLength = strlen(text);
    int padding = (columns - textLength) / 2;
    if (padding < 0) padding = 0;

    // Print spaces, then text
    printf("%*s%s", padding, "", text);  

    // Add newline only if requested
    if (newLine) {
        printf("\n");
    }
}

// Function to start the game
void startGame() {
    system("cls");

    // Define categories
    Category categories[] = { 
    {"Computers", {"Keyboard", "Monitor", "Mouse", "Motherboard", "Laptop"}},
    {"Food", {"Pizza", "Burger", "Pasta", "Sushi", "Steak"}},
    {"Countries", {"Japan", "France", "Canada", "Brazil", "Germany"}},
    {"Animals", {"Tiger", "Elephant", "Kangaroo", "Giraffe", "Penguin"}},
    {"Sports", {"Football", "Basketball", "Tennis", "Baseball", "Cricket"}},
    {"Movies", {"Titanic", "Inception", "Avatar", "Gladiator", "Interstellar"}},
    {"Programming", {"Python", "Variable", "Function", "Compiler", "Algorithm"}},
    {"Music", {"Guitar", "Piano", "Violin", "Drums", "Saxophone"}}
};

    int numCategories = sizeof(categories) / sizeof(categories[0]);
    int categoryChoice;

    // Display category selection
    printCentered("Select a Category:",1);
    fflush(stdout);  // Flush output buffer to prevent newline issue  
    int i;
    for (i = 0; i < numCategories; i++) {
        printf("   %d. %s\n", i + 1, categories[i].categoryName);
    }
    printf("\n");

    printCentered("Enter category number: ",0);
    scanf("%d", &categoryChoice);
    getchar();  // Consume newline

    // Validate choice
    if (categoryChoice < 1 || categoryChoice > numCategories) {
        printCentered("Invalid category choice. Returning to menu...",1);
        return;
    }

    // Get random word from selected category
    Category selectedCategory = categories[categoryChoice - 1];
    char* chosenWord = getRandomWord(selectedCategory);

    // Initialize guessing mechanics
    int wordLength = strlen(chosenWord);
    char guessedWord[wordLength + 1];  // Store guessed letters
    int attemptsLeft = 6;  // Max incorrect guesses
    char guessedLetters[26];  // Store guessed letters
    int guessedCount = 0;
    
    // Initialize guessedWord with underscores
    for (i = 0; i < wordLength; i++) {
        guessedWord[i] = (chosenWord[i] == ' ') ? ' ' : '_';  // Keep spaces visible
    }
    guessedWord[wordLength] = '\0'; // Null-terminate

    // Start Hangman game loop
    while (attemptsLeft > 0) {
        system("cls");
        printHangman(attemptsLeft);  // Show hangman graphic

        // Display category name
        printf("Category: ");
        printf(selectedCategory.categoryName);
        printf("\n\n");

        // Display guessed word progress
        printf("\nWord: ");
        for (i = 0; i < wordLength; i++) {
            printf("%c ", guessedWord[i]);
        }
        printf("\n\n");

        // Display guessed letters
        printf("Guessed Letters: ");
        for (i = 0; i < guessedCount; i++) {
            printf("%c ", guessedLetters[i]);
        }
        printf("\n");

        // Ask for player input
        printf("\nEnter a letter: ");
        char guess;
        scanf(" %c", &guess);
        getchar();  // Consume newline

        // Check if letter was already guessed
        int alreadyGuessed = 0;
        for (i = 0; i < guessedCount; i++) {
            if (guessedLetters[i] == guess) {
                alreadyGuessed = 1;
                break;
            }
        }

        if (alreadyGuessed) {
            printCentered("You already guessed that letter. Try again.",1);
            continue;
        }

        guessedLetters[guessedCount++] = guess;  // Store guessed letter

        // Check if letter is in word
        int correctGuess = 0;
        for (i = 0; i < wordLength; i++) {
            if (chosenWord[i] == guess) {
                guessedWord[i] = guess;
                correctGuess = 1;
            }
        }

        // If guess was incorrect, reduce attempts
        if (!correctGuess) {
            attemptsLeft--;
        }

        // Check if word is fully guessed
        if (strcmp(guessedWord, chosenWord) == 0) {
            system("cls");
            printCentered("Congratulations! You guessed the word! ",1);

            // Score calculation
            int score = attemptsLeft * 10;
            printf("\nYou scored: %d points!\n", score);
            printf("The word was: %s\n", chosenWord);
            
            free(chosenWord);  // Free allocated memory
            waitForKeypress();
            return;
        }
    }

    // If user runs out of attempts
    system("cls");
    printHangman(0);
    printf("\nThe correct word was: %s\n", chosenWord);
    
    free(chosenWord);  // Free allocated memory
    waitForKeypress();
}

// Function to get a random word from a category
char* getRandomWord(Category selectedCategory) {
    srand(time(NULL)); // Seed random number generator
    int randomIndex = rand() % MAX_WORDS;

    // Allocate memory for the chosen word
    char* chosenWord = (char*)malloc(20 * sizeof(char));  
    if (chosenWord == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);  // Exit program if memory allocation fails
    }

    strcpy(chosenWord, selectedCategory.words[randomIndex]);  // Copy word
    return chosenWord;  // Return dynamically allocated memory
}
void printHangman(int attemptsLeft) {
    switch (attemptsLeft) {
        case 6:
            printf("\n    +--------+\n    |/       |\n    |        |\n    |\n    |\n    |\n    |\n    |\n==============\n");
            printf("You have all 6 chances left. Let's go!\n");
            break;
        case 5:
            printf("\n    +--------+\n    |/       |\n    |      (•_•)\n    |        \n    |\n    |\n    |\n    |\n==============\n");
            printf("Uh oh! One mistake. Stay focused!\n");
            break;
        case 4:
            printf("\n    +--------+\n    |/       |\n    |      (•_•)\n    |        ||\n    |        ||\n    |\n    |\n    |\n==============\n");
            printf("Keep going! You can still make it!\n");
            break;
        case 3:
            printf("\n    +--------+\n    |/       |\n    |      (•_•)\n    |       /||\n    |      / ||\n    |\n    |\n    |\n==============\n");
            printf("Don't give up! You still have time!\n");
            break;
        case 2:
            printf("\n    +--------+\n    |/       |\n    |      (•_•)\n    |       /||\\\n    |      / || \\\n    |\n    |\n    |\n==============\n");
            printf("Uh oh! Just two chances left! Think carefully!\n");
            break;
        case 1:
            printf("\n    +--------+\n    |/       |\n    |      (•_•)\n    |       /||\\\n    |      / || \\\n    |       /\n    |      /\n    |\n==============\n");
            printf("Last chance! Believe in yourself!\n");
            break;
        case 0:
            printf("\n    +--------+\n    |/       |\n    |      (x_x)\n    |       /||\\\n    |      / || \\\n    |       / \\\n    |      /   \\\n    |\n==============\n");
            printf("Oh no! Game Over. Better luck next time!\n");
            break;
    }
}

void waitForKeypress() {
    printf("\nPress Enter to return to the main menu...");
    getchar(); // Wait for user input
}
// Function to view the scoreboard (Placeholder)
void viewScoreboard() {
    printf("\n");
    printCentered("[Scoreboard not implemented yet.]",1);
}

