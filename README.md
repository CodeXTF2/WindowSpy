# WindowSpy 👀

WindowSpy is a Cobalt Strike Beacon Object File meant for targetted user surveillance. The goal of this project was to trigger surveillance capabilities only on certain targets, e.g. browser login pages, confidential documents, vpn logins etc. The purpose was to increase stealth during user surveillance by preventing detection of repeated use of surveillance capabilities e.g. screenshots. It also saves the red team time in sifting through many pages of user surveillance data, which would be produced if keylogging/screenwatch was running at all times.

# How it works
![image](https://user-images.githubusercontent.com/29991665/205124642-c9d3441e-a8da-4bf2-8430-a2267e476b6c.png)

Each time a beacon checks in, the BOF runs on the target. The BOF comes with a hardcoded list of strings that are common in useful window titles e.g. login, administrator, control panel, vpn etc. You can customize this list and recompile yourself. It enumerates the visible windows and compares the titles to the list of strings, and if any of these are detected, it triggers a local aggressorscript function defined in WindowSpy.cna named spy(). By default, it takes a screenshot. You may customize this function however you want, e.g. keylogging, WireTap, webcam, etc.

The spy() function has 1 argument, $1 being the beacon id of the beacon that triggered it.

# Installation
1. load the WindowSpy.cna script into Cobalt Strike
2. Update your keywords e.g. windowspy_update confidential,secret,password

# Building from source
1. open the WindowSpy.sln solution file in Visual Studio
2. Build for target BOF (x64/x86)

# Usage
1. Leave it to run. It should automatically run on each beacon checkin and trigger accordingly.


I built this because I was bored, and was messing with user surveillance. If there are bugs, open an issue. If there are any issues with the design, feel free to open an issue too.

## Disclaimer
usual disclaimer here, I am not responsible for any crimes against humanity you may commit or nuclear war you may cause using this piece of poorly written code.
