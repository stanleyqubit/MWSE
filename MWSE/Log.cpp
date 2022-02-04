#include "Log.h"

using namespace mwse;
using namespace log;

//debug output stream
template <class CharT, class TraitsT = std::char_traits<CharT> >
class basic_debugbuf :
	public std::basic_stringbuf<CharT, TraitsT>
{
public:
	virtual ~basic_debugbuf()
	{
		sync();
	}

protected:
	int sync()
	{
	}
	void output_debug_string(const CharT *text)
	{
	}
};

template<>
int basic_debugbuf<char>::sync()
{
	OutputDebugStringA(str().c_str());
	return 0;
}

template<class CharT, class TraitsT = std::char_traits<CharT> >
class basic_dostream : 
	public std::basic_ostream<CharT, TraitsT>
{
public:

	basic_dostream() : std::basic_ostream<CharT, TraitsT>
				(new basic_debugbuf<CharT, TraitsT>()) {}
	~basic_dostream() 
	{
//        delete rdbuf(); 
	}
};

typedef basic_dostream<char>	odstream;


namespace mwse
{
	namespace log
	{
		static std::ofstream logstream;
		static odstream debugstream;

		void OpenLog(const char *path)
		{
			logstream.open(path);
		}

		void CloseLog()
		{
			logstream.close();
		}

		std::ostream& getLog()
		{
			return logstream;
		}

		std::ostream& getDebug()
		{
			return debugstream;
		}

		void prettyDump(const void* data, const size_t length) {
			constexpr unsigned int LINE_WIDTH = 16u;

			// Prepare log.
			auto& log = getLog();
			log << std::hex << std::setfill('0');

			unsigned long address = size_t(data);
			const size_t dataEnd = address + length;
			while (address < size_t(data) + length) {
				// Show address
				log << std::setw(8) << address;

				// Show the hex codes
				for (unsigned int offset = 0; offset < LINE_WIDTH; offset++) {
					if (address + offset < dataEnd) {
						log << ' ' << std::setw(2) << unsigned int(*reinterpret_cast<const unsigned char*>(address + offset));
					}
					else {
						log << "   ";
					}
				}

				// Show printable characters
				log << "  ";
				for (unsigned int offset = 0; offset < LINE_WIDTH; offset++) {
					if (address + offset < dataEnd) {
						if (*reinterpret_cast<const unsigned char*>(address + offset) < 32u) log << '.';
						else log << *reinterpret_cast<const char*>(address + offset);
					}
				}

				log << std::endl;
				address += 0x10;
			}
		}
	}
}