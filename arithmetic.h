// ���������� ������� � ������� ��� ���������� �������������� ���������
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>
using namespace std;




template <typename DataType>
struct Node
{
	DataType data;
	Node* next;

	Node(DataType d = DataType(), Node* n = nullptr) noexcept : data(d), next(n) {};
	Node(const Node& node2) noexcept : data(node2.data), next(node2.next) {};


	bool operator==(const Node& node2) const {
		return (data == node2.data);
	}
	bool operator!=(const Node& node2) const {
		return !operator==(node2);
	}
};


template <typename DataType>
class listIterator : public std::iterator<std::forward_iterator_tag, DataType>
{
public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = DataType;
	using pointer = Node<DataType>*;
	using reference = DataType&;

	listIterator(pointer tmp) : p(tmp) {};
	reference operator*() {
		return p->data;
	}
	pointer  operator->() {
		return p;
	}
	listIterator& operator++() {
		p = p->next;
		return *this;
	}
	listIterator operator++(int) {
		listIterator t = *this;
		++*this;
		return t;
	}
	bool operator==(const listIterator& it2) const {
		return p == it2.p;
	}
	bool operator!=(const listIterator& it2) const {
		return !operator==(it2);
	}

private:
	pointer p;
};


template <typename DataType>
class List
{
	Node<DataType>* fictive_head;
	Node<DataType>* head;
	Node<DataType>* Tail;

public:
	List() noexcept : head(nullptr), Tail(nullptr), fictive_head(nullptr) {} // ������� ������ ������

	List(const DataType& d) {
		head = new Node<DataType>(d);
		fictive_head = new Node<DataType>;
		Tail = new Node<DataType>;
		fictive_head->next = head;
		Tail->data = head->data;
		head->next = fictive_head;
	} // ������� ������ �� ������ �����

	bool isEmpty() {
		return head == NULL;
	}

	void InsertToHead(const DataType& d) {
		if (isEmpty()) {
			head = new Node<DataType>;
			head->data = d;
			fictive_head = new Node<DataType>;
			Tail = new Node<DataType>;
			fictive_head->next = head;
			Tail->data = head->data;
			head->next = fictive_head;
		}
		else if (GetSize() == 1) {
			head->data = d;
			head->next = Tail;
			Tail->next = fictive_head;
		}
		else {
			Node<DataType>* tmp;
			tmp = head;
			head = new Node<DataType>;
			head->data = d;
			head->next = tmp;
			fictive_head->next = head;
		}
	}// �������� ������� d ������

	DataType GetFirst() {
		if (isEmpty()) {
			throw "List is empty";
		}
		return head->data;
	} // ���������� �������

	void InsertToTail(const DataType& d) {
		if (isEmpty()) {
			head = new Node<DataType>(d);
			fictive_head = new Node<DataType>;
			Tail = new Node<DataType>;
			fictive_head->next = head;
			Tail->data = head->data;
			head->next = fictive_head;
		}
		else if (GetSize() == 1) {
			Tail->data = d;
			head->next = Tail;
			Tail->next = fictive_head;
		}
		else {
			//Node<DataType>* tmp;
			//tmp = new Node<DataType>(d);
			//if (head->next == Tail && Tail->data == head->data) {
			//	Tail->data = tmp->data;
			//}
			//else 
			//{
			Node<DataType>* tmp1;
			tmp1 = new Node<DataType>(d);
			//tmp1->data = Tail->data;
			tmp1->next = fictive_head;
			Tail->next = tmp1;
			Tail = tmp1;

			//}
		} // �������� ������� d ���������
	}

	DataType ViewTail() {
		if (isEmpty()) {
			throw "List is empty";
		}
		return Tail->data;
	} // ���������� ����������

