#pragma once
#include "pch.h"

using namespace std;
template<class T>
class ThreadSafeQueue
{
public:
	ThreadSafeQueue(int nMaxSize);
	~ThreadSafeQueue();

	bool PushBack(T item);
	T PopFront();
	bool IsEmpty();
	T Front();
	int Size();
	int GetMaxSize() { return m_nQueueMaxSize; }
	void Clear();

private:
	std::queue<T> _queue;
	T* m_arrQueue;			// 데이터를 저장하는 공간
	int	m_nQueueMaxSize;	// 크기
	int m_nCurSize;
	int m_nBeginMark;
	int m_nEndMark;

	std::mutex _mutex;
	std::condition_variable m_cvQueue;
};

template<class T>
inline ThreadSafeQueue<T>::ThreadSafeQueue(int nMaxSize)
{
	m_arrQueue = new T[nMaxSize];
	m_nQueueMaxSize = nMaxSize;
	Clear();
}

template<class T>
inline ThreadSafeQueue<T>::~ThreadSafeQueue()
{
	m_cvQueue.notify_all();
	delete[] m_arrQueue;
}

template<class T>
bool ThreadSafeQueue<T>::PushBack(T item)
{
	std::unique_lock<mutex> lock(_mutex);
	bool ownLock = m_cvQueue.wait_for(
		lock, chrono::milliseconds(50), [this] { return m_nCurSize < m_nQueueMaxSize; });

	if (ownLock == false) {
		return false;
	}

	if (m_nEndMark == m_nQueueMaxSize) {
		m_nEndMark = 0;
	}
	m_nCurSize++;
	m_arrQueue[m_nEndMark++] = item;

	//printf("size: %d, nBeginMark:%d, nEndMark:%d ", m_nCurSize, m_nBeginMark, m_nEndMark);
	//cout << "front:" << m_arrQueue[m_nBeginMark] << ", ThreadID:" << GetCurrentThreadId() << endl;
	m_cvQueue.notify_one();
	return true;
}

template<class T>
T ThreadSafeQueue<T>::PopFront()
{
	std::unique_lock<mutex> lock(_mutex);
	bool ownLock = m_cvQueue.wait_for(
		lock, chrono::milliseconds(50), [this] { return m_nCurSize > 0; });

	if (ownLock == false) {
		return nullptr;
	}

	T result = m_arrQueue[m_nBeginMark];
	m_nCurSize--;
	m_nBeginMark++;

	//printf("size: %d, nBeginMark:%d, nEndMark:%d ", m_nCurSize, m_nBeginMark, m_nEndMark);
	//cout << "front: " << m_arrQueue[m_nBeginMark] << ", ThreadID" << GetCurrentThreadId() << endl;
	m_cvQueue.notify_one();
	return result;
}

template<class T>
inline bool ThreadSafeQueue<T>::IsEmpty()
{
	std::lock_guard<mutex> lock(_mutex);
	return (m_nCurSize > 0);
}

template<class T>
inline T ThreadSafeQueue<T>::Front()
{
	std::lock_guard<mutex> lock(_mutex);
	if (m_nCurSize <= 0) {
		return NULL;
	}
	if (m_nBeginMark >= m_nQueueMaxSize - 1) {
		m_nBeginMark = 0;
	}
	return m_arrQueue[m_nBeginMark];
}

template<class T>
inline int ThreadSafeQueue<T>::Size()
{
	std::lock_guard<mutex> lock(_mutex);
	return m_nCurSize;
}

template<class T>
void ThreadSafeQueue<T>::Clear()
{
	m_nCurSize = 0;
	m_nBeginMark = 0;
	m_nEndMark = 0;
}
