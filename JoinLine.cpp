//
// Created by Shinan on 2019/3/6.
//

#include <iostream>
#include <deque>
#include <vector>
#include <sstream>

using namespace std;
#define N 4

//电线的两端
typedef deque<string> QuePeerLine;
//已经建立起连接的不同电线
typedef vector<deque<string>> VecJoinedLine;

//分割字符串
void split(const string &inStr, deque<string> &queStr, const char c)
{
  istringstream ss(inStr);
  string str;
  while (getline(ss, str, c))
  {
    queStr.emplace_back(str);
  }
}

//如果找到连接端则进行收尾连接，返回true，否则返回false
bool joinLine(const QuePeerLine &quePeerLineIn, QuePeerLine &quePeerLineOut)
{
  if (quePeerLineIn.front() == quePeerLineOut.front())
  {
    quePeerLineOut.emplace_front(quePeerLineIn.front());
    quePeerLineOut.emplace_front(quePeerLineIn.back());
    return true;
  }
  if (quePeerLineIn.front() == quePeerLineOut.back())
  {
    quePeerLineOut.emplace_back(quePeerLineIn.front());
    quePeerLineOut.emplace_back(quePeerLineIn.back());
    return true;
  }
  if (quePeerLineIn.back() == quePeerLineOut.front())
  {
    quePeerLineOut.emplace_front(quePeerLineIn.back());
    quePeerLineOut.emplace_front(quePeerLineIn.front());
    return true;
  }
  if (quePeerLineIn.back() == quePeerLineOut.back())
  {
    quePeerLineOut.emplace_back(quePeerLineIn.back());
    quePeerLineOut.emplace_back(quePeerLineIn.front());
    return true;
  }
  return false;
}

int main()
{
  //输入的一条电线
  QuePeerLine quePeerLine;
  //已经建立起连接的不同电线
  VecJoinedLine vecJoinedLine;

  string str;
  for (int i = 0; i < N; i++)
  {
    //输入一条电线
    quePeerLine.resize(0);
    getline(cin, str);
    split(str, quePeerLine, ' ');
    //输入错误则退出
    if (quePeerLine.size() != 2)
    {
      cout << "please input the right peer data!" << endl;
      return 0;
    }
    bool joined = false;
    //遍历每一条电线, 如果可以建立连接则连接
    for (auto &line : vecJoinedLine)
    {
      if (joinLine(quePeerLine, line))
      {
        joined = true;
        break;
      }
    }
    //没找到可以连接的电线，则插入到电线队列
    if (!joined)
    {
      vecJoinedLine.emplace_back(quePeerLine);
    }
  }
  //输入结束，可以建立连接的电线也已建立结束
  //继续遍历已建立连接的电线,推出一条电线和其他电线建立连接，
  //如果可以连接则继续，直到连接为1条，如果不可以则无法连接为1条电线
  while (vecJoinedLine.size() > 1)
  {
    QuePeerLine peerLine = vecJoinedLine.back();
    vecJoinedLine.pop_back();
    for (auto& leftPeerLine : vecJoinedLine)
    {
      if (!joinLine(peerLine, leftPeerLine))
      {
        cout << "不可以连接为一条电线" << endl;
        return 0;
      }
    }
  }
  std::cout << "可以连接为一条电线" << std::endl;
  return 1;
}