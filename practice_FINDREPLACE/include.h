#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include "Bass\bass.h"
#include "resource.h"
#include <sstream>
#include <vector>
#include <fstream>
#include <cstring>
#pragma comment(lib, "comctl32")
#pragma warning(disable: 4996)


#define NAMEPLEER				TEXT("AudioPleer")

#define THIS					11000
#define IDC_BTNCLOSE			THIS + 1
#define IDC_COLORTEXTRED		THIS + 2
#define IDC_COLORTEXTGREEN		THIS + 3
#define IDC_COLORTEXTBLUE		THIS + 4
#define IDC_COLORWHITE			THIS + 5
#define IDC_COLORYELLOW 		THIS + 6
#define IDC_COLORBLUEVIOLET		THIS + 7
#define IDC_COLORYELLOW 		THIS + 8
#define IDC_COLORTURQUOISE		THIS + 9
