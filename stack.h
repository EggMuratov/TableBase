// объ€вление и реализаци€ шаблонного стека
// стек поддерживает операции: 
// - вставка элемента, 
// - извлечение элемента, 
// - просмотр верхнего элемента (без удалени€)
// - проверка на пустоту, 
// - получение количества элементов в стеке
// - очистка стека
// при вставке в полный стек должна перевыдел€тьс€ пам€ть
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
	size_t size() const {   // - получение количества элементов в стеке
		return top + 1;
	}

	bool IsEmpty() const {   // - проверка на пустоту, 
		return top == -1;
	}

	void Push(const T& val) {  // - вставка элемента
		mem.push_back(val);
		top++;
	}

	T Pop() {  // - извлечение элемента
		if (top == -1) {
			throw "Stack is empty";
		}
		T val = mem.back();
		mem.pop_back();
		top--;
		return val;
	}

	T LastElem() {  // - просмотр верхнего элемента (без удалени€)
		if (top == -1) {
			throw "Stack is empty";
		}
		return  mem[top];
	}

	void ClearStack() {   // - очистка стека
		if (top == -1) {
			throw "Stack is empty";
		}
		mem.clear();
		top = -1;
	}
};
#endif