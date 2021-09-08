#include "pch.h"
#include "CorePch.h"
#include <iostream>

#include <thread> // windows / Linux 환경에서 공용으로 스레드 생성 가능한 헤더

// Entry Point : 실행하자마자 thread에 바로 실행시켜주는 (메인)함수를 지정가능
void HelloThread() {
	cout << "Hello Thread!" << endl;
}

void HelloThread_2(int32 num) {
	cout << num << endl;
	// 출력이 뒤죽박죽 -> 순서를 보장해서 출력하는게 아닌, 병렬처리 구조이기에 어떤 thread가 먼저 처리될지 모름
}
int main()
{
	// System Call (OS 커널 요청)
	// cout << "Hello World" << endl; => 꽤나 가벼운 작업은 아님
	// thread 생성도 마찬가지로 OS 요청으로 처리/생성되기에 무거운 작업

	// HelloThread(); => main thread 에서 실행
	std::thread t;	// 생성만 된 상태 => id == 0
	auto id1 = t.get_id();

	t = std::thread(HelloThread);	// 여기서부터 t thread 실행
	auto id2 = t.get_id();		// 쓰레드마다 ID(구분)

	std::thread t2(HelloThread_2, 10);

	vector<std::thread> v;
	for (int32 i = 0; i < 10; ++i) {
		v.push_back(std::thread(HelloThread_2, i));
	}
	for (int32 i = 0; i < 10; ++i) {
		if (v[i].joinable())
			v[i].join();
	}
	
	int32 count = t.hardware_concurrency(); // CPU 코어 개수?
	// t.detach();= std::thread 객체(t)에서 실제 쓰레드를 분리 / main thread랑 t thread랑 떼어주는?분리시키는? 함수 => 단, 그렇게되면 t thread의 정보추출은 불가해짐
	
	cout << "Hello Main" << endl;
	// Q. t thread보다 main thread가 먼저 종료되면 error
	if (t.joinable()) { // 해당 쓰레드 객체가 실행되고 있는지 아닌지 확인 (id != 0 ?)
		t.join();		// A. 끝까지 책임져주는 함수 join() : 지정해준 entry point 함수가 끝나고 main thread를 종료하게 함
	}					// 그래서 해당 쓰레드가 실행되고 있으면 끝까지 책임져주는 join을 쓰고 아니면 그냥 끝내도 괜찮은 코드로 설계
						/*_NODISCARD bool joinable() const noexcept {
							return _Thr._Id != 0;
						}*/

	if (t2.joinable())
		t2.join();
}
