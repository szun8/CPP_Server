#include "pch.h"
#include "CorePch.h"
#include <iostream>
// windows / Linux 환경에서 공용으로 스레드 활용 가능한 헤더
#include <thread> 
#include <atomic>
#include <mutex>	// Lock
#include <windows.h>

// part4 ) 1-9 Event
	
mutex m;
queue<int32> q;
HANDLE handle;	// handle : 어떤 Event인지 구분해주는 식별자

void Producer() {
	while (true) {
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}
		::SetEvent(handle);	// 데이터가 넣어지면, 해당 커널오브젝트를 파란불로 바꿔주세요 -> Consumer 실행!
		this_thread::sleep_for(1000000ms);	// 느리게 데이터 삽입
	}
}

void Consumer() {
	while (true) {
		::WaitForSingleObject(handle, INFINITE);
		// 파란불이면 아래 코드 실행 / 빨간불이면 수면상태(대기, 아래 코드 실행 X), CPU점유율 절감
		// Auto-Reset(false)에서는 파란불이어서 아래 코드가 실행되면 다시 시그널이 Non-Signal로 바뀐다(자동)
		::ResetEvent(handle);	// Manual-Reset(true)은 수동으로 코드 쳐줘야함

		// Producer에서 data를 안넣어주면 Consumer는 할필요가 없음 (잉여작업)
		unique_lock<mutex> lock(m);
		if (q.empty() == false) {
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}

int main()
{
	// kernel Object (커널에서 관리/사용하는 오브젝트) -> 다른 프로그램 동기화 작업시 사용
	// Usage Count 이 obj를 몇명이 사용중인지
	// Signal(파란불, true) / Non-Signal(빨간불, false) << BOOL
	// Auto / Manual << BOOL
	handle = ::CreateEvent(NULL/*보안속성*/, FALSE/*MANUAL_RESET*/, FALSE/*binitialState*/,NULL );
	
	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	return 0;

	::CloseHandle(handle);	// 생성 소멸 짝꿍의 느낌
}
