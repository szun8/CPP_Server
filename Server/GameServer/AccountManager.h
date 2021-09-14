#pragma once
#include <mutex>

class Account {
	// TO DO
};
class AccountManager
{
public:
	static AccountManager* Instance() {
		static AccountManager instance;
		return &instance;
	}

	Account* GetAccount(int32 id) {
		lock_guard<mutex> guard(_mutex);
		// ¹º°¡¸¦ °¡Á®¿È
		return nullptr;
	}

	void ProcessLogin();

private:
	mutex _mutex;
	map<int32, Account*> _accounts;
};

