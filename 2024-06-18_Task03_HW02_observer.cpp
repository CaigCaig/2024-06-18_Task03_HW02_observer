#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef enum
{
	WARNING,
	ERROR,
	FATAL_ERROR
} MESSAGE_t;

class Observer {
public:
	virtual void onWarning(const string& message) {}
	virtual void onError(const string& message) {}
	virtual void onFatalError(const string& message) {}
	virtual void setPath(const string& path_) {}
};

class Warning : public Observer
{
public:
	Warning() {};
	void onWarning(const string& message) override
	{
		cout << message << endl;
	}
};

class Error : public Observer
{
private:
	string path;
public:
	Error() {};
	void onError(const string& message) override
	{
		std::ofstream fout{ this->path, ios::app };      // открываем файл для записи
		if (fout.is_open())
		{
			fout << "Error: " << message << endl;
		}
		fout.close();
	}
	void setPath(const string& path_)
	{
		path = path_;
	}
};

class FatalError : public Observer
{
private:
	string path;
public:
	FatalError() {};
	void onFatalError(const string& message) override
	{
		cout << message << endl;
		std::ofstream fout{ this->path, ios::app };      // открываем файл для записи
		if (fout.is_open())
		{
			fout << "FatalError: " << message << endl;
		}
		fout.close();
	}
	void setPath(const string& path_)
	{
		path = path_;
	}
};

class Observed {
private:
	MESSAGE_t curr_message_t;
	string message;
	vector<Observer*> observers;
public:
	void AddObserver(Observer* obs)
	{
		observers.push_back(obs);
	}
	void RemoveObserver(Observer* obs)
	{
		auto it = remove(observers.begin(), observers.end(), obs);
		observers.erase(it, observers.end());
	}
	void NotifyObervers()
	{
		for (auto it : observers)
		{
			switch (curr_message_t)
			{
			case WARNING:
				it->onWarning(message);
				break;
			case ERROR:
				it->onError(message);
				break;
			case FATAL_ERROR:
				it->onFatalError(message);
				break;
			}
		}
	}
	void SendMessage(const MESSAGE_t curr_msg_t, const string& msg)
	{
		curr_message_t = curr_msg_t;
		message = msg;
		NotifyObervers();
	}
	void SetPath(const string& path_)
	{
		for (auto it : observers)
		{
			it->setPath(path_);
		}
	}
};


int main()
{
	Observed observed;
	Warning warning;
	Error error;
	FatalError fatal_error;

	observed.AddObserver(&warning);
	observed.AddObserver(&error);
	observed.AddObserver(&fatal_error);
	observed.SetPath("errors.log");

	observed.SendMessage(WARNING, "Some warning 1");
	observed.SendMessage(ERROR, "Some error 1");
	observed.SendMessage(FATAL_ERROR, "Some fatal_error 1");

	observed.RemoveObserver(&warning);

	observed.SendMessage(WARNING, "Some warning 2");
	observed.SendMessage(ERROR, "Some error 2");
	observed.SendMessage(FATAL_ERROR, "Some fatal_error 2");

	system("pause");
}

/*

Задание 2. Паттерн «Наблюдатель»
Развиваем идею с логированием и пишем класс, за которым смогут наблюдать различные наблюдатели и реагировать на те или иные события.

В задании нужно реализовать несколько классов:

Наблюдаемый класс с методами:
void warning(const std::string& message) const;
void error(const std::string& message) const;
void fatalError(const std::string& message) const;
Этот объект должен:

уметь добавлять не владеющие указатели на наблюдателей,
оповещать наблюдателей о вызове каждого из методов,
корректно обрабатывать разрушение объектов-наблюдателей.
Далее нужно реализовать небольшую иерархию классов-наблюдателей, реализующих интерфейс:
class Observer {
public:
	virtual void onWarning(const std::string& message) {}
	virtual void onError(const std::string& message) {}
	virtual void onFatalError(const std::string& message) {}
};
В иерархии классов-наблюдателей нужно реализовать:

класс для работы с предупреждениями, который будет печатать сообщение в консоль;
класс для работы с ошибками, который будет печатать сообщение в файл по указанному пути;
класс для работы с фатальными ошибками, который будет печатать сообщение в консоль и в файл по указанному пути.

*/