	void Delete(const DataType& d) {
		if (isEmpty()) {
			throw "List is empty";
		}
		else if (GetSize() == 1) {
			if (head->data == d) {
				delete head;
				delete Tail;
				head = nullptr;
				Tail = nullptr;
			}
			else {
				throw "Incorrect input";
			}
		}
		else if (GetSize() == 2) {
			if (head->data == d) {
				//delete head;
				head->data = Tail->data;
				head->next = fictive_head;
				fictive_head->next = head;
				//delete Tail;
				Tail->data = head->data;
			}
			else if (Tail->data == d) {
				//delete Tail;
				Tail->data = head->data;
				Tail->next = nullptr;
				head->next = fictive_head;
			}
			else {
				throw "Incorrect input";
			}
		}
		else {
			Node<DataType>* tmp = head;
			Node<DataType>* tmp1;
			int flag = 0;
			while ((tmp->next)->data != d) {
				tmp = tmp->next;
			}
			tmp1 = tmp->next; // ������ elem
			tmp->next = tmp1->next;
			if (tmp1 == head) {
				head = tmp->next;
			}
			else if (tmp1 == Tail) {
				Tail = tmp;
			}
			delete tmp1;
		}
	} // ������� ����� �� ��������� data = d//��������

	listIterator<DataType> begin() {
		if (isEmpty()) {
			throw "List is empty";
		}
		listIterator<DataType> tmp(head);
		return tmp;
	} // �������� �������� �� ������ ����� 

	listIterator<DataType> end() {
		listIterator<DataType> tmp(fictive_head);
		return tmp;
	} // �������� �������� �� ��������� �����

	listIterator<DataType> end_1() {
		listIterator<DataType> tmp(nullptr);
		return tmp;
	} // �������� �������� �� ����� ������

	~List() {
		fictive_head = nullptr;
		head = nullptr;
		Tail = nullptr;
	}

	List(const List& list2) {
		fictive_head = list2.fictive_head;
		head = list2.head;
		Tail = list2.Tail;
	}

	List& operator=(const List& list2) {
		if (this == &list2) {
			return *this;
		}
		fictive_head = list2.fictive_head;
		head = list2.head;
		Tail = list2.Tail;
		return *this;
	}

	void Clean() {
		if (head != nullptr) {
			Node<DataType>* tmp;
			tmp = head;
			while (head->next != fictive_head) {
				head = tmp->next;
				delete tmp;
				tmp = head;
			}
			delete head;
		}
		fictive_head = nullptr;
		head = nullptr;
		Tail = nullptr;
	}// ������� ��� ������  

	void InsertAfter(listIterator<DataType> it, const DataType& d) {
		if (it != nullptr) {
			if (GetSize() == 1) {
				if (listIterator<DataType>(head) == it) {
					Tail->data = d;
					head->next = Tail;
					Tail->next = fictive_head;
				}
				else {
					throw "Incorrect input";
				}
			}
			else {
				Node<DataType>* tmp = head;
				Node<DataType>* tmp2;
				Node<DataType>* ptr;
				tmp2 = new Node<DataType>(d);
				ptr = it->next;
				it->next = tmp2;
				tmp2->next = ptr;
				if (it == tail()) {
					Tail = tmp2;
				}
			}
		}
		else {
			throw "Incorrected iterator";
		}
	} // �������� ������� d ����� ����� node //������

	listIterator<DataType> Search(const DataType& d) {
		Node<DataType>* tmp = head;
		while (tmp->data != d) {
			if (tmp == fictive_head) {
				throw "Incorrect input";
			}
			tmp = tmp->next;
		}
		return listIterator<DataType>(tmp);
	} // ����� ��������� �� ����� �� ��������� data = d //������

	void Delete(const listIterator<DataType>& it) {
		if (it != nullptr) {
			if (GetSize() == 1) {
				if (listIterator<DataType>(head) == it) {
					delete head;
					delete Tail;
					head = nullptr;
					Tail = nullptr;
				}
				else {
					throw "Incorrect input";
				}
			}
			else if (GetSize() == 2) {
				if (listIterator<DataType>(head) == it) {
					delete head;
					head->data = Tail->data;
					head->next = fictive_head;
					fictive_head->next = head;
					delete Tail;
					Tail->data = head->data;
				}
				else if (listIterator<DataType>(Tail) == it) {
					delete Tail;
					Tail->data = head->data;
					head->next = fictive_head;
				}
				else {
					throw "Incorrect input";
				}
			}
			else {
				Node<DataType>* tmp = fictive_head;
				Node<DataType>* tmp1;
				while (listIterator<DataType>(tmp->next) != it) {
					tmp = tmp->next;
				}
				tmp1 = tmp->next; // ������ elem
				tmp->next = tmp1->next;
				if (tmp1 == head) {
					head = tmp->next;
				}
				else if (tmp1 == Tail) {
					Tail = tmp;
				}
			}
		}
		else {
			throw "Incorrected iterator";
		}
	} // ������� ����� �� ��������� data = d	

