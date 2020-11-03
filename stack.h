#pragma once
#include <iostream>
#include <vector>

using namespace std;

template <class T>
class Stack {
	vector<T> m_stack;
public:
	void Push(T const& elem);
	void Pop();
	void Clear();
	T Top();
	T Bottom();
	bool Empty();
	int Count();
};

template<class T>
inline void Stack<T>::Push(T const& elem)
{
	m_stack.push_back(elem);
}

template<class T>
inline void Stack<T>::Pop()
{
	if(!m_stack.empty())
		m_stack.pop_back();
}

template<class T>
inline void Stack<T>::Clear()
{
	m_stack.clear();
}

template<class T>
inline T Stack<T>::Top()
{
	return m_stack.back();
}

template<class T>
inline T Stack<T>::Bottom()
{
	return m_stack.begin();
}

template<class T>
inline bool Stack<T>::Empty()
{
	return m_stack.empty();
}

template<class T>
inline int Stack<T>::Count()
{
	return m_stack.size();
}
