#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variable

typedef struct{
    int x; 
    int y; 
    int label; 
} Coordinate; 


void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
Coordinate* findOne(int** answer, int rowSizes); 
Coordinate* navigator(int startX, int startY, int index, int repeatIndex, int size, int** answer, int* rowSizes, Coordinate* allOnes, int wordLength, Coordinate* pathway, bool restart, int** copyAnswer, Coordinate* repeats, int otherPath); 
int** copyElement(int** a); 
int getFirstDigit(int n); 
int bSize;

// Main function, DO NOT MODIFY 	
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);            
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    
    return 0;
}

void printPuzzle(char** arr) {
 for(int i = 0; i < bSize; i++){
        for(int j = 0; j < bSize; j++){
            printf("%c ", *(*(arr + i) + j));
        }
        printf("\n");
    }
}

char* toUpperCase(char* word, int wordLength) {
    char* upperCaseWord = (char*)malloc((wordLength + 1) * sizeof(char)); // Allocate memory for the new uppercase word
    for (int i = 0; i < wordLength; i++) {
        if ((*(word + i)) >= 'a' && (*(word + i)) <= 'z') {
            *(upperCaseWord + i) = (*(word + i)) - ('a' - 'A');
        } else {
            *(upperCaseWord + i) = *(word + i); 
        }
    }
    *(upperCaseWord + wordLength) = '\0'; 
    
    return upperCaseWord;
}



bool isNumberDifferenceOne(Coordinate* pathway, int number1, int number2, int wordLength, int* diffDigit, int size) {
    *diffDigit = 0; 
    int tempNumber1 = number1; 
    int *repeatValues = (int*)malloc(wordLength * sizeof(int)); 
    int i = 0; 
    
    //int number 1 = *(*(copyAnswer + newX) + newY)
    //int number 2 = *(*(copyAnswer + currentX) + currentY)
    while (tempNumber1 > 0) {
        int digit1 = tempNumber1 % 10; 
        tempNumber1 /= 10; 
        int tempNumber2 = number2; //Current 
        
        // Iterate over each digit of the second number
        while (tempNumber2 > 0) {
            int digit2 = tempNumber2 % 10; 
            tempNumber2 /= 10;
            int diff = digit1 - digit2; 
            if (diff == 1 && digit1 != (*(pathway + size)).label) {  
                (*(repeatValues + i)) = digit1; 
                i++; 
            }
        }
    }

    for(int k = 0; k < wordLength; k++){
        if(((*(pathway + size)).label) + 1 == *(repeatValues + k)){
            *diffDigit = *(repeatValues + k); 
            memset(repeatValues, 0, wordLength * sizeof(Coordinate)); //resets repeatValue 
            return true; 
        }
    }
    return false; 
}



bool isRepeat(Coordinate* pathway, int number1, int wordLength, int newX, int newY, int* diffDigit, int size) {
    int tempNumber1 = number1; 
    int *repeatValues = (int*)malloc(wordLength * sizeof(int)); 
    int i = 0; 

    // Iterate over each digit of the first number
    while (tempNumber1 > 0) {
        int digit1 = tempNumber1 % 10; 
        tempNumber1 /= 10; 
        int diff = digit1 - (*(pathway + (size - 1))).label; 
        if (diff == 1 && digit1 == (*(pathway + size)).label) {
            *(repeatValues + i) = digit1; 
            i++; 
            }
        }
    
   

    bool r = false;
    for (int i = 0; i <= size; i++) {
        if (((*(pathway + i)).x == newX && (*(pathway + i)).y == newY)) {
            r = true;
            break;
        }
    }

    if(!r){
        for(int k = 0; k < wordLength; k++){
        if ((((*(pathway + size)).label) == *(repeatValues + k))){
                *diffDigit = *(repeatValues + k); //
                memset(repeatValues, 0, wordLength * sizeof(Coordinate)); 
                return true; 
            }
        }
    }




    return false; // Return false if no digit with difference 1 is found
}