	void Delete(const listIterator<DataType>& start, const listIterator<DataType>& finish) {
		if (start != nullptr || finish != nullptr) {
			int flag = 0;
			if (listIterator<DataType>(head) == start && listIterator<DataType>(Tail) == finish) {
				this->Clean();
			}
			else {
				Node<DataType>* tmp = fictive_head;
				Node<DataType>* tmp1 = new Node<DataType>;
				while (listIterator<DataType>(tmp) != start) {
					if (listIterator<DataType>(tmp) == finish) {
						throw "Incorrect iterator";
					}
					tmp = tmp->next;
				}
				while (listIterator<DataType>(tmp) != finish) {
					this->Delete(listIterator<DataType>(tmp));
					tmp = tmp->next;
				}
				this->Delete(listIterator<DataType>(tmp));
			}
		}
		else {
			throw "Incorrected iterator";
		}
	}

	void DeleteAll(const DataType& d) {
		Node<DataType>* tmp = head;
		while (tmp->data != d) {
			if (tmp == fictive_head) {
				throw "Incorrect input";
			}
			tmp = tmp->next;
		}
		this->Delete(listIterator<DataType>(tmp), listIterator<DataType>(Tail));
	}
	void Inverse() {
		if (GetSize() == 1) {}
		else if (!isEmpty()) {
			Node<DataType>* tmp = head;
			Node<DataType>* tmp_pr = new Node<DataType>;
			Node<DataType>* tmp_nx = new Node<DataType>;
			while (tmp != fictive_head) {
				tmp_nx = tmp->next;
				tmp->next = tmp_pr;
				tmp_pr = tmp;
				tmp = tmp_nx;
			}
			Tail = head;
			head = tmp_pr;
			fictive_head->next = head;
			Tail->next = fictive_head;
		}
	} // ������������� ������, �.�. ������ ������ ���� � �������� �������

	List Merge(const listIterator<DataType>& start, const List& list2) {
		if (isEmpty() && start == nullptr) {
			return list2;
		}
		if (start != nullptr || list2.head == nullptr) {
			Node<DataType>* tmp = head;
			List<DataType> l3;
			while (listIterator<DataType>(tmp) != start) {
				if (tmp == fictive_head) {
					throw "Incorrect input";
				}
				tmp = tmp->next;
			}
			tmp->next = list2.head;
			l3.fictive_head = fictive_head;
			l3.head = head;
			l3.Tail = list2.Tail;
			l3.Tail->next = l3.fictive_head;
			return l3;
		}
		return *this;
	} // ������� ������3, ������� ������2 � ������� ������ ����� ��������� 

	void MergeWith(const listIterator<DataType>& start, const List& list2) {
		if (isEmpty() && start == nullptr) {
			*this = list2;
		}
		if (start != nullptr || isEmpty()) {
			Node<DataType>* tmp = head;
			while (listIterator<DataType>(tmp) != start) {
				if (tmp == fictive_head) {
					throw "Incorrect input";
				}
				tmp = tmp->next;
			}
			tmp->next = list2.head;
			Tail = list2.Tail;
			Tail->next = fictive_head;
		}
	} // � *this �������� ������2 ����� ���������

	friend ostream& operator<<(ostream& os, const List<DataType>& l);

	friend istream& operator>>(istream& is, const List<DataType>& l);

	bool Elem_In_Vector(vector<Node<DataType>*> v, Node<DataType>* elem) {
		for (int i = 0; i < v.size(); i++) {
			if (v[i]->data == elem->data) {
				return true;
			}
		}
		return false;
	}

