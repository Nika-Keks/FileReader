#include"OutParams.h"

/*
    input
    OUTPARAM op - structure containing output parameters

    output
    int - the number of characters that fit on one line at the current window size
*/
unsigned int nSymInLine(OUTPARAM op)
{
    return op.CAreaW / op.CharW;
}

/*
    input
    OUTPARAM op - structure containing output parameters

    output
    int - the number of lines that fit on one page at the current window size
*/
unsigned int nLineOnPage(OUTPARAM op)
{
    return op.CAreaH / op.CharH;
}
