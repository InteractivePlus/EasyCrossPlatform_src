#ifndef __FILE_CHARSET_ALIAS__
#define __FILE_CHARSET_ALIAS__.
inline const char* PlatformCharsetAliasing() {
	return
		"646" "\0" "ASCII"
		"ISO8859-1" "\0" "ISO-8859-1" "\0"
		"ISO8859-2" "\0" "ISO-8859-2" "\0"
		"ISO8859-4" "\0" "ISO-8859-4" "\0"
		"ISO8859-5" "\0" "ISO-8859-5" "\0"
		"ISO8859-7" "\0" "ISO-8859-7" "\0"
		"ISO8859-13" "\0" "ISO-8859-13" "\0"
		"ISO8859-15" "\0" "ISO-8859-15" "\0";
}
#endif