	void MadeUnique() { // ������
		if (GetSize() == 1) {}
		else if (!isEmpty()) {
			vector <Node<DataType>*> elems;
			Node<DataType>* tmp = head;
			while (tmp != fictive_head) {
				if (Elem_In_Vector(elems, tmp)) {
					Delete(listIterator<DataType>(tmp));
				}
				else {
					elems.push_back(tmp);
				}
				tmp = tmp->next;
			}
			//Node<DataType>* tmp_head = head; // ����� ������
			//elems.push_back(head);
			//Node<DataType>* tmp1 = head->next; //�������
			//Node<DataType>* tmp2 = new Node<DataType>;
			//int flag = 1, flag1 = 0;
			//if (GetSize() == 2) {
			//	flag1 = 1;
			//}
			//while (tmp1 != fictive_head) {
			//	if (!Elem_In_Vector(elems, tmp1)) {
			//		if (flag) {
			//			tmp_head->next = tmp1;
			//			tmp2->next = tmp1->next;
			//			tmp2->data = tmp1->data;
			//			flag = 0;
			//		}
			//		else {
			//			tmp2->next = tmp1;
			//			tmp2 = tmp2->next;
			//		}
			//		elems.push_back(tmp1);
			//	}
			//	tmp1 = tmp1->next;
			//}
			//head = tmp_head;
			//fictive_head->next = head;
			//Tail = tmp2;
			//Tail->next = fictive_head;
			//if (flag1) {
			//	delete Tail;
			//	Tail->data = head->data;
			//}
		}
	} // ��������� ������������� ������
	void Delete_Middle() {
		int k = GetSize();
		if (k == 2) {
			List<DataType> tmp_list(Tail->data);
			*this = tmp_list;
		}
		else if (k == 3) {
			head->next = Tail;
		}
		else {
			Node<DataType>* tmp = new Node<DataType>;
			Node<DataType>* tmp1 = new Node<DataType>;
			tmp->data = fictive_head->data;
			tmp->next = fictive_head->next;
			for (int i = 1; i < k / 2; i++) {
				tmp = tmp->next;
			}
			tmp1 = tmp->next;
			tmp->next = tmp1->next;
		}
	}
	bool Cycle() {
		Node<DataType>* tmp = Tail->next;
		if (tmp != fictive_head) {
			return true;
		}
		return false;
	}   // ����������, ���� �� ���� � ������

	void BreakCycle() {
		if (Cycle()) {
			Tail->next = fictive_head;
		}
	}// "����������" ���� � ������

	std::string ToString() {
		string result = "";
		Node <DataType>* tmp = head;
		while (tmp != Tail) {
			result += std::to_string(tmp->next->data) + "; ";
			tmp = tmp->next;
		}
	} // "��1; ��2; " 

	int GetSize() {
		int k = 0;
		Node<DataType>* tmp = new Node<DataType>;
		tmp->data = fictive_head->data;
		tmp->next = fictive_head->next;
		while (tmp->next != fictive_head) {
			k++;
			tmp = tmp->next;
		}
		return k;
	}
	// ������ ����� ������� � ������

	List Merge(const List& list2) {
		List<DataType> list_merged;
		Node<DataType>* tmp = head;
		Node<DataType>* tmp2 = list2.head;
		while (tmp != fictive_head) {
			list_merged.InsertToTail(tmp->data);
			tmp = tmp->next;
		}

		while (tmp2 != list2.fictive_head) {
			list_merged.InsertToTail(tmp2->data);
			tmp2 = tmp2->next;
		}

		return list_merged;
	} // ������� ������3, ������� � ����� �������� ������ ������2

	bool operator==(const List& list2) const {
		Node<DataType>* tmp1 = fictive_head;
		Node<DataType>* tmp2 = list2.fictive_head;
		while (tmp1->next != fictive_head && tmp2->next != list2.fictive_head) {
			if (tmp1->next->data != tmp2->next->data) {
				return false;
			}
			tmp1 = tmp1->next;
			tmp2 = tmp2->next;
		}
		return true;
	} // ������ �����, ���� �������� � ��� ���� � ���������� �������
};


class Monom
{
	double coeff;
	int pow_x;
	int pow_y;
	int pow_z;
public:

	Monom();
	Monom(const Monom& m) noexcept;
	Monom(double c, int px, int py, int pz) noexcept;
	Monom(const string& s);

	Monom& operator=(const Monom& m) noexcept;
	Monom operator+(const Monom& m);
	Monom operator-(const Monom& m);
	Monom operator*(const double& oth) const noexcept;
	Monom operator*(const Monom& other) const noexcept;
	string to_string();

