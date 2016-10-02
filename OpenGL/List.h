#pragma once
template<typename T>
class ListNode
{
public:
	T item;
	ListNode<T> * next;
	ListNode(T t) : item(t)
	{
		next = nullptr;
	}
};

template<typename T>
class List
{
public:
	ListNode<T> * head;
	ListNode<T> * last;

public:
	List()
	{
		head = nullptr;
		last = nullptr;
	}
	T pushback(T t)
	{
		if (head)
		{
			last->next = new ListNode<T>(t);
			last = last->next;
		}
		else
		{
			head = last = new ListNode<T>(t);
		}
		return last->item;
	}

	~List()
	{
		while (head)
		{
			ListNode<T> * current = head->next;
			delete head;
			head = current;
		}
	}

};