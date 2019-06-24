#include "Config.hpp"

#define VAL(a,b) CVARS[a]=Argument(b)
#define STRVAL(a,b) CVARS[a]=Argument(ARG_STRING,b)
void Config::DefaultValues() {
	VAL("maxfps",121ll);
	VAL("showfps",1ll);
	VAL("console",1ll);
}
