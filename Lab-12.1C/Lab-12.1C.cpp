#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>

void CreateBIN(const char* fname) {
    FILE* fout = NULL;
    errno_t err = fopen_s(&fout, fname, "wb"); 
    if (err != 0 || !fout) {
        printf("Error: could not open file %s for writing.\n", fname);
        return;
    }

    char ch;
    double number;

    do {
        printf("Enter a number: ");
        if (scanf_s("%lf", &number) != 1) { 
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Очищення буфера
            continue;
        }
        fwrite(&number, sizeof(double), 1, fout);

        // Очищення буфера
        while (getchar() != '\n');

        printf("Continue? (y/n): ");
        scanf_s(" %c", &ch, 1); 
    } while (ch == 'y' || ch == 'Y');

    fclose(fout);
}

void PrintBIN(const char* fname) {
    FILE* fin = NULL;
    errno_t err = fopen_s(&fin, fname, "rb"); 
    if (err != 0 || !fin) {
        printf("Error: could not open file %s for reading.\n", fname);
        return;
    }

    double number;
    printf("Contents of %s:\n", fname);
    while (fread(&number, sizeof(double), 1, fin)) {
        printf("%.6lf ", number);
    }
    printf("\n");

    fclose(fin);
}

void ProcessBIN(const char* fname, const char* gname) {
    FILE* fin = NULL;
    errno_t err = fopen_s(&fin, fname, "rb"); 
    if (err != 0 || !fin) {
        printf("Error: could not open input file %s.\n", fname);
        return;
    }

    FILE* fout = NULL;
    err = fopen_s(&fout, gname, "w"); 
    if (err != 0 || !fout) {
        printf("Error: could not open output file %s.\n", gname);
        fclose(fin);
        return;
    }

    double number, maxVal, minVal, sum = 0.0;
    int count = 0;

    // Перевірка на порожній файл
    if (fread(&number, sizeof(double), 1, fin) != 1) {
        printf("Error: input file is empty.\n");
        fclose(fin);
        fclose(fout);
        return;
    }

    // Ініціалізація значень
    maxVal = minVal = number;
    sum += number;
    count++;

   
    while (fread(&number, sizeof(double), 1, fin)) {
        if (number > maxVal) maxVal = number;
        if (number < minVal) minVal = number;
        sum += number;
        count++;
    }

   
    if (count > 0) {
        double avgMaxMin = (maxVal + minVal) / 2.0;
        double avgAll = sum / count;

        fprintf(fout, "Average of max and min: %.2lf\n", avgMaxMin);
        fprintf(fout, "Average of all numbers: %.2lf\n", avgAll);

        printf("Results have been saved to %s\n", gname);
    }

    fclose(fin);
    fclose(fout);
}

int main() {
    char fname[100];
    char gname[100];

    printf("Enter input file name: ");
    scanf_s("%s", fname, (unsigned)_countof(fname));

    CreateBIN(fname);
    PrintBIN(fname);

    printf("Enter output file name: ");
    scanf_s("%s", gname, (unsigned)_countof(gname)); 

    ProcessBIN(fname, gname);

    FILE* fin = NULL;
    errno_t err = fopen_s(&fin, gname, "r"); 
    if (err != 0 || !fin) {
        printf("Error: could not open file %s for reading.\n", gname);
        return 1;
    }

    printf("Contents of %s:\n", gname);
    char line[256];
    while (fgets(line, sizeof(line), fin)) {
        printf("%s", line);
    }

    fclose(fin);
    return 0;
}
