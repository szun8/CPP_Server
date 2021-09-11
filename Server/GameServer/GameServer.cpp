#include "pch.h"
#include "CorePch.h"
#include <iostream>
// windows / Linux 환경에서 공용으로 스레드 활용 가능한 헤더
#include <thread> 
#include <atomic>
#include <mutex>	// Lock

// part4 ) 1-4. Lock
// 공유데이터의 동시접근 [X]
// Mutual Exclusive (상호배타적)
// 사용하는 범위 조심

vector<int32> v;
// 기본적으로 자료구조 컨테이너는 멀티쓰레드에서 동작하지 않는다고 가정

// // [crash가 나는 이유]
// 동적배열 특성상 capacity가 꽉차면 새로 늘려주거나 새로운 메모리 할당 후
// 기존 메모리 복사 - 삭제의 과정을 거침
// 그런데 멀티쓰레드에서 이 과정을 거친다면, 똑같은 과정을 쓰레드수만큼 불규칙적이게 실행(할당-복사-삭제)함
// double-free 현상이 발생

// 그렇다면 충분히 메모리를 우선 할당(reserve)후 push를 해준다면 해결되지 않을까?
// 불규칙하게 push를 실행하기 때문에 동일 메모리에 중복 접근 실행가능 => 조금의 메모리 손실(원한만큼의 size가 도출이 안되는 현상)

// 규칙적으로 원활히 코드 실행을 할 수는 없을까?
// 일단 atomic은 사용 불가 (container의 기능은 못쓰고 atomic의 기능만 사용가능) 
// (!) Lock 사용 : 한 과정을 하는 동안 다른 쓰레드는 해당 데이터에 침입이 불가하게 [Lock]을 걸어주는 규칙을 만들어주는 것

mutex m;	// 일종의 자물쇠, 재귀적(이중적) 호출 불가 => 다른 버전 사용, unlock()을 안해주면 안됨,,,

// RAII (Resource Acquisition Is Initialization) : 하나하나 Lock/unLock을 해주는 불편을 해결
template<typename T>
class LockGuard {	// [Lock 자동문]
public:
	LockGuard(T& m) {
		_mutex = &m;
		_mutex->lock();
	}
	
	~LockGuard(T& m) {
		_mutex->unlock();
	}

private:
	T* _mutex;
};
void Push() {
	for (int32 i = 0; i < 10000; ++i) {
		// m.lock();			// 잠기
		
		// LockGuard<std::mutex> lockGuard(m);	= 객체의 유효범위가 끝나면 break를 했던 뭐든 소멸이되기에 unlock을 자동으로 실행시켜줌
		std::lock_guard<std::mutex> lockGuard(m);	// std 표준도 존재
		std::unique_lock<std::mutex> uniqueLock(m, std::defer_lock);// lockGuard랑 기능은 동일, 추가적으로 옵션지정 가능
		//defer_lock : 일단 lock에 대한 interface만 제공하고(바로 잠그지 않고) 명시적으로 lock을 걸어주면 그때 잠기는 옵션
		uniqueLock.lock();		// 잠기는 시점 지정

		v.push_back(i);
		if (i == 5000) break;	// unlock을 하지 않고 해당 쓰레드가 잠기는 현상 발생 => class LockGuard 해결

		// m.unlock();			// 풀기 => 싱글쓰레드처럼 동작(느릴수있음)
	}
}

int main()
{
	std::thread t1(Push);
	std::thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;
	return 0;
}
