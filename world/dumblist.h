#ifndef DUMBLIST_H
#define DUMBLIST_H

template<typename T> class List
{
	class Node
	{
	public:
		Node *next;
		T value;
	};

	Node *start; //начальный нод
	Node *iterator; //нод для итерации - вспомогательный
	int *dlink; //счетчик д-копий
public:
	List(); //создает пустой список с собственным д-счетчиком
	List(const List& other); //копирует список (д-копия)
	List& operator=(const List& other); //перед д-копией сначала d-clear
	~List(); //чистый вызов d-clear

	bool append(T value); //добавляет в конец значение и вернет индекс
	void clear(); //очистка списка без учета д-копий, насильно

	bool toStart(); //устанавливает итератор в start (если есть вообще start)
	T& next(); //переходит к следующему ноду и возвращает его значение
		//(при переходе за конец списка ошибается).
	bool atEnd(); //true - если в конце списка (текущий указатель - 0)

	int count(); //размер списка
	T& at(int index); //возвращает элемент с индексом
};

//----------------------------- IMPLEMENTATION ---------------------------------

template<typename T> List<T>::List()
{
	start=iterator=0;
	dlink=new int;
	*dlink=1;
}

template<typename T> List<T>::List(const List& other)
{
	start=other.start;
	iterator=other.iterator;
	dlink=other.dlink;
	++(*dlink);
}

template<typename T> List<T>& List<T>::operator=(const List& other)
{
	--(*dlink);
	if(*dlink==0)
		clear();

	start=other.start;
	iterator=other.iterator;
	dlink=other.dlink;
	++(*dlink);

	return *this;
}

template<typename T> List<T>::~List()
{
	--(*dlink);
	if(*dlink==0)
		clear();
}

template<typename T> void List<T>::clear()
{
	iterator=start;
	while(iterator)
	{
		start=start->next;
		delete iterator;
		iterator=start;
	}
}

template<typename T> bool List<T>::append(T value)
{
	Node *newNode=new Node;
	if(!newNode) return false;
	newNode->value=value;
	newNode->next=0;

	if(start)
	{
		Node *last=start;
		for(;last->next;last=last->next);
		last->next=newNode;
	}
	else
		start=newNode;

	return true;
}

template<typename T> bool List<T>::toStart()
{
	iterator=start;
	return (iterator!=0);
}

template<typename T> T& List<T>::next()
{
	Node *old=iterator;
	iterator=iterator->next;
	return old->value;
}

template<typename T> bool List<T>::atEnd()
{
	return (iterator==0);
}

template<typename T> int List<T>::count()
{
	Node *node=start;
	int i=0;
	for(i=0;node;i++)
		node=node->next;
	return i;
}

template<typename T> T& List<T>::at(int index)
{
	Node *node=start;
	for(int i=0;i<index;i++)
		node=node->next;
	return node->value;
}

#endif // DUMBLIST_H
