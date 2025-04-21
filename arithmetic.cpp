// реализация функций и классов для вычисления арифметических выражений
#include "../include/arithmetic.h"
#include "../include/stack.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>
using namespace std;


char TArithmeticExpression::GetType(char a) {
	if (isalpha(a)) {
		return '0';
	}
	else if ((isdigit(a)) || (a == '.')) {
		return '1';
	}
	else if ((a == '(') || (a == ')') || (a == '*') || (a == '/') || (a == '+') || (a == '-')) {
		return '2';
	}
	else {
		return '3';
	}
}


bool TArithmeticExpression::Check() {
	int flag_bracket = 0, flag_operators = 0,
		flag_points = 0;
	if ((GetType(infix[0]) == '2' && infix[0] != '(' && infix[infix.size() - 1] != ')') ||
		(GetType(infix[infix.size() - 1]) == '2' && infix[0] != '(' && infix[infix.size() - 1] != ')') ||
		(infix[0] == '.') || (infix[infix.size() - 1] == '.')) {
		return false;
	}

	for (int i = 0; i < infix.size(); i++) {
		if (GetType(infix[i]) == '3') {
			return false;
		}
		if (flag_bracket < 0) {
			return false;
		}
		if (infix[i] == '(') {
			if (i > 0 && (i < infix.size() - 2)) {
				if (GetType(infix[i - 1]) != '2' || (GetType(infix[i + 1]) == '2' && (infix[i + 1] != ')'))) {
					return false;
				}
			}
			flag_bracket += 1;
		}
		else if (infix[i] == ')') {
			flag_bracket -= 1;
		}
		else if (GetType(infix[i]) == '0') {
			flag_operators = 0;
		}
		else if (GetType(infix[i]) == '2') {
			if (flag_operators > 0) {
				return false;
			}
			if (flag_points > 1) {
				return false;
			}
			flag_points = 0;
			flag_operators = 1;
		}
		else if (infix[i] == '.') {
			if (GetType(infix[i - 1]) != '1') {
				return false;
			}
			flag_points += 1;
		}
		else if (GetType(infix[i]) == '1') {
			if (i != 0) {
				if (GetType(infix[i - 1]) == '0') {
					return false;
				}
			}
			flag_operators = 0;
		}
	}
	if (flag_bracket > 0) {
		return false;
	}
}


void TArithmeticExpression::Parse_Polynom() {
	string elem = "";
	int is_polinom = 0;
	for (int i = 0; i < infix.size(); i++) {
		if (GetType(infix[i]) != '2') {
			elem += infix[i];
			if (infix[i] == 'x') {
				is_polinom = 1;
			}
		}
		else {
			if (elem != "") {
				if (is_polinom) {
					All_Elems.push_back("p" + elem);
				}
				else {
					All_Elems.push_back(elem);
				}
			}
			elem = "";
			std::string str(1, infix[i]);
			All_Elems.push_back(str);
		}
	}
	if (elem != "") {
		if (is_polinom) {
			All_Elems.push_back("p" + elem);
		}
		else {
			All_Elems.push_back(elem);
		}
	}
}

/*
void TArithmeticExpression::Parse() {
	if (!Check()) {
		throw "Incorrect input";
	}
	else {
		string token(1, infix[0]);
		int p = 1, k = 0, prevType = GetType(infix[0]);
		while (p < infix.size()) {
			int x = GetType(infix[p]);
			if ((prevType != x) || (prevType == '2') && (x == '2')) {
				lexems.push_back(token);
				token = string(1, infix[p]);
				k++;
			}
			else {
				token += infix[p];
			}
			prevType = x;
			p++;
		}
		lexems.push_back(token);
	}

};
*/

