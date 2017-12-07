#include "XSYDMySQL.h"

EasyCrossPlatform::Database::MySQLManager::MySQLManager()
{
	mysql_init(this->MyConn);
	this->isConnected = false;
}

EasyCrossPlatform::Database::MySQLManager::~MySQLManager()
{
	mysql_close(this->MyConn);
	this->isConnected = false;
}

bool EasyCrossPlatform::Database::MySQLManager::Connect(std::string Host, unsigned int Port, std::string Username, std::string Password, std::string Database)
{
	return this->Connect(Host.c_str(),Port,Username.c_str(),Password.c_str(),Database.c_str());
}

bool EasyCrossPlatform::Database::MySQLManager::Connect(const char * Host, unsigned int Port, const char * Username, const char * Password, const char * Database)
{
	if (this->isConnected) {
		return false;
	}
	if (mysql_real_connect(this->MyConn, Host, Username, Password, Database, Port, NULL, 0) == NULL) {
		return false;
	} else {
		this->isConnected = true;
		return true;
	}
}

std::string EasyCrossPlatform::Database::MySQLManager::EscapeString(std::string PreviousString)
{
	if (!this->isConnected) {
		return std::string();
	}
	unsigned int StrLen = PreviousString.length() * 2 + 2;
	char* MyNewStr = new char[StrLen];
	mysql_real_escape_string(this->MyConn, MyNewStr, PreviousString.c_str(), PreviousString.length());
	std::string TempResult = MyNewStr;
	delete[] MyNewStr;
	return TempResult;
}

bool EasyCrossPlatform::Database::MySQLManager::Query(std::string Content)
{
	return this->Query(Content.c_str(), Content.length());
}

bool EasyCrossPlatform::Database::MySQLManager::Query(const char * Content, unsigned int Length)
{
	if (!this->isConnected) {
		return false;
	}
	if (mysql_real_query(this->MyConn, Content, Length)) { //!=0, 0 为 false(成功), 其余为true
		return false;
	}
	return true;
}

EasyCrossPlatform::Database::MySQLResult EasyCrossPlatform::Database::MySQLManager::FetchResult()
{
	EasyCrossPlatform::Database::MySQLResult MyResult;
	if (!this->isConnected) {
		throw std::runtime_error("This MySQL Connection is not connected");
		return MyResult;
	}
	MYSQL_RES* MYRES = mysql_store_result(this->MyConn);
	unsigned int Num_Fields = mysql_num_fields(MYRES);
	if (Num_Fields == 0) {
		return MyResult;
	}
	MYSQL_FIELD* MyTempField;
	std::string MyTempFieldName;
	for (unsigned int i = 0; i < Num_Fields; i++) {
		MyTempField = mysql_fetch_field(MYRES);
		MyTempFieldName = MyTempField->name;
		MyResult.Fields.push_back(MyTempFieldName);
	}
	unsigned int Num_Rows = mysql_num_rows(MYRES);
	MYSQL_ROW MyTempRow;
	std::string MyTempRowFieldValue;
	unsigned long* MyTempRowLengths;
	std::vector<std::string> MyTempRowValue;
	for (unsigned int i = 0;i < Num_Rows; i++) {
		MyTempRow = mysql_fetch_row(MYRES);
		MyTempRowLengths = mysql_fetch_lengths(MYRES);
		MyTempRowValue.clear();
		for (unsigned int j = 0; j < Num_Fields; j++) {
			if (MyTempRow[j] != NULL) {
				MyTempRowFieldValue.assign(MyTempRow[j], MyTempRowLengths[j]);
			} else {
				MyTempRowFieldValue.assign("");
			}
			MyTempRowValue.push_back(MyTempRowFieldValue);
		}
		MyResult.Rows.push_back(MyTempRowValue);
	}
	mysql_free_result(MYRES);
	return MyResult;
}

unsigned long long EasyCrossPlatform::Database::MySQLManager::CheckResultNum(std::string Statement)
{
	return this->CheckResultNum(Statement.c_str());
}

unsigned long long EasyCrossPlatform::Database::MySQLManager::CheckResultNum(const char * Statement)
{
	unsigned long long NumResult;
	if (!this->isConnected) {
		throw std::runtime_error("This MySQL Connection is not connected");
	}
	bool QueryResult = this->Query(Statement);
	if (!QueryResult) {
		return 0;
	}
	MYSQL_RES* MYRES = mysql_store_result(this->MyConn);
	if (mysql_num_fields(MYRES) != 0) {
		NumResult = mysql_num_rows(MYRES);
	}
	else {
		NumResult = 0;
	}
	mysql_free_result(MYRES);
	return NumResult;
}

int EasyCrossPlatform::Database::MySQLManager::GetLastErrorNo()
{
	if (this->isConnected) {
		return mysql_errno(this->MyConn);
	}
	else {
		return 0;
	}
}

MYSQL * EasyCrossPlatform::Database::MySQLManager::GetConn() const
{
	return this->MyConn;
}
