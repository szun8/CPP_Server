#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"


void AccountManager::ProcessLogin() {
	// 한 매니저에 두개 이상의 lock을 잡은 경우가 다수 발생가능

	// account Lock
	lock_guard<mutex> guard(_mutex);

	// User Lock (간접적)
	User* user = UserManager::Instance()->GetUser(100);
}