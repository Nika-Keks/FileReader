#pragma once

#include <tchar.h>
#include <windows.h>
#include<stdio.h>
#include<stdlib.h>

typedef char* STR;

typedef struct TEXT_t
{
    STR* strings;   // array of pointers to the place in the text where the new line begins
    char* allSyms;  // array of all characters in the text
    unsigned int NStr;  // number of strings in the text
    unsigned int NSyms; // number of characters in the text
    unsigned int maxStrLen; // length of longest string
}TEXT;

void ReadText(FILE* file, TEXT* text);

unsigned int lenStr(TEXT text, int i);

unsigned int lenSubstr(TEXT text, int iStr, char* substr_);

unsigned int getNLines(TEXT text, int nSymInLine);

unsigned int nLinesBefor(TEXT text, int nSymInLine, STR substr_);

void freeText(TEXT* text);
