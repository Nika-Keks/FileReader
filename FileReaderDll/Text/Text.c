    #include"Text.h"

/*
    input
    FILE* file - file descriptor that stores text

    output
    TEXT* text - the function allocates memory and fills the fields of the structure containing the text
*/
void ReadText(FILE* file, TEXT* text)
{
    fseek(file, 0, SEEK_END);
    text->NSyms = ftell(file);
    fseek(file, 0, SEEK_SET);
    text->allSyms = (char*)malloc(sizeof(char) * text->NSyms);
    fread(text->allSyms, sizeof(char), text->NSyms, file);

    text->NStr = 0;
    int strLen = 0;
    text->maxStrLen = 0;
    for (int i = 0; i < text->NSyms; i++)
    {
        strLen++;
        if (text->allSyms[i] == '\n')
        {
            text->NStr++;
            if (text->maxStrLen < strLen)
                text->maxStrLen = strLen;
            strLen = 0;
        }
    }
    if (text->allSyms[text->NSyms - 1] != '\n')
    {
        text->NStr++;
    }

    text->strings = (STR*)malloc(sizeof(STR) * text->NStr);
    text->strings[0] = text->allSyms;
    int i_str = 0;
    for (int i = 0; i < text->NSyms; i++)
    {
        if (text->allSyms[i] == '\n' || i == text->NSyms - 1)
        {
            i_str++;
            if (i_str < text->NStr)
                text->strings[i_str] = &(text->allSyms[i + 1]);
        }
    }
}

/*
    input
    TEXT text - structure storing the text
    int i - index of the string, the length of which the function will ferment

    output
    unsigned int - length of string
*/
unsigned int lenStr(TEXT text, int i)
{
    if (i == text.NStr - 1)
        return text.NSyms - (text.strings[i] - text.strings[0]);
    return text.strings[i + 1] - text.strings[i];
}

/*
    input
    TEXT text - structure storing the text
    int iStr - the index of the string containing the substring
    char* substr_ - pointer to substring in string

    output
    unsigned int - number of characters from substr_ to end of string
*/
unsigned int lenSubstr(TEXT text, int iStr, char* substr_)
{
    return lenStr(text, iStr) - (substr_ - text.strings[iStr]);
}

/*
    input
    TEXT text - structure storing the text
    int nSymInLine - the number of characters that fit on the line at the current window size

    output
    unsigned int - the number of lines that fit in the client area at the current window size
*/
unsigned int getNLines(TEXT text, int nSymInLine)
{
    unsigned int NLine = 0;

    for (int i = 0; i < text.NStr; i++)
    {
        NLine += lenStr(text, i) / nSymInLine;
        NLine += (lenStr(text, i) % nSymInLine == 0) ? 0 : 1;
    }
    return NLine;
}

/*
    intput
    TEXT text - structure storing the text
    int nSymInLine - the number of characters that fit on the line at the current window size
    STR substr_ - pointer to the substring where line counting stops

    output
    unsigned int - the number of lines occupied by the part of the text that lies before substr_
*/
unsigned int nLinesBefor(TEXT text, int nSymInLine, STR substr)
{
    unsigned int NLine = 0;

    for (int i = 0; i < text.NStr; i++)
    {
        if (text.strings[i + 1] > substr)
        {
            NLine += (substr - text.strings[i]) / nSymInLine;
            NLine += (substr - text.strings[i]) % nSymInLine == 0 ? 0 : 1;
            break;
        }
        NLine += lenStr(text, i) / nSymInLine;
        NLine += (lenStr(text, i) % nSymInLine == 0) ? 0 : 1;
    }
    return NLine;
}


/*
    function frees memory allocated for text
*/
void freeText(TEXT* text)
{
    free(text->strings);
    free(text->allSyms);
}








