#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function prototypes
int romanToDecimal(char *roman); // Converts Roman numeral to decimal
void decimalToWords(int num, char *words); // Converts decimal number to words
void processLine(char *line, FILE *outputFile); // Processes a single line from the input file
int performOperation(int num1, int num2, char operator); // Performs arithmetic operations

int main() {
    FILE *inputFile, *outputFile;
    char line[256]; // Buffer to hold each line from the input file

    // Open input and output files
    inputFile = fopen("input.txt", "r"); // Open input file for reading
    outputFile = fopen("output.txt", "w"); // Open output file for writing

    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening file.\n");
        return 1; // Exit if file cannot be opened
    }

    // Process each line in the input file
    while (fgets(line, sizeof(line), inputFile)) {
        processLine(line, outputFile); // Handle each line and write results to output
    }

    // Close files
    fclose(inputFile);
    fclose(outputFile);

    printf("Processing complete. Check output.txt for results.\n");
    return 0; // Exit successfully
}

// Convert Roman numeral to decimal
int romanToDecimal(char *roman) {
    int values[256] = {0}; // Array to map Roman numeral characters to their values
    values['I'] = 1;
    values['V'] = 5;
    values['X'] = 10;
    values['L'] = 50;
    values['C'] = 100;
    values['D'] = 500;
    values['M'] = 1000;

    int total = 0;
    for (int i = 0; roman[i] != '\0'; i++) { // Iterate through the Roman numeral string
        // Subtract if the current value is less than the next value
        if (values[roman[i]] < values[roman[i + 1]]) {
            total -= values[roman[i]];
        } else { // Otherwise, add the value
            total += values[roman[i]];
        }
    }
    return total; // Return the decimal equivalent
}

// Convert decimal number to words
void decimalToWords(int num, char *words) {
    const char *belowTwenty[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten",
                                 "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    char temp[256] = ""; // Temporary buffer for words
    int chunk, chunkIndex = 0;

    if (num == 0) { // Special case for zero
        strcpy(words, "Zero");
        return;
    }

    while (num > 0) {
        chunk = num % 1000; // Extract the last three digits
        if (chunk > 0) {
            char chunkWords[256] = "";
            // Process hundreds place
            if (chunk / 100 > 0) {
                sprintf(chunkWords, "%s Hundred ", belowTwenty[chunk / 100]);
            }
            chunk %= 100;
            // Process tens and ones place
            if (chunk >= 20) {
                strcat(chunkWords, tens[chunk / 10]);
                chunk %= 10;
                if (chunk > 0) {
                    strcat(chunkWords, " ");
                    strcat(chunkWords, belowTwenty[chunk]);
                }
            } else if (chunk > 0) {
                strcat(chunkWords, belowTwenty[chunk]);
            }

            // Append thousand, million, etc., if applicable
            if (thousands[chunkIndex][0] != '\0') {
                strcat(chunkWords, " ");
                strcat(chunkWords, thousands[chunkIndex]);
            }

            sprintf(temp, "%s %s", chunkWords, temp); // Combine with previous chunks
        }

        num /= 1000; // Move to the next chunk
        chunkIndex++;
    }

    strcpy(words, temp); // Copy result to the output buffer
}

// Process a single line from the input file
void processLine(char *line, FILE *outputFile) {
    char roman1[64], roman2[64], operator;
    int num1, num2, result;
    char resultWords[256];

    // Parse the line (e.g., "MCMXCIV + MMXXIV")
    if (sscanf(line, "%s %c %s", roman1, &operator, roman2) == 3) {
        num1 = romanToDecimal(roman1); // Convert first Roman numeral
        num2 = romanToDecimal(roman2); // Convert second Roman numeral
        result = performOperation(num1, num2, operator); // Perform arithmetic operation

        if (result < 0) { // Handle invalid results
            fprintf(outputFile, "Invalid operation: %s\n", line);
            return;
        }

        decimalToWords(result, resultWords); // Convert result to words
        fprintf(outputFile, "%s %c %s = %d (%s)\n", roman1, operator, roman2, result, resultWords);
    } else {
        fprintf(outputFile, "Invalid format: %s\n", line); // Handle invalid input format
    }
}

// Perform arithmetic operation
int performOperation(int num1, int num2, char operator) {
    switch (operator) {
        case '+': return num1 + num2; // Addition
        case '-': return num1 - num2; // Subtraction
        case '*': return num1 * num2; // Multiplication
        case '/': return (num2 != 0) ? num1 / num2 : -1; // Division (handle divide by zero)
        default: return -1; // Invalid operator
    }
}