Coordinate* navigator(int startX, int startY, int manyOnes, int repeatIndex, int size, int** answer, int* rowSizes, Coordinate* allOnes, int wordLength, Coordinate* pathway, bool restart, int** copyAnswer, Coordinate* repeats, int otherPath) {
    bool repeat = false;
    bool found = false;
    int repeatX = -1; 
    int repeatY = -1; 
    int newX;
    int newY;
    int currentX = startX;
    int currentY = startY;
    int theAnchor = otherPath; 
    int digitNum = -1; 

    if (size == wordLength - 1) {
        return pathway;
    }


    if (restart) {
        size = 0; 
        theAnchor = 0; //
        (*(pathway + 0)).x = currentX;
        (*(pathway + 0)).y = currentY; 
        (*(pathway + 0)).label = (*(allOnes + manyOnes)).label;
        restart = false; 
        repeatIndex = 0; 
    }    



    if(manyOnes < *(rowSizes + 0) || *(rowSizes + 0) > 0){
        for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {       //Checks for Bounds 
            if (dx == 0 && dy == 0) continue;   //
            newX = currentX + dx;
            newY = currentY + dy;
            if (newX >= 0 && newX < bSize && newY >= 0 && newY < bSize) {    //Checks if the grid is one size above it     
                if(isNumberDifferenceOne(pathway,*(*(copyAnswer + newX) + newY) , *(*(copyAnswer + currentX) + currentY) , wordLength, &digitNum, size)){
                    found = true;
                    if (found) {
                        if (*(*(copyAnswer + newX) + newY) > 9) {
                            size++;
                            (*(pathway + size)).label = digitNum;
                            (*(pathway + size)).x = newX;
                            (*(pathway + size)).y = newY;
                            startX = newX;
                            startY = newY;
                        } else {
                            size++;
                            (*(pathway + size)).x = newX;
                            (*(pathway + size)).y = newY;
                            (*(pathway + size)).label = *(*(copyAnswer + newX) + newY);
                            startX = newX;
                            startY = newY;
                        }
                    }
                        digitNum = 0;
                } 

                else if(isRepeat(pathway,*(*(copyAnswer + newX) + newY), wordLength, newX, newY, &digitNum, size)){
                        repeatX = newX; 
                        repeatY = newY; 
                        theAnchor = (*(pathway + size)).label; 
                        repeat = true;
                        digitNum = 0;                 
                    }
                
                   
                
            }
        }
    }

    if (size == wordLength - 1) {
        return pathway;
    } else {
        if (!found && repeatIndex > 0) { //None found but repeat 
            int newSize = size; 
            int offset = 0; 
             restart = false;
            for(int i = size; i > 0; i--)
            {
                if(theAnchor == (*(pathway + i)).label)
                { 
                    break; 
                }
                (*(pathway + i)).x = 0; 
                (*(pathway + i)).y = 0; 
                (*(pathway + i)).label = 0; 
                newSize--; 
                offset++; 
            }
            size = size - offset; 
            for (int i = 0; i < repeatIndex; i++) 
            {
                if ((((*(repeats + i)).label) - (*(pathway + (newSize - 1))).label) == 1   ) 
                {
                    (*(pathway + newSize)).x = (*(repeats + i)).x;
                    (*(pathway + newSize)).y = (*(repeats + i)).y;
                    (*(pathway + newSize)).label = (*(repeats + i)).label;
            
                    for (int j = i + 1; j < repeatIndex; j++) { 
                        *(repeats + (j - 1)) = *(repeats + j); 
                    }
                }
            } return navigator((*(pathway + newSize)).x,(*(pathway + newSize)).y, manyOnes, repeatIndex, size, answer, rowSizes, allOnes, wordLength, pathway, restart, copyAnswer, repeats, 0); 
        } 
        else if (!found && repeatIndex == 0) //None found and no repeats 
        {  
                free(pathway); 
                pathway = NULL; 
                repeats = NULL; 
                pathway = (Coordinate*)malloc(wordLength * sizeof(Coordinate));
                repeats = (Coordinate*)malloc(wordLength * sizeof(Coordinate));
                memset(pathway, 0, wordLength * sizeof(Coordinate)); 
                memset(repeats, 0, wordLength * sizeof(Coordinate)); 
                manyOnes++; 
                if(manyOnes > *(rowSizes + 0)){
                    return NULL;
                }
                size = 0; 
                restart = true; // 
                startX = (*(allOnes + manyOnes)).x; // 
                startY = (*(allOnes + manyOnes)).y;
                return navigator(startX, startY, manyOnes, repeatIndex, size, answer, rowSizes, allOnes, wordLength, pathway, restart, copyAnswer, repeats, 0);
        }else if (found && !repeat) //If there's a value that's one of the original  
        { 
            restart = false;
            return navigator(startX, startY, manyOnes, repeatIndex, size, answer, rowSizes, allOnes, wordLength, pathway, restart, copyAnswer, repeats, theAnchor);
        }else if(repeat) //if boolean repeat is triggered from isRepeat 
        {
            (*(repeats + repeatIndex)).x = repeatX;
            (*(repeats + repeatIndex)).y = repeatY; 
            (*(repeats + repeatIndex)).label = *(*(answer + repeatX) + repeatY);  //
            repeatIndex++; 
            restart = false;
            return navigator(startX, startY, manyOnes, repeatIndex, size, answer, rowSizes, allOnes, wordLength, pathway, restart, copyAnswer, repeats, theAnchor);
        }
        }
    }
    return NULL;
}



