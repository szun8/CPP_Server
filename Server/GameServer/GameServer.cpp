#include "pch.h"
#include "CorePch.h"
#include <iostream>
// windows / Linux 환경에서 공용으로 스레드 활용 가능한 헤더
#include <thread> 
#include <atomic>
#include <mutex>	// Lock
#include <windows.h>
#include <future>

// part4 ) 1-11 Future
// 미래 객체, 단발성 조건변수
// 활용도가 많지는 않음, 가볍게~ 33분~

int64 result;

int64 Calculate() {
	int64 sum = 0;
	for (int32 i = 0;i<100'0000; ++i) {
		sum += i;
	}
	result = sum; 
	return sum;
}

void PromiseWorker(std::promise<string>&& promise) {
	promise.set_value("Secret Message");
}


void TaskWorker(std::packaged_task<int64(void)>&& task) {
	task();
}

int main()
{
	// 동기(synchronous) 실행
	int64 sum = Calculate();
	cout << sum << endl;

	thread t1(Calculate);	// 직접
	t1.join();
	// 단기간(짧게)만 사용하는데 직접 쓰레드 하나를 만드는 거창한 업무까지는 필요없을 경우
	
	// 비동기 실행
	// 	   - 무조건 멀티쓰레드인건 아니다! (개념 구분)
	// std::future
	{
		// 1) deferred -> lazy evaluation 지연해서 실행
		// 2) async -> 별도의 쓰레드를 만들어서 실행 
		// 3) deferred | async -> 둘 중 알아서 골라주세요

		// '언젠가' '미래에' 결과를 뱉어줄거야! = future
		std::future<int64> future = std::async(std::launch::async, Calculate);

		// 멤버함수를 호출하고 싶을때, 조금 달라지는 future 문법
		class Knight {
		public:
			int64 GetHp() { return 100; }
		};

		Knight knight;	// 객체는 항상 만들고!
		std::future<int64> future2 = std::async(std::launch::async, &Knight::GetHp, knight); // knight.GetHp();

		// get을 해주기 전에 요청한 함수가 일처리를 했는지 안했는지 잠깐 엿보고 싶을때
		std::future_status status = future.wait_for(1ms);
		if(status == future_status::ready){}	// 완료 되었을시,
		future.wait();	// 무한정 기다리는 함수 => 결과물이 이제서야 필요하다! (get과 동일 업무 수행)
		
		// TODO (결과를 바로 받아볼 필요 없을때)

		int64 sum = future.get(); // 결과물이 이제서야 필요하다!
	}

	// std::promise (미래객체를 만들어주는 또다른 방법)
	{
		// 미래(std::future)에 결과물을 반환해줄거라 약속(std::promise)해줘 (계약서)
		std::promise<string> promise;
		std::future<string> future = promise.get_future();	// future은 메인 쓰레드가 보유

		thread t(PromiseWorker, std::move(promise));		// promise는 t 쓰레드가 보유
		string message = future.get();
		cout << message << endl;
		t.join();
	}
	
	// std::packaged_task
	{
		// 전달하려는 함수의 type과 일치 (input, output)
		std::packaged_task<int64(void)> task(Calculate);
		// promise랑 비슷한데, packaged_task는 다른 쓰레드에서 해당 함수를 호출해주세요의 느낌?
		// 일반 함수에서는 결과를 return으로 받아오는데
		// 이거는 따로 그런게 아니라 결과물 자체를 get으로 받아오는!
		std::future<int64> future = task.get_future();

		std::thread t(TaskWorker, std::move(task));
		int64 sum = future.get();

		t.join();
	}

	// 결론)
	// mutex, condition_variable까지 가지 않고 단순한 애들을 처리할 수 있는
	// 특히나, 한 번 발생하는 이벤트에 유용하다
	// 닭잡는데 소잡는 칼을 쓸 필요없다!

	// 1) async : 원하는 함수를 비동기적으로 실행
	// 2) promise : 결과물을 promise를 통해 future로 받아줌
	// 3) packaged_task : 원하는 함수의 실행 결과를 packaged_task를 통해 future로 받아줌

	return 0;
}