void TArithmeticExpression::ToPostfix_Polynom() {
	Parse_Polynom();
	TVectorStack <string> operator_stack;
	for (int i = 0; i < All_Elems.size(); i++) {
		string item = All_Elems[i];
		if (item[0] == 'p' || GetType(item[0]) == '1') {
			postfix.push_back(item);
		}
		else if (GetType(item[0]) == '2') {
			if (item == "(") {
				operator_stack.Push(item);
			}
			else if (item == ")") {
				string last = operator_stack.Pop();
				while (last != "(") {
					postfix.push_back(last);
					last = operator_stack.Pop();
				}
			}
			else {
				if (operator_stack.IsEmpty()) {
					operator_stack.Push(item);
				}
				else {
					if (priority[item[0]] > priority[operator_stack.LastElem()[0]]) {
						operator_stack.Push(item);
					}
					else {
						while (!operator_stack.IsEmpty()) {
							if (priority[item[0]] <= priority[operator_stack.LastElem()[0]] && item != "(" && item != ")") {
								postfix.push_back(operator_stack.Pop());
							}
							else {
								break;
							}
						}
						operator_stack.Push(item);
					}
				}
			}
		}
	}
	if (!operator_stack.IsEmpty()) {
		while (!operator_stack.IsEmpty()) {
			postfix.push_back(operator_stack.Pop());
		}
	}
}


void TArithmeticExpression::ToPostfix() {
	//Parse();
	TVectorStack <string> operator_stack;
	for (int i = 0; i < lexems.size(); i++) {
		string item = lexems[i];
		if (item[0] == 'p' || GetType(item[0]) == '1') {
			postfix.push_back(item);
		}
		else if (GetType(item[0]) == '2') {
			if (item == "(") {
				operator_stack.Push(item);
			}
			else if (item == ")") {
				string last = operator_stack.Pop();
				while (last != "(") {
					postfix.push_back(last);
					last = operator_stack.Pop();
				}
			}
			else {
				if (operator_stack.IsEmpty()) {
					operator_stack.Push(item);
				}
				else {
					if (priority[item[0]] > priority[operator_stack.LastElem()[0]]) {
						operator_stack.Push(item);
					}
					else {
						while (!operator_stack.IsEmpty()) {
							if (priority[item[0]] <= priority[operator_stack.LastElem()[0]] && item != "(" && item != ")") {
								postfix.push_back(operator_stack.Pop());
							}
							else {
								break;
							}
						}
						operator_stack.Push(item);
					}
				}
			}
		}
	}
	if (!operator_stack.IsEmpty()) {
		while (!operator_stack.IsEmpty()) {
			postfix.push_back(operator_stack.Pop());
		}
	}
}


TArithmeticExpression::TArithmeticExpression(string infix) : infix(infix) {
	priority = { {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2} };
	ToPostfix();
}


string TArithmeticExpression::GetInfix() const {
	return infix;
}


string TArithmeticExpression::GetPostfix() const {
	string res;
	for (int i = 0; i < postfix.size(); i++) {
		res += postfix[i];
	}
	return res;
}


double* TArithmeticExpression::Get_Elem(string item) {
	for (int i = 0; i < operands_value.size(); i++) {
		if (operands_value[i].first == item) {
			return &(operands_value[i].second);
		}
	}
	return nullptr;
}


double TArithmeticExpression::Get_Operand(string item) {
	double* a = Get_Elem(item);
	if (a == nullptr) {
		double b;
		cout << "Введите значение " << item << ": ";
		cin >> b;
		operands_value.push_back(std::pair <string, double>(item, b));
		return b;
	}
	return *a;
}


double TArithmeticExpression::Calculate() {
	TVectorStack <string> operators;
	TVectorStack <double> numbers;
	for (int i = 0; i < postfix.size(); i++) {
		string item = postfix[i];
		char type_elem = GetType(item[0]);
		if (type_elem == '0') {
			double c = Get_Operand(item);
			numbers.Push(c);
		}
		else if (type_elem == '1') {
			numbers.Push(stod(item));
		}
		else if (type_elem == '2') {
			double first_num = numbers.Pop();
			double second_num = numbers.Pop();
			double result;
			if (item == "+") {
				result = first_num + second_num;
			}
			else if (item == "-") {
				result = second_num - first_num;
			}
			else if (item == "*") {
				result = first_num * second_num;
			}
			else {
				if (second_num == 0 || first_num == 0) {
					throw "Division by zero";
				}
				result = second_num / first_num;
			}
			numbers.Push(result);
		}
	}
	return numbers.Pop();
}


