#include "baseClass.h"

void baseClass::vprintf(const string txtMsg) const
{
	vprintf(txtMsg, 0);
	return;
}

void baseClass::vprintf(const string txtMsg, const bool flagName) const
{
	if (flagVerbose)
	{
		if (flagName)
			printf("[%s] ", className.c_str());
			
		printf(txtMsg.c_str());
	}
	
	return;
}
