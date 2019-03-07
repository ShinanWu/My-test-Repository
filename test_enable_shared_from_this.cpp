//
// Created by Shinan on 2019/2/25.
//

#include <memory>
#include <iostream>
#include <functional>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <condition_variable>
using namespace std;
using namespace std::placeholders;

std::mutex mtx;
std::condition_variable cv;
std::atomic<bool> bReady{false};

class MainNotifier
{
public:
  MainNotifier()
  {
    cout << "MainNotifier()" << endl;
  }
  void waitCond()
  {
    int i = 0;
    while (i++ < 10)
    {
      std::unique_lock<std::mutex> lck(mtx);
      while (!bReady) cv.wait(lck);
      bReady = false;
      if(!callBack_)
      {
        cout << "error" << endl;
        continue;
      }
      auto cb = std::move(callBack_);
      cb();
    }
  }
  void addCallBack(const function<void()> &cb)
  {
    callBack_ = cb;
  }
  void remvCallBack()
  {
    callBack_ = nullptr;
  }
  function<void()> callBack_;
};

class Notifier{
public:
  Notifier(MainNotifier &mainNotifier) : mainNotifier_(mainNotifier)
  {}
  ~Notifier()
  {
    cout << " ~Notifier" << endl;
  }
  void interLayerCall()
  {
    callBack_();
  }
  void addCallBack(const function<void()> &cb)
  {
    callBack_ = cb;
    mainNotifier_.addCallBack(std::bind(&Notifier::interLayerCall, this));
  }
  function<void()> callBack_;
  MainNotifier &mainNotifier_;
};

class Context : public enable_shared_from_this<Context>
{
public:
  Context(const shared_ptr<Notifier>& spNotifier) : spNotifier_(spNotifier)
  {
    cout << "Context()" << endl;
  };
  ~Context(){cout << "~Context()" << endl;}
  void init()
  {
  }
  void regist()
  {
    cout << "register" << endl;
    f1_ = std::bind(&Context::fun1, shared_from_this());
    //f2_ = std::bind(&Context::fun2, shared_from_this(), _1);
    spNotifier_->addCallBack(f1_);
  }
  void fun1()
  {
    cout << "call fun1" << endl;
    _destory();
    //spNotifier_->addCallBack(f1_);
  }
  void fun2(int i){cout << "call fun2" << endl;}
  void _destory()
  {
    cout << "call _destory" << endl;
    spNotifier_ = nullptr;
    f1_ = nullptr;
    //f2_ = nullptr;
  }


public:
  shared_ptr<Notifier> spNotifier_;
  function<void()> f1_;
  function<void(int)> f2_;
};

class Generate
{
public:
  void creatContext(MainNotifier &mainNotifier)
  {
    auto spNotifier = make_shared<Notifier>(mainNotifier);
    auto spContext = make_shared<Context>(spNotifier);
    spContext->regist();
    cout << spContext.use_count() << endl;
  }
};

void threadEntry(MainNotifier &mainNotifier)
{
  int i = 0;
  int run;
  Generate generate;
  generate.creatContext(mainNotifier);
  cout << "thread start!" << endl;
  while (i++ < 100)
  {
    cin >> run;
    std::unique_lock<std::mutex> lck(mtx);
    bReady = true;
    cv.notify_one();
    cout << "run once!" <<endl;
  }
}

int main()
{
  MainNotifier mainNotifier;
  MainNotifier &ref = mainNotifier;
  std::thread th (threadEntry, std::ref(mainNotifier));
  mainNotifier.waitCond();
  std::this_thread::sleep_for(std::chrono::seconds(10000));
}


//#include <iostream>
//#include <memory>
//#include <string>
//using namespace std;
//
//class a
//{
//public:
//    int m = 1;
//    a(){std::cout << "a" <<std::endl;}
//    a(a&& m){std::cout << "a&&" <<std::endl;}
//
//    virtual int fun(){std::cout << "class a" << std::endl; return 1; }
//};
//
//class b : public a
//{
//public:
//    static int j;
//    int k;
//    int m = 2;
//    int fun(){std::cout << "class b, m" << m << std::endl; return 2;}
//};
//
//void test(a &obj)
//{
//    obj.fun();
//    cout << "debug " << obj.m;
//}
//
//a getret()
//{
//    a m;
//    return m;
//}
//
//#include <functional>
//#include <vector>
////using namespace std;
////using namespace std::placeholders;
////template<typename F, typename V>
////vector<> map(function<F> fun, vector<V> vec)
////{
////    Vector<decltype(F)> vecRet;
////    for(auto it : vec)
////    {
////        vecRet.emplace_back(fun(vec));
////    }
////    return vecRet;
////}
////
////char fun(char c) {
////    if (c >= 'Context') { return c + 32; }
////    return c;
////}
//
//#include <algorithm>
////int main()
////{
////    vector<char> vecChar;
////
////    string str(" Context b C d e f G H I j k ");
////    for(auto it : str)
////    {
////        vecChar.emplace_back(it);
////    }
////    auto vecCharRet = map(std::bind<char(char)>(fun,_1), vecChar);
////
////    for(auto it : vecCharRet)
////        std::cout << it;
////}
//struct PageLayout{
//    struct{
//        int pi;
//        char key[5];
//    };
//    char d[10];
//};
//
//#include <fstream>
//#include <assert.h>
//int main()
//{
//    fstream file("C:\\test_part_0");
//    string str;
//    getline(file, str);
//    cout << str;
//
//
//}
//
//
////int main() {
////
////    a m = getret();
////    cout << m.m;
////    m.fun();
////#include<iostream>
////        int n[][3] = { 10, 20, 30, 40, 50, 60};
////        int (*p)[3];
////        p = n;
////        cout << p[0][0] << "," << *(p[0] + 1) << "," << (*p)[2] << endl;
////        int m[2][3] = {1, 2, 3, 4, 5, 6};
////        int* i = m;
////        cout << typeid(m).name() << "," << typeid(m[0]).name() << "," << typeid(&m[0][0]).name();
//
////    test(b);
////    shared_ptr<a> spa(new a);
////    shared_ptr<b> spb(new b);
////    spa->funa();
////    spb->funb();
////    shared_ptr<void> spvoida = spa;
////    shared_ptr<void> spvoidb = spb;
////    shared_ptr<b> spvoidc = static_pointer_cast<a>(spa);
////    spvoidc->funb();
////    a (*a)[10] = new a[20][10];
////    a[8][9].m = 5;
////    std::cout << a[11][19].m;
//  //  a[0] = new a[10];
//  //  a[1] = new a;
//  //  a (*b)[10][10] = new a[10][10][10];
// //  std::cout << "a: " << a[0]->funa() << endl;
//  //  std::cout << "a: " << b[0][0]->funa() << endl;
////    return 0;
////}
//