Polynom Get_Monom(string m) {
	string res;
	for (int i = 1; i < m.size(); i++) {
		res += m[i];
	}
	Polynom result(res);
	return result;
}


Polynom TArithmeticExpression::Calculate_Polynom() {
	TVectorStack <string> elems;
	for (int i = 0; i < postfix.size(); i++) {
		Polynom result;
		string item = postfix[i];
		char type_elem = GetType(item[0]);
		if (item[0] == 'p' || type_elem == '1') {
			elems.Push(item);
		}
		else if (type_elem == '2') {
			string first_elem = elems.Pop();
			string second_elem = elems.Pop();
			Polynom first_m;
			Polynom second_m;
			double first_d;
			double second_d;
			double r;
			int f_p = 0;
			int s_p = 0;
			if (first_elem[0] == 'p') {
				first_m = Get_Monom(first_elem);
				f_p = 1;
			}
			else {
				first_d = stod(first_elem);
			}
			if (second_elem[0] == 'p') {
				second_m = Get_Monom(second_elem);
				s_p = 1;
			}
			else {
				second_d = stod(second_elem);
			}
			if (f_p && s_p) {
				if (item == "+") {
					Polynom p = first_m + second_m;
					if (result.Empty()) {
						result = p;
					}
					else {
						result = result + p;
					}
				}
				else if (item == "-") {
					Polynom p = second_m - first_m;
					if (result.Empty()) {
						result = p;
					}
					else {
						result = result + p;
					}
				}
				else {
					Polynom p = first_m * second_m;
					if (result.Empty()) {
						result = p;
					}
					else {
						result = result + p;
					}
				}
				string s = 'p' + result.to_string();
				elems.Push(s);
			}
			else if (f_p) {
				Polynom p = first_m * second_d;
				if (result.Empty()) {
					result = p;
				}
				else {
					result = result + p;
				}
				string s = 'p' + result.to_string();
				elems.Push(s);
			}
			else if (s_p) {
				Polynom p = second_m * first_d;
				if (result.Empty()) {
					result = p;
				}
				else {
					result = result + p;
				}
				string s = 'p' + result.to_string();
				elems.Push(s);
			}
			else {
				if (item == "+") {
					r = first_d + second_d;
				}
				else if (item == "-") {
					r = second_d - first_d;
				}
				else if (item == "*") {
					r = first_d * second_d;
				}
				else {
					if (first_d == 0) {
						throw "Division by zero";
					}
					r = second_d / first_d;
				}
				string s = to_string(r);
				elems.Push(s);
			}
		}
	}
	return Get_Monom(elems.Pop());
}


Monom::Monom() : coeff(0), pow_x(0), pow_y(0), pow_z(0) {}

Monom::Monom(const Monom& m) noexcept : coeff(m.coeff), pow_x(m.pow_x), pow_y(m.pow_y), pow_z(m.pow_z) {}

Monom::Monom(double c, int px, int py, int pz) noexcept : coeff(c), pow_x(px), pow_y(py), pow_z(pz) {}

Monom::Monom(const string& s)
{
	std::string::const_iterator it = s.begin();
	std::string sub;

	for (it; *it != 'x'; ++it)
	{
		sub.push_back(*it);
	}
	++it;

	coeff = std::stod(sub);
	sub.clear();

	for (it; *it != 'y'; ++it)
	{
		sub.push_back(*it);
	}
	++it;
	pow_x = std::stoi(sub);
	sub.clear();

	for (it; *it != 'z'; ++it)
	{
		sub.push_back(*it);
	}
	++it;
	pow_y = std::stoi(sub);
	sub.clear();

	for (it; it != s.end(); ++it)
	{
		sub.push_back(*it);
	}
	pow_z = std::stoi(sub);
}

Monom& Monom::operator=(const Monom& m) noexcept
{
	if (*this == m)
	{
		return *this;
	}

	coeff = m.coeff;
	pow_x = m.pow_x;
	pow_y = m.pow_y;
	pow_z = m.pow_z;

	return *this;
}

Monom Monom::operator+(const Monom& m)
{
	if ((pow_x == m.pow_x) && (pow_y == m.pow_y) && (pow_z == m.pow_z))
	{
		return Monom(coeff + m.coeff, pow_x, pow_y, pow_z);
	}
	else
	{
		throw std::invalid_argument("Cannot add monomials with different pows");
	}
}

