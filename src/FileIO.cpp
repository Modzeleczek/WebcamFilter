#include "../include/FileIO.hpp"

#include <stdio.h>
#include <stdlib.h>

int saveBinary(const char *fileName, const uint8_t *const buffer, const long length)
{
    FILE *file = fopen(fileName, "wb");
    if (file == NULL)
        return -1;
    int ret = 0;
    if (fwrite(buffer, sizeof(uint8_t), length, file) < (size_t)length) // jeżeli nie zapiszemy całego bufora
        ret = -2;
    if (fclose(file) != 0 && ret == 0)
        ret = 1; // błąd niekrytyczny, bo plik został zapisany, ale nie zamknięty
    return ret;
}

int loadBinary(const char *fileName, uint8_t **buffer, long *length)
{
    FILE *file = fopen(fileName, "rb");
    if (file == NULL)
        return -1;
    int ret = 0;
    *length = -1;
    if (fseek(file, 0, SEEK_END) != 0) // przesuwamy kursor na koniec pliku
        ret = -2;
    else if ( (*length = ftell(file)) == -1 ) // odczytujemy pozycję kursora, aby znać rozmiar pliku
        ret = -3;
    else if (fseek(file, 0, SEEK_SET) != 0) // rewind(f)
        ret = -4;
    else
    {
        *buffer = (uint8_t*) malloc(sizeof(uint8_t) * (*length)); // rozmiar pliku (bez null terminatora, bo plik jest binarny, a nie tekstowy)
        if (*buffer == NULL)
            ret = -5;
        else if (fread(*buffer, sizeof(uint8_t), *length, file) < (size_t)*length) // jeżeli nie udało się odczytać całego pliku
        {
            free(buffer);
            ret = -6;
        }
    }
    if (fclose(file) != 0 && ret == 0)
        return 1; // błąd niekrytyczny, bo plik został odczytany, ale nie zamknięty
    return 0;
}

int loadText(const char *fileName, char **buffer)
{
    FILE *file = fopen(fileName, "r");
    if (file == NULL)
        return -1;
    int ret = 0;
    long length = -1;
    if (fseek(file, 0, SEEK_END) != 0) // przesuwamy kursor na koniec pliku
        ret = -2;
    else if ( (length = ftell(file)) == -1 ) // odczytujemy pozycję kursora, aby znać rozmiar pliku
        ret = -3;
    else if (fseek(file, 0, SEEK_SET) != 0) // rewind(f)
        ret = -4;
    else
    {
        *buffer = (char*) malloc(sizeof(char) * (length + 1)); // rozmiar pliku (z null terminatorem, bo plik jest tekstowy)
        if (*buffer == NULL)
            ret = -5;
        else if (fread(*buffer, sizeof(char), length, file) < (size_t)length) // jeżeli nie udało się odczytać całego pliku
        {
            free(buffer);
            ret = -6;
        }
        else
            (*buffer)[length] = '\0'; // na końcu tekstu wstawiamy null terminator
    }
    if (fclose(file) != 0 && ret == 0)
        return 1; // błąd niekrytyczny, bo plik został odczytany, ale nie zamknięty
    return 0;
}