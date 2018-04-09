/*myvector.h*/

//
// myvector class --- a replacement for std::vector based on linked
// nodes of varying-sized array "chunks".  The idea is to avoid the
// reallocation of the underlying dynamic array (and the copying of 
// elements) when the vector becomes full.
//
// Ji Xiao
// Bachelor of Science in Computer Science
// U. of Illinois, Chicago
// College of Engineering, Department of Computer Science
//

#pragma once

#include <iostream>
#include <iterator>
#include <string>

using namespace std;


//
// myvector<T>
//
template<typename T>
class myvector
{
private:
	// One node of the vector, i.e. an array "chunk" of size nodeSize.
	class Node
	{
	public:
		T *Elements;
		int         NumElems;
		int         NodeSize;
		class Node *Next;

		// constructor
		Node(int nodesize)
			: Elements(new T[nodesize]), NumElems(0), NodeSize(nodesize), Next(nullptr)
		{
			string s = "Node: constructor called (size" + to_string(NodeSize) + ")";
			Logger::WriteMessage(s.c_str());
		}

		// destructor
		~Node()
		{

		}

		void push_back(const T& e)
		{
			Elements[NumElems] = e;
			NumElems++;
		}
	};

	// iterator
	class iterator
	{
	public:
		Node *cur;
		int index;

		iterator(Node *c, int i)
			: cur(c), index(i)
		{ }

		iterator& operator++()
		{
			index++;

			if (cur->NodeSize == index)
			{
				cur = cur->Next;
				index = 0;
			}

			return *this;
		}

		T& operator*()
		{
			return cur->Elements[index];
		}

		bool operator!=(const iterator& rhs)
		{
			if (cur != rhs.cur)
				return true;
			if (index != rhs.index)
				return true;

			return false;
		}
	};

	//
	// myvector:
	//
private:
	Node  *Head;
	Node  *Tail;
	int    NumElems;
	int    NumNodes;

public:
	myvector()
		: Head(new Node(4)), Tail(Head), NumElems(0), NumNodes(1)
	{
		Logger::WriteMessage("myvector: constructor called");
	}

	// copy constructor:
	myvector(const myvector& other) 
		: Head(new Node(4)), Tail(Head), NumElems(0), NumNodes(1)
	{

		Node *temp = other.Head;

		while (temp != nullptr)
		{
			for (int i = 0; i < temp->NumElems; i++)
			{
				push_back(temp->Elements[i]);
			}

			temp = temp->Next;
		}
		
	}

	// destructor:
	~myvector()
	{

		Node *tem1 = Head;
		Node *tem2 = tem1->Next;

		while (tem2 != nullptr)
		{
			delete(tem1);
			tem1 = tem2;
			tem2 = tem1->Next;
		}
		delete(tem1);
	}

	int size() const
	{
		return NumElems;
	}

	int capacity() const
	{
		int ttlCap = 0;
		Node *tem = Head;

		while (tem!= nullptr)
		{
			ttlCap += tem->NodeSize;
			tem = tem->Next;
		}

		return ttlCap;
	}

	int nodes() const
	{
		return NumNodes;
	}

	void push_back(const T& e)
	{

		if (Tail->NodeSize == Tail->NumElems)
		{
			Tail->Next = new Node(Tail->NodeSize * 2);
			Tail = Tail->Next;
			NumNodes++;
		}

		Tail->push_back(e);  
		NumElems++;
	}

	T& operator[](int i)
	{
		int count = i;
		int itea = 4;
		int nth = 0;
		Node *tem = Head;

		while (count >= itea)
		{
			count -= itea;
			nth++;
			
			itea *= 2;
		}

		for (int j = 0; j < nth; j++)
		{
			tem = tem->Next;
		}

		return tem->Elements[count];
	}

	T& front()
	{
		return Head->Elements[0];
	}

	T& back()
	{
		return Tail->Elements[Tail->NumElems - 1];
	}

	iterator begin()
	{
		return iterator(Head, 0);
	}
	
	iterator end()
	{
		if (Tail->NodeSize > Tail->NumElems)
			return iterator(Tail, Tail->NumElems);
		else
			return iterator(Tail->Next, 0);
	}

};