Monom Monom::operator-(const Monom& m)
{
	if ((pow_x == m.pow_x) && (pow_y == m.pow_y) && (pow_z == m.pow_z))
	{
		return Monom(coeff - m.coeff, pow_x, pow_y, pow_z);
	}
	else
	{
		throw std::invalid_argument("Cannot add monomials with different pows");
	}
}

Monom Monom::operator*(const double& oth) const noexcept
{
	if (oth == 0 || coeff == 0.0)
	{
		return Monom();
	}
	return Monom(coeff * oth, pow_x, pow_y, pow_z);
}

Monom Monom::operator*(const Monom& m) const noexcept
{
	if (coeff == 0 || m.coeff == 0)
	{
		return Monom();
	}

	return Monom(coeff * m.coeff, pow_x + m.pow_x, pow_y + m.pow_y, pow_z + m.pow_z);
}

bool Monom::operator==(const Monom& m) const noexcept
{
	return (coeff == m.coeff) && (pow_x == m.pow_x) && (pow_y == m.pow_y) && (pow_z == m.pow_z);
}

bool Monom::operator!=(const Monom& m) const noexcept
{
	return !(*this == m);
}

ostream& operator<<(ostream& ostr, const Monom& m)
{
	ostr << m.coeff;
	ostr << "x" << m.pow_x;
	ostr << "y" << m.pow_y;
	ostr << "z" << m.pow_z;
	return ostr;
}

bool Monom::EqPow(const Monom& m) const noexcept
{
	return (pow_x == m.pow_x) && (pow_y == m.pow_y) && (pow_z == m.pow_z);
}


double Monom::GetCoeff() const
{
	return coeff;
}


Polynom::Polynom(const string& s) //-3.1x10y10z10 + 20.2x-10y-10z-10
{
	std::string copy_s = s;
	std::string tmp;
	std::string::iterator it = copy_s.begin();

	while (it != copy_s.end())
	{
		if (*it != ' ' && *it != '+')
		{
			tmp.push_back(*it);
		}
		else if (*it == '+')
		{
			polinoms.InsertToTail(Monom(tmp));
			tmp.clear();
		}

		++it;
	}

	if (it == copy_s.end())
	{
		polinoms.InsertToTail(Monom(tmp));
		tmp.clear();
	}
}


Polynom::Polynom(Polynom&& p) noexcept
{
	polinoms = std::move(p.polinoms);
}


Polynom::Polynom(const Polynom& p)
{
	polinoms.Clean();
	List<Monom> second_polinoms = p.polinoms;
	listIterator<Monom> start = second_polinoms.begin();

	while (start != second_polinoms.end())
	{
		polinoms.InsertToTail(*start);
		++start;
	}
}


Polynom& Polynom::operator=(const Polynom& p)
{
	if (this == &p)
	{
		return *this;
	}

	polinoms.Clean();
	List<Monom> second_polinoms = p.polinoms;
	listIterator<Monom> start = second_polinoms.begin();

	while (start != second_polinoms.end())
	{
		polinoms.InsertToTail(*start);
		++start;
	}

	return *this;
}


Polynom& Polynom::operator=(Polynom&& p) noexcept
{
	polinoms = std::move(p.polinoms);
	return *this;
}


Polynom Polynom::operator*(Monom& m)
{
	Polynom res;
	listIterator<Monom> start = polinoms.begin();

	for (start; start != polinoms.end(); ++start)
	{
		Monom tmp = *start * m;
		res.polinoms.InsertToTail(tmp);
	}

	return res;
}


Polynom Polynom::operator*(double& oth)
{
	Polynom res;
	listIterator<Monom> start = polinoms.begin();

	for (start; start != polinoms.end(); ++start)
	{
		Monom tmp = *start * oth;
		res.polinoms.InsertToTail(tmp);
	}

	return res;


}


