#include "Logger.hpp"

Logger& operator <<(Logger& o, unsigned int x)
{
	if(o.IsEnabled()) {
		o.GetStream() << x;
	}
	return o;
}
Logger& operator <<(Logger& o, int x)
{
	if(o.IsEnabled()) {
		o.GetStream() << x;
	}
	return o;
}
Logger& operator <<(Logger& o, const char* x)
{
	if(o.IsEnabled()) {
		o.GetStream() << x;
	}
	return o;
}
Logger& operator <<(Logger& o, const std::string& x)
{
	if(o.IsEnabled()) {
		o.GetStream() << x;
	}
	return o;
}
Logger& operator <<(Logger& o, float x)
{
	if(o.IsEnabled()) {
		o.GetStream() << x;
	}
	return o;
}
Logger& operator <<(Logger& o, double x)
{
	if(o.IsEnabled()) {
		o.GetStream() << x;
	}
	return o;
}

Logger& operator <<(Logger& o, std::ostream& (*x)(std::ostream&))
{
	if(o.IsEnabled()) {
		x(o.GetStream());
	}
	return o;
}

Logger& operator <<(Logger& o, std::ios& (*x)(std::ios&))
{
	if(o.IsEnabled()) {
		x(o.GetStream());
	}
	return o;
}
Logger& operator <<(Logger& o, std::ios_base& (*x)(std::ios_base&))
{
	if(o.IsEnabled()) {
		x(o.GetStream());
	}
	return o;
}


Logger::Logger() : mStream(std::cerr), mEnabled(true)
{
}
