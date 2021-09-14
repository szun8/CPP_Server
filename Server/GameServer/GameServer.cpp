#include "pch.h"
#include "CorePch.h"
#include <iostream>
// windows / Linux 환경에서 공용으로 스레드 활용 가능한 헤더
#include <thread> 
#include <atomic>
#include <mutex>	// Lock
#include "AccountManager.h"
#include "UserManager.h"

// part4 ) 1-5. DeadLock 교착상태
// lock을 해주고 unlock을 해주지 않은 상태
// lock_guard<mutex> 로 해결 가능, 그러나 모든 교착상태를 해결해주진 못함

void Func1() {
	for (int32 i = 0; i < 1000; ++i) {
		UserManager::Instance()->ProcessSave();
	}
}

void Func2() {
	for (int32 i = 0; i < 1000; ++i) {
		AccountManager::Instance()->ProcessLogin();
	}
}

int main()
{
	std::thread t1(Func1);
	std::thread t2(Func2);

	t1.join();
	t2.join();

	cout << "Jobs Done" << endl;

	mutex m1;
	mutex m2;
	std::lock(m1, m2);
	// 2개 이상 lock 가능(std 표준), 순서를 바꿔도 안바꾼 것과 다름없이 일정 규칙으로 동작

	lock_guard<mutex> g1(m1, std::adopt_lock);
	lock_guard<mutex> g3(m2, std::adopt_lock);
	// adopt_lock(option) : 해당 mutex는 이미 lock 했으니 나중에 소멸할 때 (unlock)풀어주기만해
	return 0;
}
