#include "pch.h"
#include "CorePch.h"
#include <iostream>
// windows / Linux 환경에서 공용으로 스레드 생성 가능한 헤더
#include <thread> 
#include <atomic>

// part4 ) 1-3. Atomic (All-Or-Nothing)
// atom : 원자
// 멀티쓰레드에서 stack은 각기 다른 영역으로 데이터를 차지하고 있음
// 해당 연산을 쓰면, 한 연산에서 atomic변수를 건드릴 때 다른 연산이 지금 연산에 침범하지 않도록 cpu 내에서 막아버리는? 기다리게하는 과정을 처리함

atomic<int32> sum = 0;	// 전역(Heap), 공유데이터

void Add() {
	for (int32 i = 0; i < 100'0000; i++) {
		sum.fetch_add(1);
		// ++sum;
		// 어셈블리어(3줄?) = cpu에서 메모리를 꺼내오고 연산하는 것을 동시에 할 수 없음
		// int32 eax = sum;		// 1. eax = 0
		// eax = eax + 1;		// 2. eax = 1
		// sum = eax;			// 3. sum = 1
	}
}

void Sub() {
	for (int32 i = 0; i < 100'0000; i++) {
		sum.fetch_add(-1);
		// --sum;
		// int32 eax = sum;		// 1. eax = 0
		// eax = eax - 1;		// 2. eax = -1
		// sum = eax;			// 3. sum = -1
	}
}
// 멀티쓰레드 환경에서 두개를 동시에 실행한다면, 어떤 것이 먼저실행되고 나중에 실행될진 모르지만,
// 섞여서 실행 후, 값이 덮어쓰여서 값의 충돌로 오류가 발생 => if 1st tried, sum = 1 or -1
// 공유데이터의 단점 (수정시)

// 동기화 : 공유데이터를 다룰때 순서를 정해줘야하는 경우
// 애초에 1~3번의 단계를 한번에 실행해주면 값의 충돌현상을 없앨 수 있음 (오류제거) = 동기화 기법 사용
// => Atomic 연산 : All-Or-Nothing 다 실행이 되거나 아예 안하거나 둘중 하나의 상황만 존재할 수 있는 경우의 연산, 연산이 근데 느리김함(필요한 경우만, 병목현상 발생가능)

int main()
{
	Add();
	Sub();
	cout << sum << endl;	// 0

	std::thread t1(Add);
	std::thread t2(Sub);
	t1.join();
	t2.join();

	cout << sum << endl;	// (?) 엉뚱한 숫자 -> atomic -> 0 (!)

	return 0;
}
