#if !defined(XPCPROCESS_H_)
#define XPCPROCESS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include <windows.h>
#include "XPCProcessAttrib.h"

class XPCProcess  
{
public:
	void vResume();
	void vTerminate();
	void vWaitForTerminate(DWORD dwWaitTime=INFINITE);
	DWORD dwGetExitCode();
	XPCProcess(XPCProcessAttrib& attrib);
	virtual ~XPCProcess();

private:
	XPCProcessAttrib& procAttrib;
	char * ErrorString(char * sLeader);
};

#endif 
