#include <cstring>
#include <iostream>
#include <cstdio>

#ifndef BASECLASS_H
#define BASECLASS_H

/// \brief a simple class to enable verbose and non verbose output for logging
class baseClass
{
public:
	baseClass(std::string name) : className(name) {};

protected:
	bool flagVerbose = true; //!< enable or disable verbose output
	const std::string className; //!< name of the class

	/// \brief a print operator that prints the message to the console if
	/// verbose output is enabled,
	/// \param txtMsg text to print
	/// \param flagName should we add the class name before the output message
	void vprintf(const std::string& txtMsg, const bool flagName = true) const;

};

#endif