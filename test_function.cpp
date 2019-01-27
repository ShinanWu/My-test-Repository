#include <iostream>
#include <functional>

class A
{
 public:
  void funA(int num)
  {
    std::cout << "funA, num:" << num;
  }
};

class B
{
 public:
  void funB(int num1, int num2)
  {
    std::cout << "funB, num1:" << num1 << " num2:" << num2;
  }
};

using namespace std::placeholders;

static void genericCall(void* pCall)
{
  auto p2Call = static_cast<std::function<void(int)>*> (pCall);
  (*p2Call)(2);
}

int main()
{
  A a;
  B b;
  std::function<void(int)> funA = std::bind (&A::funA, a, _1);
  std::function<void(int)> funB = std::bind (&B::funB, b, 3, _1);

  genericCall(&funA);
  genericCall(&funB);
}