Polynom Polynom::operator+(Polynom& p)
{
	Polynom res;

	res.polinoms = polinoms.Merge(p.polinoms);
	res.DelCoeffZero();

	listIterator<Monom> it = res.polinoms.begin();
	listIterator<Monom> next_it = it;

	while (it != res.polinoms.end())
	{
		next_it = it;
		++next_it;

		while (next_it != res.polinoms.end())
		{
			if ((*it).EqPow(*next_it))
			{
				*it = *it + *next_it;
				listIterator<Monom> to_delete = next_it;
				++next_it;
				res.polinoms.Delete(*to_delete);
			}
			else
			{
				++next_it;
			}
		}
		++it;
	}

	return res;
}


Polynom Polynom::operator-(Polynom& p)
{
	Polynom res;
	double c = -1.0;
	p = p * c;
	res = *this + p;

	return res;
}


Polynom Polynom::operator*(Polynom& p)
{
	Polynom res;

	listIterator<Monom> it1 = polinoms.begin();


	for (it1; it1 != polinoms.end(); ++it1)
	{
		for (listIterator<Monom> it2 = p.polinoms.begin(); it2 != p.polinoms.end(); ++it2)
		{
			Monom tmp = *it1 * *it2;
			res.polinoms.InsertToTail(tmp);
		}
	}
	res.DelCoeffZero();

	listIterator<Monom> it = res.polinoms.begin();
	listIterator<Monom> next_it = it;

	while (it != res.polinoms.end())
	{
		next_it = it;
		++next_it;

		while (next_it != res.polinoms.end())
		{
			if ((*it).EqPow(*next_it))
			{
				*it = *it + *next_it;
				listIterator<Monom> to_delete = next_it;
				++next_it;
				res.polinoms.Delete(*to_delete);
			}
			else
			{
				++next_it;
			}
		}
		++it;
	}


	return res;
}


bool Polynom::operator==(const Polynom & p) const noexcept{
	if (polinoms == p.polinoms) {
		return true;
	}
	return false;
}


vector <double> Monom::Get_Pows() {
	vector <double> r;
	r.push_back(pow_x);
	r.push_back(pow_y);
	r.push_back(pow_z);
	return r;
}


string tostring(Monom& m) {
	double c = m.GetCoeff();
	vector <double> p = m.Get_Pows();
	string res = std::to_string(c) + 'x' + std::to_string(p[0]) + 'y' + std::to_string(p[1]) + 'z' + std::to_string(p[2]);
	return res;
}

string Polynom::to_string() {
	string res;
	listIterator<Monom> start = polinoms.begin();
	listIterator<Monom> end = polinoms.end();

	for (start; start != end; ++start)
	{
		Monom a = *start;
		res +=  tostring(a);
		if (std::next(start) != end)
		{
			res += " + ";
		}
	}

	return res;
}

ostream& operator<<(ostream& ostr, const Polynom& p)
{
	List<Monom> second_polinoms = p.polinoms;
	listIterator<Monom> start = second_polinoms.begin();
	listIterator<Monom> end = second_polinoms.end();

	for (start; start != end; ++start)
	{
		ostr << *start;
		if (std::next(start) != end)
		{
			ostr << " + ";
		}
	}

	return ostr;
}


istream& operator>>(istream& istr, Polynom& poli)
{
	poli.polinoms.Clean();

	std::string input;

	cout << "Enter <<end>> to exit" << endl;
	cout << "Enter Polinom:" << endl;

	while (true)
	{
		cout << "> ";
		istr >> input;

		if (input == "end")
		{
			break;
		}

		poli.polinoms.InsertToTail(Monom(input));
	}

	return istr;
}


bool Polynom::Empty() {
	if (polinoms.isEmpty()) {
		return true;
	}
	return false;
}


void Polynom::DelCoeffZero()
{
	listIterator<Monom> current = polinoms.begin();
	int k = polinoms.GetSize();
	if (k  == 1) {
		if ((*current).GetCoeff() == 0.0)
		{
			listIterator<Monom> del = current;
			polinoms.Delete(*del);
		}
	}
	else if (k >= 2) {
		while (current != polinoms.end())
		{
			if ((*current).GetCoeff() == 0.0)
			{
				listIterator<Monom> del = current;
				++current;
				polinoms.Delete(*del);
			}
			else
			{
				++current;
			}
		}
	}
}
