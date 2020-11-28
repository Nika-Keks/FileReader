#pragma once


typedef struct OUTPARAM_t
{
    unsigned int CAreaH; // client area height
    unsigned int CAreaW; // Client area width
    unsigned int CharH;  // character hight
    unsigned int CharW;  // character wigth

    unsigned int iCurStr; // the index of the string in the text with which to start displaying a piece of text in the client area
    unsigned int iFirstSym; // the index of the character in the string from which to start displaying a piece of text in the client area
    char* curSubStr; // pointer to substring with which to start displaying a piece of text in the client area
    unsigned int nLines; // the total number of lines the text will occupy at the current size of the client area
    unsigned int nColums; // the total number of columns the text will occupy at the current size of the client area
}OUTPARAM;


unsigned int nSymInLine(OUTPARAM op);

unsigned int nLineOnPage(OUTPARAM op);

