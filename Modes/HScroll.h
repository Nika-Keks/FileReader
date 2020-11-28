#pragma once
#include"../Text/Text.h"
#include"../OutParams/OutParams.h"
#include"../Scroll/Scroll.h"

void HSOut(HDC hdc, PAINTSTRUCT ps, OUTPARAM op, TEXT text);

void HSProcChangeMode(OUTPARAM* op, TEXT* text);

void HSProcChangeWSize(LPARAM lParam, OUTPARAM* op, TEXT* text);

void HSScrollV(HWND hwnd, WPARAM wParam, OUTPARAM *op);

void HSScrollH(HWND hwnd, WPARAM wParam, OUTPARAM *op);

void HSSetScrollParam(HWND hwnd, OUTPARAM op);
