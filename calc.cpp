/*

Задание:
По «правильной» записи арифметического выражения в «инфиксной форме» найти его значение и
вывести «постфиксную форму»  этого выражения.
Под «правильной» записью подразумевается: корректная расстановка скобок, невозможность
поставить два знака операций подряд, отсутствие лишних символов.
	«инфиксная форма»			2 + 3
	«префиксная форма»			+ 2 3
	«постфиксная форма» - ОПН 	2 3 +

Требования к программе:

1.При делении, проверка на ненулевой знаменатель
2.Числа в выражении могут быть целые, дробные и в формате с плавающей точкой
3.Числа в выражении могут со знаком и без знака, знак числа может быть только после
скобки или в начале строки: -2*(-5)  или 3*(-2 + 5)
4.Стек реализовать в виде линейного списка. (Не использовать стандартные библиотеки!!!)
5.Описание стека, и все операции для работы с ним можно оформить в виде отдельной
библиотеки (файл с расширением *.h и *.cpp)
6.Используем шаблоны функций

Вычисление значения выражения по ОПН.
Для арифметического выражения: «7+((6+2)/(1+1)-3)*(9+(2+5)*2)=»
Имеем строку S = '7 6 2 + 1 1 + / 3 - 9 2 5 + 2 * + * + =', являющуюся
обратной польской записью этого выражения.  Для вычисления значения этого выражения
используем стек Х, переменная i перебирает символы строки.
Если очередной символ число - записываем его в стек Х, если арифметическая операция, то снимаем
два числа из стека Х, выполняем операцию и результат помещаем в стек Х.


*/

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>



using namespace std;


template <typename T>
struct list
{
    T id;
    list* next;
    int val = 1;
};


template <typename T>
void push(list<T>*& ptr, T x)
{
    list<T>* tmp = new list<T>;
    (*tmp).id = x;
    tmp->next = ptr;
    ptr = tmp;
}


template <typename T>
void print_list(list<T>* ptr)
{
    if (ptr->val != 0)
    {
        cout << ptr->id << " -> ";
        print_list(ptr->next);
    }
}


template <typename T>
T pop(list<T>*& ptr)
{
    list<T>* tmp = ptr;
    T x = tmp->id;
    ptr = ptr->next;
    delete(tmp);
    return(x);
}
int priority(char x) {
    if (x == '=') return 1;
    if (x == '(') return 2;
    if (x == '+' || x == '-') return 3;
    if (x == '*' || x == '/') return 4;
    return -1;
}
int operate(list<char>*&lst1, list<double>*& lst, string& s) {
    char op = pop(lst1);
    s += op;
    s += " ";
    switch (op) {
    case('+'):
        push(lst, pop(lst) + pop(lst));
        break;
    case('-'):
        push(lst, -(pop(lst) - pop(lst)));
        break;
    case('*'):
        push(lst, pop(lst) * pop(lst));
        break;
    case('/'):
        double x = pop(lst);
        double y = pop(lst);
        if (x != 0) push(lst, y / x);
        else {
            std::cout << "Division by zero!!!";
            return 0;
        }
        break;
    }
    return -1;
}
int main()
{
    ifstream f("input.txt");
    if(!f) {
        std::cout<< "File <input.txt> not open";
        exit(1);
    }
    list<double>* stackX = NULL;
    list<char>* stackY = NULL;

    string str;
    std::getline(f, str);
    std::cout << str<<"\n";
    size_t i = 0;           // указатель в строке str
    double x;
    int sgn = 1;            // знак числа
    string s;               // строка для вывода ОПН
    while (i < str.size()) {
        if( str[i]==' '|| str[i]=='\t') {
            i++;
            continue;
        }
        // std::cout<<str[i]<<", s=<"<<s<<">\n";   // для отладки только
        if (isdigit(str[i])) {
            size_t d=0;
            //x = std::stod(str.substr(i), &d);
            const char* pBeg = str.c_str()+i;             // начало строки
            char* pEnd;
            x = strtod(pBeg, &pEnd);
            d = pEnd-pBeg;
            push(stackX, sgn * x);
            s += str.substr(i, d)+' ';
            i+=d;
            sgn = 1;

        }
        else {
            if (str[i] == ')') {
                while (stackY != NULL && stackY->id != '(') {
                    operate(stackY, stackX, s);
                }
                pop(stackY);
                i++;
            }
            else {
                if (str[i] == '=') {
                    while (stackY!=NULL) {
                        operate(stackY, stackX, s);
                    }
                    i++;
                }
                else {
                    int t = priority(str[i]);
                    if (t == 2) {
                        push(stackY, str[i]);
                        i++;
                    }
                    else {
                        if ((str[i] == '-' && i == 0) || (str[i] == '-' && str[i - 1] == '(')) sgn = -1;
                        else {
                            while (stackY != NULL && t <= priority(stackY->id)) {
                                operate(stackY, stackX, s);
                            }
                            push(stackY, str[i]);

                        }
                        i++;
                    }
                }
            }
        }
    }
    std::cout << "\n" << s << "=";
    std::cout << "\nAnswer: " << stackX->id << "\n";
   // std::cin >> sgn;
    return 0;
}
