#pragma once
#include "stdafx.h"

const int TAG_ITEM_FPCHECK = 1;

const COLORREF TAG_GREEN = RGB(0, 190, 0);
const COLORREF TAG_RED = RGB(190, 0, 0);

inline static bool startsWith(const char *pre, const char *str)
{
	size_t lenpre = strlen(pre), lenstr = strlen(str);
	return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
};