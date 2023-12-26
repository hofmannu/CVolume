#include "baseClass.h"

void baseClass::vprintf(const std::string& txtMsg, const bool flagName) const
{
	if (flagVerbose)
	{
		if (flagName) printf("[%s] ", className.c_str());

		printf(txtMsg.c_str());
	}

	return;
}