Coordinate* findOne(int** answer, int rowSizes){
    int index = 0;
    Coordinate* allOnes = (Coordinate*)(malloc)(rowSizes * sizeof(Coordinate)); 
    for(int i = 0; i < bSize; i++){
        for(int j = 0; j < bSize; j++){
            if(*(*(answer + i) + j) == 1){
                (*(allOnes + index)).x = i; 
                (*(allOnes + index)).y = j; 
                (*(allOnes + index)).label = 1; 
                index++; 
            }else if(*(*(answer + i) + j) >= 10 && (*(*(answer + i) + j) % 10 == 1 || *(*(answer + i) + j) / 10 == 1)){
                (*(allOnes + index)).x = i; 
                (*(allOnes + index)).y = j; 
                (*(allOnes + index)).label = 1; 
                index++; 
            }
//
        }
    }
    return allOnes; 

}

int addIntegerToMultiDigitNumber(int a, int b) {
    // Calculate the number of digits in 'a'
    int digitsInA = (int)(log10(a)) + 1;

    // Multiply 'b' by a power of 10 corresponding to the number of digits in 'a'
    b *= pow(10, digitsInA);

    // Add 'a' to the result
    b += a;

    return b;
}


int** copyElement(int** a){
    int** b = (int**)malloc(bSize * sizeof(int*));
    for(int i = 0; i < bSize; i++){
        *(b + i) = (int*)malloc(bSize * sizeof(int));
        for(int j = 0; j < bSize; j++){
           *(*(b + i) + j) = *(*(a + i) + j); 
        }
    }

    return b; 
}

void searchPuzzle(char** arr, char* word) {
    //Turns word upperCase
    int **answer = (int**)malloc(bSize * sizeof(int*));
    int wordLength = strlen(word); 
    char* upperCaseWord = toUpperCase(word, wordLength);
    strcpy(word, upperCaseWord);     



    //Makes int 2D Grid of 0s
    for (int i = 0; i < bSize; i++) {
        *(answer + i) = (int*)malloc(bSize * sizeof(int));
        for(int j = 0; j < bSize; j++){
            *(*(answer + i) + j) = 0; 
        }
    }

    //Label the 2D Grid based upon the char of word 
    int coordSize = 0; 
    int* rowSizes = (int*)calloc(wordLength, sizeof(int));
    for(int i = 0; i < bSize; i++){
        for(int j = 0; j < bSize; j++){
            for(int k = 0; k < wordLength; k++){
                if(*(*(arr + i) + j) == *(word + k)){
                    if(*(*(answer + i) + j) == 0){
                        *(*(answer + i) + j) = k + 1; // Increment each character position in word
                        (*(rowSizes + k))++; 
                        coordSize++; 
                    }else{
                        *(*(answer + i) + j) = *(*(answer + i) + j) * 10 + (k + 1);
                        (*(rowSizes + k))++; 
                        coordSize++; 
                    }
                }
            }  
        }
    }


//Create array that keeps track of the amount of ones 
Coordinate* allOnes = findOne(answer, *(rowSizes + 0)); 

//Use to keep track and save char/numbers that are adjacent 
Coordinate* pathway = (Coordinate*)malloc(wordLength * sizeof(Coordinate)); 

// Use to keep track of the amount of repeats 
Coordinate* repeats = (Coordinate*)malloc(wordLength * sizeof(Coordinate)); 


bool restart = true; 

//Copies the int 2D Grid 
int** copyAnswer = copyElement(answer); 


//Result is used to find the adjacent  pathway 
Coordinate* result = navigator((*(allOnes + 0)).x,(*(allOnes + 0)).y,0,0,0, answer, rowSizes, allOnes, wordLength, pathway, restart, copyAnswer, repeats, 0);

for(int i = 0; i < bSize; i++){
    for(int j = 0; j < bSize; j++){
        *(*(answer + i) + j) = 0; 
    }
}

    if(result != NULL){
        printf("\nWord Found!:\nPrinting the Search Path:\n"); 
        for(int i = 0; i < bSize; i++){
            int a = 0; 
            int b = 0; 
            for(int j = 0; j < bSize; j++){
                for(int k = 0; k < wordLength; k++){
                    if((*(result + k)).x == i && (*(result + k)).y == j){
                        if(*(*(answer + i) + j) == 0){
                            *(*(answer + i) + j) = (*(result + k)).label; 
                        }else{
                            a = *(*(answer + i) + j); 
                            b = (*(result + k)).label; 
                            *(*(answer + i) + j) = addIntegerToMultiDigitNumber(a, b);
                        }
                    }
                }
            }

        }

        for(int i = 0; i < bSize; i++){
            for(int j = 0; j < bSize; j++){
                printf("%-7d", *(*(answer + i) + j));
            }
            printf("\n");
        }


    }else{
        printf("\nWord Not Found!"); 
    }
}










