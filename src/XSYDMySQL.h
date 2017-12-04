#ifndef __XSYDMySQLFile__
	#define __XSYDMySQLFile__
	#include "EasyCP_Common.h"
	#include "mariadb/mysql.h"
	namespace EasyCrossPlatform {
		namespace Database {
			struct MySQLResult {
				std::vector<std::string> Fields;
				std::vector<std::vector<std::string>> Rows;
			};
			class MySQLManager {
				protected:
					MYSQL* MyConn;
					bool isConnected;
				public:
					MySQLManager();
					~MySQLManager();
					bool Connect(std::string Host, unsigned int Port, std::string Username, std::string Password, std::string Database);
					bool Connect(const char * Host, unsigned int Port, const char * Username, const char * Password, const char* Database);
					std::string EscapeString(std::string PreviousString);
					bool Query(std::string Content);
					bool Query(const char * Content, unsigned int Length);
					MySQLResult FetchResult();
					unsigned long long CheckResultNum(std::string Statement);
					unsigned long long CheckResultNum(const char* Statement);
					int GetLastErrorNo();
					MYSQL* GetConn() const;
			};
		}
	}
#endif