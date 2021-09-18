# CPP_Server
🌐 part 4 : Game server

목차   
[1. MultiThread Programming](#1-multithread-programming)

---
<H2>1. MultiThread Programming</H2>

- `Thread 생성`   
    : thread 실행은 병렬구조, 순서가 최초에 정해져있는 것이 아니다 어떤 thread가 실행될지 모름
  ```C++
  using int32 = __int32;
  #include <thread> 

  int main(){
    std::thread t1;         // t1 생성
    t1 = std::thread();     // t1 실행
    std::thread t2();       // t2 생성 및 실행
    std::thread t3(Func1);  // t3 생성시, 바로 호출되는 함수 지정 가능 => Entry Point

    auto id1 = t1.get_id(); // thread마다 가지고 있는 고유 ID 리턴, 생성만된 혹은 종료된 thread의 id = 0
    int32 count = t.hardware_concurrency(); // CPU 코어 개수 리턴

    bool joinT1 = t1.joinable(); // 지정 thread가 실행되었는지 학인 (id!=0)
    t1.join();  // 자식 thread보다 main thread(부모)가 먼저 끝나지 않도록 책임져주는 함수
    t2.join();  t3.join();
    t1.detach() // main thread와 t1 thread 분리 => t1에 대한 정보 추출은 이제 불가능 
    
    return 0;
  }
  ```   
- [`Atomic`](https://github.com/szun8/CPP_Server/commit/1fc9bb588be7684bdff754bef5b3147d7d1c73dc)   
    : Lock과 비슷한 멀티쓰레드 환경 속, 전역 변수(Heap)에서 데이터공유의 단점 해결법, All-Or-Nothing   
    
- `Lock`   
    : 하나의 thread 실행중에는 다른 thread가 들어와 방해하는(?) 것을 금지(lock)시켜 다른 thread를 대기시키는 기능   
    ✳ 상호배타적, 공유데이터의 동시접근 불가, 일종의 자물쇠 역할, `mutex`   
    ```C++
    #include <mutex>	// Lock
    
    int main(){
      mutex m1;
      m1.lock();
      m1.unlock;
      
      // 하나씩 lock() & unlock()를 해주는 것을 std 표준으로 해결
      mutex m2;
      std::lock_guard<mutex> lg(m2); // 생성될때 lock, 소멸될때 unlock -> class 개념
      std::unique_lock<mutex> ul(m2, std::defer_loc); // 생성되자마자 lock을 하는게 아니라 lock 실행구간 지정가능 옵션
      ul.Lock();  // 시점 지정
      
      return 0;
    ```
    + `DeadLock` 교착상태 : Lock 시점 순서지정 등으로 해결
    + `context Switching` : thread 변경시, 현재 User Mode, 다음 thread와 Cornul Mode 사이에 정보보유로 발생하는 비용적 문제

    🔅 대기방법 : spinLock, Sleep, Event  
    1) SpinLock : 계속 user mode에서 해당 thread가 실행가능할 때까지(다른 thread가 lock을 반환할 떄까지) 루프를 돌면서 재시도하는 lock 동기화 방법(존버)   
        ✳ `atomic` 사용 : 실행가능한 영역에 thread가 ①들어가고 ②lock을 걸어주는 일을 한번에 실행하기위해 사용
        ```C++
            class SpinLock{
            public:
                void lock(){
                    bool expected = false;  // 현재 _locked 의 값을 예상
                    bool desired = true;    // 원하는 값
                    
                    while(_locked.compare_exchange_strong(expected, desired) == false){
                    // return true : 해당 쓰레드 진입해서 원하는 과정 수행 (while 탈출)
                    // return false : 다른 누가 사용중이므로 접근 불가상태 (계속 진행)
                        expected = false;   // 반복할 때마다 false로 바꿔줘야함
                    }
                }
                void unlock(){
                    _locked.stroe(false);   // _locked = false;
                }
            private:
                atomic<bool> _locked = false;
            };
        ```
    3) Sleep
    4) Event
        - AutoResetEvent
        - ManualResetEvent
       
