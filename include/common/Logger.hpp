#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <iostream>

class Logger {
public:
	//std::ostream& operator<<(std::ostream& o) { return o; }
	Logger();

	bool IsEnabled() { return mEnabled; }
	void Enable() { mEnabled = true; }
	void Disable() { mEnabled = false; }

	std::ostream& GetStream() { return mStream; }

private:
	std::ostream& mStream;
	bool mEnabled;

};

Logger& operator <<(Logger& o, unsigned int x);
Logger& operator <<(Logger& o, int x);
Logger& operator <<(Logger& o, const char* x);
Logger& operator <<(Logger& o, const std::string& x);
Logger& operator <<(Logger& o, float x);
Logger& operator <<(Logger& o, double x);
Logger& operator <<(Logger& o, void* x);

Logger& operator <<(Logger& o, std::ostream& (*x)(std::ostream&));
Logger& operator <<(Logger& o, std::ios& (*x)(std::ios&));
Logger& operator <<(Logger& o, std::ios_base& (*x)(std::ios_base&));

#endif
