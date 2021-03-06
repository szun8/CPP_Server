# CPP_Server
๐ part 4 : Game server

๋ชฉ ์ฐจ   
[1. MultiThread Programming](#1-multithread-programming)

---
<H2>1. MultiThread Programming</H2>

- `Thread ์์ฑ`   
    : thread ์คํ์ ๋ณ๋ ฌ๊ตฌ์กฐ, ์์๊ฐ ์ต์ด์ ์ ํด์ ธ์๋ ๊ฒ์ด ์๋๋ค ์ด๋ค thread๊ฐ ์คํ๋ ์ง ๋ชจ๋ฆ
  ```C++
  using int32 = __int32;
  #include <thread> 

  int main(){
    std::thread t1;         // t1 ์์ฑ
    t1 = std::thread();     // t1 ์คํ
    std::thread t2();       // t2 ์์ฑ ๋ฐ ์คํ
    std::thread t3(Func1);  // t3 ์์ฑ์, ๋ฐ๋ก ํธ์ถ๋๋ ํจ์ ์ง์  ๊ฐ๋ฅ => Entry Point

    auto id1 = t1.get_id(); // thread๋ง๋ค ๊ฐ์ง๊ณ  ์๋ ๊ณ ์  ID ๋ฆฌํด, ์์ฑ๋ง๋ ํน์ ์ข๋ฃ๋ thread์ id = 0
    int32 count = t.hardware_concurrency(); // CPU ์ฝ์ด ๊ฐ์ ๋ฆฌํด

    bool joinT1 = t1.joinable(); // ์ง์  thread๊ฐ ์คํ๋์๋์ง ํ์ธ (id!=0)
    t1.join();  // ์์ thread๋ณด๋ค main thread(๋ถ๋ชจ)๊ฐ ๋จผ์  ๋๋์ง ์๋๋ก ์ฑ์์ ธ์ฃผ๋ ํจ์
    t2.join();  t3.join();
    t1.detach() // main thread์ t1 thread ๋ถ๋ฆฌ => t1์ ๋ํ ์ ๋ณด ์ถ์ถ์ ์ด์  ๋ถ๊ฐ๋ฅ 
    
    return 0;
  }
  ```   
- [`Atomic`](https://github.com/szun8/CPP_Server/commit/1fc9bb588be7684bdff754bef5b3147d7d1c73dc)   
    : Lock๊ณผ ๋น์ทํ ๋ฉํฐ์ฐ๋ ๋ ํ๊ฒฝ ์, ์ ์ญ ๋ณ์(Heap)์์ ๋ฐ์ดํฐ๊ณต์ ์ ๋จ์  ํด๊ฒฐ๋ฒ, All-Or-Nothing   
    
- `Lock`   
    : ํ๋์ thread ์คํ์ค์๋ ๋ค๋ฅธ thread๊ฐ ๋ค์ด์ ๋ฐฉํดํ๋(?) ๊ฒ์ ๊ธ์ง(lock)์์ผ ๋ค๋ฅธ thread๋ฅผ ๋๊ธฐ์ํค๋ ๊ธฐ๋ฅ   
    โณ ์ํธ๋ฐฐํ์ , ๊ณต์ ๋ฐ์ดํฐ์ ๋์์ ๊ทผ ๋ถ๊ฐ, ์ผ์ข์ ์๋ฌผ์  ์ญํ , `mutex`   
    ```C++
    #include <mutex>	// Lock
    
    int main(){
      mutex m1;
      m1.lock();
      m1.unlock;
      
      // ํ๋์ฉ lock() & unlock()๋ฅผ ํด์ฃผ๋ ๊ฒ์ std ํ์ค์ผ๋ก ํด๊ฒฐ
      mutex m2;
      std::lock_guard<mutex> lg(m2); // ์์ฑ๋ ๋ lock, ์๋ฉธ๋ ๋ unlock -> class ๊ฐ๋
      std::unique_lock<mutex> ul(m2, std::defer_loc); // ์์ฑ๋์๋ง์ lock์ ํ๋๊ฒ ์๋๋ผ lock ์คํ๊ตฌ๊ฐ ์ง์ ๊ฐ๋ฅ ์ต์
      ul.Lock();  // ์์  ์ง์ 
      
      return 0;
    ```
    + `DeadLock` ๊ต์ฐฉ์ํ : Lock ์์  ์์์ง์  ๋ฑ์ผ๋ก ํด๊ฒฐ
    + `context Switching` : thread ๋ณ๊ฒฝ์, ํ์ฌ User Mode, ๋ค์ thread์ Cornul Mode ์ฌ์ด์ ์ ๋ณด๋ณด์ ๋ก ๋ฐ์ํ๋ ๋น์ฉ์  ๋ฌธ์ 

    ๐ ๋๊ธฐ๋ฐฉ๋ฒ : spinLock, Sleep, Event  
    1) SpinLock : ๊ณ์ user mode์์ ํด๋น thread๊ฐ ์คํ๊ฐ๋ฅํ  ๋๊น์ง(๋ค๋ฅธ thread๊ฐ lock์ ๋ฐํํ  ๋๊น์ง) ๋ฃจํ๋ฅผ ๋๋ฉด์ ์ฌ์๋ํ๋ lock ๋๊ธฐํ ๋ฐฉ๋ฒ(์กด๋ฒ)   
        โณ `atomic` ์ฌ์ฉ : ์คํ๊ฐ๋ฅํ ์์ญ์ thread๊ฐ โ ๋ค์ด๊ฐ๊ณ  โกlock์ ๊ฑธ์ด์ฃผ๋ ์ผ์ ํ๋ฒ์ ์คํํ๊ธฐ์ํด ์ฌ์ฉ
        ```C++
            class SpinLock{
            public:
                void lock(){
                    bool expected = false;  // ํ์ฌ _locked ์ ๊ฐ์ ์์
                    bool desired = true;    // ์ํ๋ ๊ฐ
                    
                    while(_locked.compare_exchange_strong(expected, desired) == false){
                    // return true : ํด๋น ์ฐ๋ ๋ ์ง์ํด์ ์ํ๋ ๊ณผ์  ์ํ (while ํ์ถ)
                    // return false : ๋ค๋ฅธ ๋๊ฐ ์ฌ์ฉ์ค์ด๋ฏ๋ก ์ ๊ทผ ๋ถ๊ฐ์ํ (๊ณ์ ์งํ)
                        expected = false;   // ๋ฐ๋ณตํ  ๋๋ง๋ค false๋ก ๋ฐ๊ฟ์ค์ผํจ
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
       
