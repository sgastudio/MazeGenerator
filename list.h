#pragma once
#include <iostream>
#include <vector>

using namespace std;

template <class T>
class List {
	vector<T> m_stack;
public:
	void Push(T const& elem);
	void Pop();
	void Clear();
	T First();
	T Last();
	bool Empty();
	int Count();
};

template<class T>
inline void List<T>::Push(T const& elem)
{
	m_stack.insert(0, elem);
	//m_stack.push_back(elem);
}

template<class T>
inline void List<T>::Pop()
{
	if (!m_stack.empty())
		m_stack.pop_back();
}

template<class T>
inline void List<T>::Clear()
{
	m_stack.clear();
}

template<class T>
inline T List<T>::First()
{
	return m_stack.back();
}

template<class T>
inline T List<T>::Last()
{
	return m_stack.begin();
}

template<class T>
inline bool List<T>::Empty()
{
	return m_stack.empty();
}

template<class T>
inline int List<T>::Count()
{
	return m_stack.size();
}
