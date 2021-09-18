#include "pch.h"
#include "CorePch.h"
#include <iostream>
// windows / Linux 환경에서 공용으로 스레드 활용 가능한 헤더
#include <thread> 
#include <atomic>
#include <mutex>	// Lock
#include <windows.h>

// part4 ) 1-10 Condition Variable
// Event의 연장선, 조건변수
	
mutex m;
queue<int32> q;
HANDLE handle;	// handle : 어떤 Event인지 구분해주는 식별자

condition_variable cv;

void Producer() {
	while (true) {
		
		// 1) Lock을 잡고
		// 2) 공유변수 값을 수정
		// 3) Lock을 풀고
		// 4) 조건변수 통해 다른 쓰레드에게 통지
		
		// 3번을 안하고 4번을 해도되냐? NO!
		// 쓰레드를 깨워도 먼저하는 것이 lock이기에 여기서 lock을 안풀어주면
		// 깨워진 쓰레드에서 lock을 잡지 못함

		{
			unique_lock<mutex> lock(m);	// 1, 3번
			q.push(100);				// 2번
		}
		cv.notify_one(); // 4번, Wait중인 쓰레드가 있으면 딱 1개를 깨운다
		//::SetEvent(handle);	// 데이터가 넣어지면, 해당 커널오브젝트를 파란불로 바꿔주세요 -> Consumer 실행!
		//this_thread::sleep_for(1000000ms);	// 느리게 데이터 삽입
	}
}

void Consumer() {
	while (true) {
		
		unique_lock<mutex> lock(m);
		cv.wait(lock, []() {return q.empty() == false; });
		// 1) Lock을 잡으려고 시도 (이미 잡혔으면 통과)
		// 2) 조건확인
		//	- 만족 0 => 빠져나와서 이어서 코드를 진행
		
		// 그런데 notify_one 을 했으ㅡ면 항상 조건식을 만족하는거 아닐까?
		// Spurious Wakeup (가짜 기상?)
		// notify_one할 때 lock을 잡고 있는 것이 아니기 때문에 크로스체킹
		// 조건과 notify는 독립적으로 시행하기에 충분히 조건을 걸어줘야함
		// if (q.empty() == false) - 위에서 람다로 조건을 이미 통과시켜준 다음이므로 없어도됨
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}

		//  - 만족 X => Lock을 풀어주고(unique_lock) 대기 상태 전환
	}
}

int main()
{
	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	return 0;
}
