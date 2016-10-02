#include <iostream>

void main()
{
	List<float> list;
	list.pushback(0.5);
	list.pushback(0.7);

	ListNode<float> * iterator = list.head;
	while (iterator)
	{
		std::cout << iterator->item << "\n";
		iterator = iterator->next;
	}
}