	bool operator==(const Monom& m) const noexcept;
	bool operator!=(const Monom& m) const noexcept;

	bool EqPow(const Monom& m) const noexcept;
	double GetCoeff() const;
	vector <double> Get_Pows();

	friend ostream& operator<<(ostream& ostr, const Monom& m);
};


class Polynom
{
	List<Monom> polinoms;
public:
	Polynom() = default;
	Polynom(const string& s);
	Polynom(const Polynom& p);
	Polynom(Polynom&& p) noexcept;
	Polynom& operator=(Polynom&& p) noexcept;
	Polynom& operator=(const Polynom& other);
	Polynom operator*(Monom& m);
	Polynom operator*(double& oth);
	Polynom operator+(Polynom& other);
	Polynom operator-(Polynom& other);
	bool Empty();
	string to_string();
	Polynom operator*(Polynom& other);
	// operatot == 
	void DelCoeffZero();
	~Polynom() = default;

	friend ostream& operator<<(ostream& ostr, const Polynom& poli);
	friend istream& operator>>(istream& istr, Polynom& poli);
};

class TArithmeticExpression {
	string infix;
	vector <string> postfix;
	vector <string> lexems;
	vector <string> All_Elems;
	map <char, int> priority;
	vector <std::pair <string, double>> operands_value;
	char GetType(char a);
	bool Check();
	void Parse_Polynom();
	//void Parse();
	void ToPostfix();
public:
	TArithmeticExpression(string infix);
	void ToPostfix_Polynom();
	string GetInfix() const;
	string GetPostfix() const;
	double* Get_Elem(string item);
	double Get_Operand(string item);
	double Calculate();
	Polynom Calculate_Polynom();
};


template <typename TKey, typename TValue>struct TTableRec {
	TKey key;
	TValue value;

	TTableRec(TTableRec<TKey, TValue>& rec) : key(rec.key), value(rec.value) {};
	TTableRec(TKey& k = TKey(), TValue& v = TValue()) : key(k), value(v) {};
};

template <typename TKey, typename TValue> class TTableList {
	List<TTableRec<TKey, TValue>> table;
public:
	TTableList() {};
	TTableList(TKey& k, TValue& v) {
		TTableRec<TKey, TValue> tmp(k, v);
		table.InsertToTail(tmp);
	}
	size_t size() const noexcept { return table.GetSize(); }
	TValue& operator[](listIterator<TTableRec<TKey, TValue>> pos) { return *pos.value; }
	void Delete_Elem(TTableRec<TKey, TValue> elem) { table.Delete(elem); }
};
/*
template <typename TKey, typename TValue>
struct TTableRec {
	TKey key;
	TValue value;
	TTableRec(TKey k = TKey(), TValue val = TValue()) : key(k), value(val) {};
	TTableRec(TTableRec& rec) : key(rec.key), value(rec.value) {};
};


template <typename TKey, typename TValue> class TTableList {
	List<TTableRec<TKey, TValue>> table;
public:
	TTableList() {
		List<TTableRec<TKey, TValue>> l;
		table = l;
	};
	TTableList(TTableRec<TKey, TValue>& d){
		TTableList();
		table.InsertToHead(d);
	};
	size_t size() const noexcept { return data.GetSize();}
	TTableRec<TKey, TValue>& operator[](listIterator<TTableRec<TKey, TValue>> pos) { return *pos;}
	void Add_Elem(TTableRec<TKey, TValue> new_elem) {	table.InsertToTail(new_elem);}
	void Delete_Elem(TTableRec<TKey, TValue> elem) { table.Delete(elem); }
};
*/

/*
template <typename TKey, typename TValue> class TTableHash {
	struct TTableRec {
		TKey key;
		TValue value;
	};
	List<TTableRec> data{};
public:
	TUnorderedTable() = default;
	size_t size() const noexcept { return data.GetSize(); }
	TValue& operator[](listIterator<TTableRec> pos) { return *pos.value; }
	void Add_Elem(TTableRec new_elem) { data.InsertToTail(new_elem); }
	void Delete_Elem(TTableRec elem) { data.Delete(elem); }
};*/