#include "pch.h"
#include "CorePch.h"
#include <iostream>
// windows / Linux 환경에서 공용으로 스레드 활용 가능한 헤더
#include <thread> 
#include <atomic>
#include <mutex>	// Lock

// part4 ) 1-7 Spin Lock
// 면접단골질문 

class SpinLock {
public:
	void lock() {
		// 여기서는 아무리 쓸떼없디고 인식된 코드여도 다른 쓰레드가 건드릴 수 있기에?
		// volatile 호출이 필요 => atomic 에서 해결

		// CAS (Compare-And-Swap)
		bool expected = false;
		bool desired = true;

		// CAS (compare_exchange_strong) 의사코드
		//if (_locked == expected) {
		//	expected = _locked;
		//	_locked = desired;
		//	return true;
		//}
		//else {	// expected 값이 false 라면, (다른 누가 이미 소유하는중)
		//	expected = _locked;
		//	return false;
		//}

		// 내가 이길때까지 계속 뺑뺑이를 도는 SpinLock의 개념
		while (_locked.compare_exchange_strong(expected, desired) == false) {
			// 실패했으면 성공할때(true)까지 계속 돌기
			// expected는 bool &값을 받고 있어서 매번 값이 바뀌기에
			// 우리가 원했던 초창기 값으로 바꿔주는 것만 넣어주면 됨 (조심)
			expected = false;
		}

		/*while (_locked) {
			// 두번으로 나뉘어 실행되는 해당 코드를 위에서 한방에 처리
		}
		_locked = true;*/
	}

	void unlock() {
		// _locked = false; bool의 값인지 정확히 알기 어려우니 아래 코드로
		_locked.store(false);
	}
private:
	atomic<bool> _locked = false;	// volatile (C++) 컴파일러에게 최적화를 하지말아달라 부탁 ( + C# 메모리배려, 가시성)
};

mutex m;
int32 sum = 0;
SpinLock spinLock;

void Add() {
	for (int32 i = 0; i < 10'0000; ++i) {
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}

void Sub() {
	for (int32 i = 0; i < 10'0000; ++i) {
		lock_guard<SpinLock> guard(spinLock);
		sum--;
	}
}

int main()
{
	volatile int32 a = 0;
	a = 1;
	a = 2;
	a = 3;	// Release 모드에서 컴파일러 입장 : 왜 쓸떼없게 a = 1~3을 해주고 있냐 => 무시
	a = 4;	// -> volatile 입력시, 무시했었던 1~3 초기화 부분도 컴파일해줌 (최적화 X)
	cout << a << endl;

	std::thread t1(Add);
	std::thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;	// 0

	return 0;
}
