#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"

void UserManager::ProcessSave() {
	// account Lock
	Account* account = AccountManager::Instance()->GetAccount(100);

	// user lock
	lock_guard<mutex> guard(_mutex);

	// 교착상태 해결법
	// - lock을 해주는 순서를 맞추면 해결 가능
	// - mutex에 대한 class를 만들어서 각 객체에 Id 부여 
	//		=> lock을 걸 때마다 추적해서 id가 큰 순부터 lock을 걸어주는 등의 규칙사용

	// TO DO
}