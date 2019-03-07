//
// Created by Shinan on 2019/3/6.
//

#include <ctime>
#include <vector>
#include <thread>
#include "KV.h"
#include <unistd.h>
//根据时间戳排序
bool cmp_by_value(const PairKTime& lhs, const PairKTime& rhs) {
  return lhs.second < rhs.second;
}

KV::KV()
{
  pthread_rwlock_init(&rwlock, NULL);
  //创建超时检测线程
  std::thread threadObj(&KV::innerThreadEntry, shared_from_this());
  threadObj.detach();
  innerThread_ = std::move(threadObj);
}

void KV::initInnerThread()
{
  //创建超时检测线程
  std::thread threadObj(&KV::innerThreadEntry, shared_from_this());
  threadObj.detach();
  innerThread_ = std::move(threadObj);
}

KV::~KV()
{
  pthread_rwlock_destroy(&rwlock);
}

V KV::get(K k)
{
  AutoLockRead autoLockRead(rwlock);
  auto it = mapStr_.find(k);
  if (it == mapStr_.end())
    return "";
  return it->second;
}

void KV::set(K k, V v)
{
  AutoLockWrite autoLockWrite(rwlock);
  auto it = mapStr_.find(k);
  //覆盖V值并更新时间戳,返回
  if (it != mapStr_.end())
  {
    it->second = v;
    _updateKTimeStamp(k);
    return;
  }
  //如果没有此k且已有N条记录，删除最早的数据
  if (mapStr_.size() == N)
  {
    _eraseEarliest();
  }
  mapStr_[k] = v;
  _updateKTimeStamp(k);
}

void KV::_eraseEarliest()
{
  AutoLockWrite autoLockWrite(rwlock);
  //根据时间戳排序,删除最早的key
  vector<PairKTime> vecPairKTime(mapKTimeStamp_.begin(), mapKTimeStamp_.end());
  sort(vecPairKTime.begin(), vecPairKTime.end(), cmp_by_value);
  K k = vecPairKTime.front().first;
  mapStr_.erase(k);
  mapKTimeStamp_.erase(k);
}

std::time_t KV::_getTimeStamp()
{
  std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds>
      tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
  auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
  std::time_t timestamp = tmp.count();
  return timestamp;
}

void KV::_updateKTimeStamp(K k)
{
  AutoLockWrite autoLockWrite(rwlock);
  mapKTimeStamp_[k] = _getTimeStamp();
}

void KV::innerThreadEntry(weak_ptr<KV> wpKV)
{
  while (true)
  {
    //删除超时的KV
    sleep(T);
    auto spKV = wpKV.lock();
    if (spKV)
    {
      spKV->_eraseExpired();
    }
    else
    {
      break;
    }
  }
}

void KV::_eraseExpired()
{
  AutoLockWrite autoLockWrite(rwlock);
  //删除超时的K
  auto it = mapKTimeStamp_.begin();
  for (; it != mapKTimeStamp_.end();)
  {
    auto lastTime = it->second;
    if (_getTimeStamp() - lastTime > T)
    {
      auto k = it->first;
      mapStr_.erase(k);
      it = mapKTimeStamp_.erase(it);
    }
    else
    {
      it++;
    }
  }
}

int main()
{
  shared_ptr<KV> spKV(new KV);
  //spKV->initInnerThread();
  spKV->set("abc", "fff");
  auto str = spKV->get("abc");
  cout << str << endl;
}