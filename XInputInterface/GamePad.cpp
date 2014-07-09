#include "GamePad.h"

DWORD XInputGamePadGetState(DWORD dwUserIndex, XINPUT_STATE *pState)
{
    return XInputGetState(dwUserIndex, pState);
}

void XInputGamePadSetState(DWORD dwUserIndex, float leftMotor, float rightMotor)
{
    XINPUT_VIBRATION vibration = { (int)(leftMotor * 65535), (int)(rightMotor * 65535) };
    XInputSetState(dwUserIndex, &vibration);
}

DWORD XInputGamePadGetStateHack(DWORD dwUserIndex, XINPUT_STATE *pState)
{
    //First create an HINSTANCE of the xinput1_3.dll.
    HINSTANCE hGetProcIDDLL = LoadLibrary("C:/Windows/System32/xinput1_3.dll");
    //HINSTANCE hGetProcIDDLL = LoadLibrary(XINPUT_DLL);

    //Get the address of ordinal 100.
    FARPROC lpfnGetProcessID = GetProcAddress(HMODULE(hGetProcIDDLL), (LPCSTR)100);

    //typedef the function. It takes an int and a pointer to a XINPUT_STATE and returns an error code
    //as an int.  it's 0 for no error and 1167 for "controller not present".  presumably there are others
    //but I never saw them.  It won't cause a crash on error, it just won't update the data.
    typedef int(__stdcall * pICFUNC)(int, XINPUT_STATE &);
    pICFUNC getControllerState = pICFUNC(lpfnGetProcessID);

    DWORD result = getControllerState(dwUserIndex, *pState);

    FreeLibrary(hGetProcIDDLL);

    return result;
}
