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
Coordinate* navigator(int startX, int startY, int index, int indexC, int size, int** answer, int* rowSizes, Coordinate* allOnes, int wordLength, Coordinate* checked, bool newOne, int** copyAnswer, Coordinate* repeats, int saveFile); 
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
        if (word[i] >= 'a' && word[i] <= 'z') {
            upperCaseWord[i] = word[i] - ('a' - 'A');
        } else {
            upperCaseWord[i] = word[i]; 
        }
    }
    upperCaseWord[wordLength] = '\0'; 
    
    return upperCaseWord;
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


void searchPuzzle(char** arr, char* word) {
    int **answer = (int**)malloc(bSize * sizeof(int*));
    int wordLength = strlen(word); 
    char* upperCaseWord = toUpperCase(word, wordLength);
    strcpy(word, upperCaseWord); 
    // capitalWord(word, wordLength); 
    for (int i = 0; i < bSize; i++) {
        answer[i] = (int*)malloc(bSize * sizeof(int));
        for(int j = 0; j < bSize; j++){
            *(*(answer + i) + j) = 0; 
        }
    }


    int coordSize = 0; 
    int* rowSizes = (int*)calloc(wordLength, sizeof(int));
    for(int i = 0; i < bSize; i++){
        for(int j = 0; j < bSize; j++){
            for(int k = 0; k < wordLength; k++){
                if(arr[i][j] == word[k]){
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

        for(int i = 0; i < bSize; i++){
        for(int j = 0; j < bSize; j++){
            printf("%d ", *(*(answer + i) + j));
        }
            printf("\n");
        }



Coordinate* allOnes = findOne(answer, rowSizes[0]); 


Coordinate* checked = (Coordinate*)malloc(wordLength * sizeof(Coordinate)); 
Coordinate* repeats = (Coordinate*)malloc(wordLength * sizeof(Coordinate)); 

bool newOne = true; 
int** copyAnswer = copyElement(answer); 
Coordinate* result = navigator(allOnes[0].x,allOnes[0].y,0,0,0, answer, rowSizes, allOnes, wordLength, checked, newOne, copyAnswer, repeats, 0);


    for(int i = 0; i < bSize; i++){
        for(int j = 0; j < bSize; j++){
            *(*(answer + i) + j) = 0; 
        }
    }

    if(result != NULL){
        printf("Visited:"); 
        for(int j = 0; j < wordLength; j++){
                printf("Label: %d, ", (*(result + j)).label);
                printf("[x: %d, y: %d] ", (*(result + j)).x, (*(result + j)).y);
        }
        printf("\n"); 
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
        printf("Word Not Found!"); 
    }
}




int** copyElement(int** a){
    int** b = (int**)malloc(bSize * sizeof(int*));
    for(int i = 0; i < bSize; i++){
        b[i] = (int*)malloc(bSize * sizeof(int));
        for(int j = 0; j < bSize; j++){
           *(*(b + i) + j) = *(*(a + i) + j); 
        }
    }

    return b; 
}

bool isDigitDifferenceOne(int digit1, int digit2) {
    return (digit1 - digit2 == 1);
}


// Function to check if any digit of the second number differs by exactly 1 from any digit of the first number
bool isNumberDifferenceOne(Coordinate* checked, int number1, int number2, int wordLength, int newX, int newY, int* diffDigit, int size, bool* repeat, bool* found) {
    *diffDigit = 0; 
    int tempNumber1 = number1; 
    int *repeatValues = (int*)malloc(wordLength * sizeof(int)); 
    int i = 0; 
    
    // Iterate over each digit of the first number
    while (tempNumber1 > 0) {
        int digit1 = tempNumber1 % 10; // Get the current digit of the first number
        tempNumber1 /= 10; // Move to the next digit of the first number
        int tempNumber2 = number2; //Current 
        
        // Iterate over each digit of the second number
        while (tempNumber2 > 0) {
            int digit2 = tempNumber2 % 10; // Get the current digit of the second number
            tempNumber2 /= 10; // Move to the next digit of the second number

            
            // Check if the difference between the current digits is exactly 1
            int diff = digit1 - digit2; 
            if (diff == 1 && digit1 != (*(checked + size)).label) {  //
                (*(repeatValues + i)) = digit1; 
                i++; 
            }
        }
    }

    for(int k = 0; k < wordLength; k++){
        if(((*(checked + size)).label) + 1 == *(repeatValues + k)){
            *diffDigit = *(repeatValues + k); 
            memset(repeatValues, 0, wordLength * sizeof(Coordinate)); 
            return true; 
        }
    }



    return false; // Return false if no digit with difference 1 is found
}



bool isRepeat(Coordinate* checked, int number1, int number2, int wordLength, int newX, int newY, int* diffDigit, int size, bool* repeat, bool* found) {
    int tempNumber1 = number1; 
    int *repeatValues = (int*)malloc(wordLength * sizeof(int)); 
    int i = 0; 

    // Iterate over each digit of the first number
    while (tempNumber1 > 0) {
        int digit1 = tempNumber1 % 10; 
        tempNumber1 /= 10; 
        int diff = digit1 - checked[size - 1].label; 
        printf("Digit1: %d and checked[size - 1].label: %d\n", digit1, checked[size - 1].label);
        printf("diff: %d\n", diff);
        if (diff == 1 && digit1 == (*(checked + size)).label) {
            printf("THE DIGIT1: %d == checked[size].label: %d\n", digit1, (*(checked + size)).label);
            repeatValues[i] = digit1; 
            i++; 
            }
        }
    
    printf("newX:%d and newY: %d\n", newX, newY); 
    printf("checked[size - 1].label: %d checked[size - 1].x:%d and checked[size - 1].y: %d\n", checked[size - 1].label, checked[size - 1].x, checked[size - 1].y);
    printf("checked[size].label: %d checked[size].x:%d and checked[size].y: %d\n", checked[size].label, checked[size].x, checked[size].y);  
    

    bool r = false;
    for (int i = 0; i <= size; i++) {
        if ((checked[i].x == newX && checked[i].y == newY)) {
            r = true;
            break;
        }
    }

    if(!r){
        for(int k = 0; k < wordLength; k++){
        if ((((*(checked + size)).label) == *(repeatValues + k))){
                *diffDigit = *(repeatValues + k); //
                memset(repeatValues, 0, wordLength * sizeof(Coordinate)); 
                return true; 
            }
        }
    }




    return false; // Return false if no digit with difference 1 is found
}


Coordinate* navigator(int startX, int startY, int index, int indexC, int size, int** answer, int* rowSizes, Coordinate* allOnes, int wordLength, Coordinate* checked, bool newOne, int** copyAnswer, Coordinate* repeats, int saveFile) {
    printf("Entering navigator: saveFile=%d, startX=%d, startY=%d, index=%d, indexC=%d, size=%d\n", saveFile, startX, startY, index, indexC, size); 
    bool repeat = false;
    bool found = false;
    int repeatX = -1; 
    int repeatY = -1; 
    int newX;
    int newY;
    int currentX = startX;
    int currentY = startY;
    int theAnchor = saveFile; 
    int digitNum = -1; 

    if (size == wordLength - 1) {
        return checked;
    }


    if (newOne) {
        size = 0; 
        theAnchor = 0; //
        (*(checked + 0)).x = currentX;
        (*(checked + 0)).y = currentY; 
        (*(checked + 0)).label = (*(allOnes + index)).label;
        newOne = false; 
        indexC = 0; 
    }

    for (int k = 0; k < wordLength; k++) { 
                printf("checked[k].label:%d", (*(checked + k)).label); 
                printf("[%d, %d]:\n", (*(checked + k)).x, (*(checked + k)).y);
    }
            
    
    printf("checked[%d].x: %d and checked[%d].y: %d and checked[%d].label: %d\n", size, checked[size].x, size, checked[size].y, size, checked[size].label); 
    printf("RowSizes[0]: %d\n", *(rowSizes + 0)); 
    printf("wordLength: %d\n", wordLength);
    if(index < *(rowSizes + 0) || *(rowSizes + 0) > 0){
        for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;   //
            newX = currentX + dx;
            newY = currentY + dy;
            if (newX >= 0 && newX < bSize && newY >= 0 && newY < bSize) {    //Checks if the grid is one size above it     
                printf("Checking cell (%d, %d): %d\n", newX, newY, *(*(copyAnswer + newX) + newY));
                if(isNumberDifferenceOne(checked,*(*(copyAnswer + newX) + newY) , *(*(copyAnswer + currentX) + currentY) , wordLength, newX, newY, &digitNum, size, &repeat, &found)){
                    printf("Found adjacent cell with correct label (%d, %d) which is %d\n", newX,newY, *(*(copyAnswer + newX) + newY) );
                    printf("Digit: %d\n", digitNum); 
                    found = true;
                    if (found) {
                        if (*(*(copyAnswer + newX) + newY) > 9) {
                            size++;
                            (*(checked + size)).label = digitNum;
                            (*(checked + size)).x = newX;
                            (*(checked + size)).y = newY;
                            startX = newX;
                            startY = newY;
                        } else {
                            size++;
                            (*(checked + size)).x = newX;
                            (*(checked + size)).y = newY;
                            (*(checked + size)).label = *(*(copyAnswer + newX) + newY);
                            startX = newX;
                            startY = newY;
                        }
                    }
                        digitNum = 0; //(*(checked + k))
                        
                        for (int k = 0; k < wordLength; k++) { 
                            printf("size: %d\n", size); 
                            printf("checked[k].label:%d", (*(checked + k)).label); 
                            printf("[%d, %d]:\n", (*(checked + k)).x, (*(checked + k)).y);
                        }
                        
                } 
                else if(isRepeat(checked,*(*(copyAnswer + newX) + newY), *(*(copyAnswer + currentX) + currentY), wordLength, newX, newY, &digitNum, size, &repeat, &found)){
                        repeatX = newX; 
                        repeatY = newY; 
                        theAnchor = (*(checked + size)).label; 
                        printf("Repeated Cell (%d, %d)\n", repeatX, repeatY);
                        printf("checked[size].label: %d and digitNum: %d and Repeated Cell (%d, %d)\n",(*(checked + size)).label, digitNum, repeatX, repeatY);
                        printf("theAnchor: %d\n", theAnchor);
                        repeat = true;
                        digitNum = 0;                 
                    }
                
                   
                
            }
        }
    }
    printf("Exiting navigator\n");

    if (size == wordLength - 1) {
        return checked;
    } else {
        if (!found && indexC > 0) { //None found but repeat 
            int newIndex = size; 
            int offset =0; 
             newOne = false;
            printf("size: %d\n", size); 
            for(int i = size; i > 0; i--){
                printf("i: %d\n", i); 
                printf("theAnchor: %d == (*(checked + i)).label: %d\n", theAnchor, (*(checked + i)).label);
                for (int k = 0; k < wordLength; k++) { 
                    printf("checked[k].label:%d", (*(checked + k)).label); 
                    printf("[%d, %d]:\n", (*(checked + k)).x, (*(checked + k)).y);
                }
                if(theAnchor == (*(checked + i)).label){
                     printf("(*(checked + i)).x: %d \n(*(checked + i)).y: %d \n(*(checked + i)).label: %d \n", (*(checked + i)).x, (*(checked + i)).y,(*(checked + i)).label); 
                    break; 
                }
                printf("(*(checked + i)).x: %d \n(*(checked + i)).y: %d \n(*(checked + i)).label: %d \n", (*(checked + i)).x, (*(checked + i)).y,(*(checked + i)).label); 

                (*(checked + i)).x = 0; 
                (*(checked + i)).y = 0; 
                (*(checked + i)).label = 0; 
                newIndex--; 
                offset++; 
                printf("newIndex: %d\n", newIndex); 
            }
            size = size - offset; 
            printf(" newChecked[0].x: %d and newChecked[0].y:%d and newChecked[0].label: %d\n", (*(checked + newIndex)).x, (*(checked + newIndex)).y, (*(checked + newIndex)).label);
            printf("IndexC: %d\n", indexC);
            for (int i = 0; i < indexC; i++) 
            {
                printf("(*(checked + (newIndex)).label: %d ---- (*(repeats + %d)).label: %d - (*(checked + (newIndex - 1))).label: %d == 1\n",i, (*(checked + (newIndex))).label, (*(repeats + i)).label, (*(checked + (newIndex - 1))).label);
                if ((((*(repeats + i)).label) - (*(checked + (newIndex - 1))).label) == 1   ) {
                printf("THE repeats[i].x: %d repeats[i].y: %d\n", (*(repeats + i)).x, (*(repeats + i)).y);
                    (*(checked + newIndex)).x = (*(repeats + i)).x;
                    (*(checked + newIndex)).y = (*(repeats + i)).y;
                    (*(checked + newIndex)).label = (*(repeats + i)).label;
                // Remove the value from repeats by shifting the remaining values
                for (int j = i + 1; j < indexC; j++) { // Start from i + 1 instead of i 
                    *(repeats + (j - 1)) = *(repeats + j); // Shift the elements one position to the left
                }
                
                printf("IndexC: %d\n", indexC);
                printf("newChecked[1].x: %d newChecked[1].y: %d\n", checked[newIndex].x, checked[newIndex].y);
                }
            }
               
               return navigator((*(checked + newIndex)).x,(*(checked + newIndex)).y, index, indexC, size, answer, rowSizes, allOnes, wordLength, checked, newOne, copyAnswer, repeats, 0); 
            } else if (!found && indexC == 0) {  //None found and no repeats 
                free(checked); 
                checked = NULL; 
                repeats = NULL; 
                checked = (Coordinate*)malloc(wordLength * sizeof(Coordinate));
                repeats = (Coordinate*)malloc(wordLength * sizeof(Coordinate));
                memset(checked, 0, wordLength * sizeof(Coordinate)); 
                memset(repeats, 0, wordLength * sizeof(Coordinate)); 
                index++; //Goes to the next 1 in the grid
                if(index == rowSizes[0]){
                    printf("Word not Found\n");
                    return NULL;
                }
                size = 0; 
                newOne = true; // resets CopyAnswer and add new addOne to checked
                startX = (*(allOnes + index)).x; // 
                startY = (*(allOnes + index)).y;
                return navigator(startX, startY, index, indexC, size, answer, rowSizes, allOnes, wordLength, checked, newOne, copyAnswer, repeats, 0);
            } else if (found && !repeat) { //If there's a value that's one of the original  
                newOne = false;
                return navigator(startX, startY, index, indexC, size, answer, rowSizes, allOnes, wordLength, checked, newOne, copyAnswer, repeats, theAnchor);
            }else if(repeat){
                (*(repeats + indexC)).x = repeatX; // 
                (*(repeats + indexC)).y = repeatY; 
                (*(repeats + indexC)).label = *(*(answer + repeatX) + repeatY);  //
                indexC++; 
                newOne = false;

                for(int i = 0; i < indexC; i++){
                    printf("repeats[%d], label: %d  x: %d  y:%d\n", i, repeats[i].label, repeats[i].x, repeats[i].y);
                }


                return navigator(startX, startY, index, indexC, size, answer, rowSizes, allOnes, wordLength, checked, newOne, copyAnswer, repeats, theAnchor);
            
            
            
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



