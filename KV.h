//
// Created by Shinan on 2019/3/6.
//

#ifndef FOR_INTERVIEW_KV_H
#define FOR_INTERVIEW_KV_H

#define N 40 //最多存储键值对
#define T 200 //超时时间ms

#include <string>
#include <unordered_map>
#include <memory>
#include <chrono>
#include <thread>
#include <pthread.h>
#include <iostream>

using namespace std;

typedef string K;
typedef string V;

typedef pair<string, time_t> PairKTime;

//封装读写锁
class AutoLockRead
{
public:
  explicit AutoLockRead(pthread_rwlock_t &rwlock) : rwlock_(rwlock)
  { pthread_rwlock_rdlock(&rwlock);}
  ~AutoLockRead(){ pthread_rwlock_unlock(&rwlock_);}
private:
  pthread_rwlock_t &rwlock_;
};
class AutoLockWrite
{
public:
  explicit AutoLockWrite(pthread_rwlock_t &rwlock) : rwlock_(rwlock)
  { pthread_rwlock_wrlock(&rwlock);}
  ~AutoLockWrite(){ pthread_rwlock_unlock(&rwlock_);}
private:
  pthread_rwlock_t &rwlock_;
};

class KV : public enable_shared_from_this<KV>
{
public:
  KV();
  virtual ~KV();
  //初始化内部超时检测线程。
  void initInnerThread();

  //线程安全写，如果K值已经存在，则会覆盖V值
  void set(K k, V v);

  //线程安全读，如果不存在则会返回空串
  V get(K k);



private:
  //存储超过N会删除最早的KV
  void _eraseEarliest();

  //数据超过T时间没有被修改，就会被自动删除
  void _eraseExpired();

  //每次写数据则更新时间戳
  void _updateKTimeStamp(K k);

  //获取当前时间戳
  time_t _getTimeStamp();

  //检测超时线程入口
  static void innerThreadEntry(weak_ptr<KV> wpKV);

private:
  //存放KV值
  unordered_map<string, string> mapStr_;

  //存放K及对应时间戳
  unordered_map<string, time_t> mapKTimeStamp_;

  std::thread innerThread_;

  //读写锁提高读写的效率
  pthread_rwlock_t rwlock;
};

#endif //FOR_INTERVIEW_KV_H
