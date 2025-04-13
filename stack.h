// ���������� � ���������� ���������� �����
// ���� ������������ ��������: 
// - ������� ��������, 
// - ���������� ��������, 
// - �������� �������� �������� (��� ��������)
// - �������� �� �������, 
// - ��������� ���������� ��������� � �����
// - ������� �����
// ��� ������� � ������ ���� ������ �������������� ������
#ifndef __STACK_H
#define __STACK_H
#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class TVectorStack {
	int top;
	std::vector<T> mem;
public:
	TVectorStack() : top(-1) {}
	size_t size() const {   // - ��������� ���������� ��������� � �����
		return top + 1;
	}

	bool IsEmpty() const {   // - �������� �� �������, 
		return top == -1;
	}

	void Push(const T& val) {  // - ������� ��������
		mem.push_back(val);
		top++;
	}

	T Pop() {  // - ���������� ��������
		if (top == -1) {
			throw "Stack is empty";
		}
		T val = mem.back();
		mem.pop_back();
		top--;
		return val;
	}

	T LastElem() {  // - �������� �������� �������� (��� ��������)
		if (top == -1) {
			throw "Stack is empty";
		}
		return  mem[top];
	}

	void ClearStack() {   // - ������� �����
		if (top == -1) {
			throw "Stack is empty";
		}
		mem.clear();
		top = -1;
	}
};
#endif