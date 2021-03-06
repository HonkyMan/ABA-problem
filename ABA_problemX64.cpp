// ABA_problemX64.cpp: определяет точку входа для консольного приложения.
//
#include <atomic>
#include "stdafx.h"
#include<thread>


union Pointer
{
	unsigned long int ptr;
	unsigned char *bytes;
};

template <typename Obj>
class Stack {
private:
	struct Node
	{
		Node *next;
		Obj data;
	};

	Obj* top_ptr = NULL;
public:
	Stack(): top_ptr(NULL){}
	Stack(Obj data)
	{
		Node abe = { NULL, data };
		top_ptr = abe;
	}
	// Снимает верхний элемент и возвращает указатель на него.
	Obj * ValidPtr(Obj *ptr) {
		return (Obj *)((unsigned __int64)ptr & 0x0000FFFFFFFFFFFF);
	}
	Obj* Pop() {
		while (1) {
			Obj* ret_ptr = top_ptr;
			if (!ret_ptr) return NULL;
			Obj* next_ptr = ValidPtr(ret_ptr)->next;
			// Если верхний элемент - всё ещё ret, считаем, что никто не менял стек.
			// (Это утверждение не всегда истинно из-за проблемы ABA)
			// Атомарно заменяем top на next.

			if (atomic_compare_exchange_weak_explicit(top_ptr, ret_ptr, next_ptr))
			{
				//top_ptr = (int*)((unsigned __int64)top_ptr + 0x000100000000000);
				return ret_ptr;
			}
			else
			{
				top_ptr = ret_ptr;
			}
			// Иначе - стек изменён, пробуем заново.
		}
	}
	//
	// Добавляет obj_ptr на вершину стека.
	//
	void Push(Obj* obj_ptr) {
		while (1) {
			obj_ptr = (Obj *)((unsigned __int64)top_ptr + 0x000100000000000);
			Obj* next_ptr = top_ptr;
			obj_ptr->next = next_ptr;
			//next_ptr = (Obj*)(next_ptr | 0x)
			// Если верхний элемент - всё ещё next, считаем, что никто не менял стек.
			// (Это утверждение не всегда истинно, из-за проблемы ABA)
			// Атомарно заменяем top на obj.
			

			if (atomic_compare_exchange_weak_explicit(top_ptr, next_ptr, obj_ptr))
			{
				return;
			}
			// Иначе - стек изменён, пробуем заново.
		}
	}
};

/*Node *IncrementPointer(Node *ptr)
{
	
	unsigned long int bytes = unsigned long int(ptr);
	
	//bytes |= (1 << 1664);
	/*for (int i = 0; i < 8; i++)
	{
		tt[i] = (*(bytes) >> (16 * i)) & 0xffff;
	}
	return (Node*)bytes;
}*/

int main()
{
	Stack<int> obj(5);

	obj.Push((int*)10);
	obj.Push((int*)7);

	obj.Pop();
	return 0;
}


//int main()
//{
//	int *a = new int[11];
//	a[0] = 0;
//
//	printf("a = %p, sizeof(a) = 5ld\n", a, sizeof(a));
//
//	for (size_t i = 0; i < 18; i++)
//	{
//		a = (int *)((unsigned long long)a + 0x001000000000000);
//	}
//	printf("a = %p, sizeof(a) = 5ld\n", a, sizeof(a));
//	/*Stack<Node> a;
//	Node *node = new Node();
//	node->next = NULL;
//	node->data = 2356;
//	a.Push(node);
//
//	printf("a = %p, sizeof(a) = 5ld\n", node, sizeof(node));
//
//	node = IncrementPointer(node);
//	printf("a = %p, sizeof(a) = 5ld\n", node, sizeof(node));
//
//	printf("a = %p \n", a);*/
//
//	bool f = true;
//	while (f)
//	{
//		//do smth
//	}
//
//    return 0;
//}

