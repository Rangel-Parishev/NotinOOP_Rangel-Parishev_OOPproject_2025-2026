#include "Administrator.h"

Administrator& Administrator::getInstance(const string& username, const string& password)
{
	static Administrator instance(username, password);
	return instance;
}
