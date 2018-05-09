#ifndef __FILE_CHARSET_ALIAS__
	#define __FILE_CHARSET_ALIAS__
	inline const char* PlatformCharsetAliasing() {
		return "ISO8859-1" "\0" "ISO-8859-1" "\0"
			"ISO8859-2" "\0" "ISO-8859-2" "\0"
			"ISO8859-5" "\0" "ISO-8859-5" "\0"
			"ISO8859-6" "\0" "ISO-8859-6" "\0"
			"ISO8859-7" "\0" "ISO-8859-7" "\0"
			"ISO8859-8" "\0" "ISO-8859-8" "\0"
			"ISO8859-9" "\0" "ISO-8859-9" "\0"
			"ISO8859-15" "\0" "ISO-8859-15" "\0"
			"IBM-850" "\0" "CP850" "\0"
			"IBM-856" "\0" "CP856" "\0"
			"IBM-921" "\0" "ISO-8859-13" "\0"
			"IBM-922" "\0" "CP922" "\0"
			"IBM-932" "\0" "CP932" "\0"
			"IBM-943" "\0" "CP943" "\0"
			"IBM-1046" "\0" "CP1046" "\0"
			"IBM-1124" "\0" "CP1124" "\0"
			"IBM-1129" "\0" "CP1129" "\0"
			"IBM-1252" "\0" "CP1252" "\0"
			"IBM-eucCN" "\0" "GB2312" "\0"
			"IBM-eucJP" "\0" "EUC-JP" "\0"
			"IBM-eucKR" "\0" "EUC-KR" "\0"
			"IBM-eucTW" "\0" "EUC-TW" "\0"
			"big5" "\0" "BIG5" "\0"
			"GBK" "\0" "GBK" "\0"
			"TIS-620" "\0" "TIS-620" "\0"
			"UTF-8" "\0" "UTF-8" "\0";
	}
#endif