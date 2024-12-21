#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MATRIX_SIZE 27 // 26 letters + 1 space
#define MAX_FILE 100

// function for character to index
int char_to_index(char c)
{
    // control for space char
    if (c == ' ')
    {
        return 26;
    }

    // control for letters
    if (c >= 'a' && c <= 'z')
    {
        // turning ascii values
        return c - 'a';
    }
    return -1;
}

// calculate biagram frequencies from a file
void calculate_biagram_freq(FILE *file, int matrix[MATRIX_SIZE][MATRIX_SIZE], int *total_biagram)
{
    char prev = 0, current;

    while ((current = fgetc(file)) != EOF)
    {
        // skip newline, tab, and return characters
        if (current == '\n' || current == '\t' || current == '\r')
        {
            continue;
        }

        // check if 'prev' and 'current' are valid biagram chars
        if ((prev >= 'a' && prev <= 'z') || prev == ' ')
        {

            if ((current >= 'a' && current <= 'z') || current == ' ')
            {
                // map characters to matrix indices
                int i = char_to_index(prev);
                int j = char_to_index(current);

                // ignore biagrams with two spaces
                if (!(i == 26 && j == 26))
                {
                    matrix[i][j]++;
                    (*total_biagram)++;
                }
            }
        }

        // update 'prev' only for valid characters
        if ((current >= 'a' && current <= 'z') || current == ' ')
        {
            prev = current;
        }
    }
}

// function for calculate dissimilarity between two biagram matrices
double calculate_dissimilarity(int matrix1[MATRIX_SIZE][MATRIX_SIZE], int total1,
                               int matrix2[MATRIX_SIZE][MATRIX_SIZE], int total2)
{
    double dissimilarity = 0.0;
    double freq1, freq2;

    for (int i = 0; i < MATRIX_SIZE; i++)
    {

        for (int j = 0; j < MATRIX_SIZE; j++)
        {

            // conditions for not returning invalid char
            if (total1 > 0)
            {

                freq1 = (double)matrix1[i][j] / total1;
            }

            if (total2 > 0)
            {

                freq2 = (double)matrix2[i][j] / total2;
            }

            // finding dissimilarity score
            dissimilarity += fabs(freq1 - freq2);
        }
    }
    return dissimilarity;
}

int main()
{

    // initialize matrix and count values for language_x.txt
    int matrix_x[MATRIX_SIZE][MATRIX_SIZE] = {0};
    int total_biagram_x = 0;
    double dissimilarity;

    FILE *file_x = fopen("language_x.txt", "r");

    // calculate biagram frequencies of language_x.txt
    calculate_biagram_freq(file_x, matrix_x, &total_biagram_x);
    fclose(file_x);

    int n = 1;
    char file_name[MAX_FILE];

    while (1)
    {
        // changing name of the txt files
        snprintf(file_name, MAX_FILE, "language_%d.txt", n);

        FILE *file_l = fopen(file_name, "r");

        // if you couldnt file, break
        if (!file_l)
        {
            break;
        }

        // initialize matrix and count values for language_~.txt
        int matrix_l[MATRIX_SIZE][MATRIX_SIZE] = {0};
        int total_biagram_l = 0;

        // calculate biagram frequencies of language_~.txt
        calculate_biagram_freq(file_l, matrix_l, &total_biagram_l);
        fclose(file_l);

        // calculate dissimilarity between language_x.txt and other language_~.txt's
        dissimilarity = calculate_dissimilarity(matrix_x, total_biagram_x, matrix_l, total_biagram_l);
        printf("%f\n", dissimilarity);

        // increasing n value for skip other file
        n++;
    }
    return 0;
}