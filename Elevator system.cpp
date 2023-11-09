#include <iostream>
#include <cmath>	//для нахождения модуля 
#include <thread>	//для задержки консоли
#include <vector>

class house
{
private:
	int nbfloor;	//кол-во этажей
	int nbel;	//кол-во лифтов
public:
	const int getnbfl() { return nbfloor; }
	const int getnbel() { return nbel; }
	void getdata()
	{
		while (true)	//проверка на "дурака"
		{
			std::cout << "Write number of floors - >0, <21 : "; std::cin >> nbfloor;
			std::cout << "Write number of elevators - >0, <7 : "; std::cin >> nbel;
			if (std::cin.good() && nbfloor < 21 && nbfloor>0 && nbel > 0 && nbel < 7)
				break;
			std::cin.clear();
			system("cls");
			std::cout << "Numbers must be integers, in the written range!\n";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  //первый аргумент - введенные данные
		}
	}
};
house h;
class elevator	//сам лифт
{
private:
	int pos = 1;
	char goal[21];	//нажата ли кнопка на этаже и какая
	bool wanted[21] = {};	//выбран ли этот этаж как цель
	char condition = 's';	//текущее состояние лифта, u - дв. вверх, d -  дв. вниз
public:
	elevator()
	{
		for (int n = 0; n < 21; n++)
			goal[n] = 'n';
	}
	void newgoal(int a, char ch)	//начать движение при новом человеке
	{
		goal[a] = ch;
		if (condition == 's')
			if (a > pos) condition = 'u';
			else condition = 'd';
	}
	void newwanted(int a)	//начать движение при новой выбранной пассажиром цели
	{
		wanted[a] = true;
		if (condition == 's')
			if (a > pos) condition = 'u';
			else condition = 'd';
	}
	void draw(int a)	//нарисовать текущую строчку для этого лифта(есть ли он сам там, или его цели)
	{
		if (goal[a] == 'u')
		{
			std::cout << "  " << (char)30;	//стрелка вверх
			if (pos == a) std::cout << (char)219 << (char)219;	//лифт
			else std::cout << "  ";
			if (wanted[a])
				std::cout << (char)4;	//значёк, что сюда нужно пассажиру
		}
		if (goal[a] == 'd')
		{
			std::cout << "  " << (char)31;	//стрелка вниз
			if (pos == a) std::cout << (char)219 << (char)219;	//лифт
			else std::cout << "  ";
			if (wanted[a])
				std::cout << (char)4;	//значёк, что сюда нужно пассажиру
		}
		if (pos == a && goal[a] == 'n')
		{
			if (wanted[a]) std::cout << "   " << (char)219 << (char)219 << (char)4;	//лифт и значёк, что сюда нужно пассажиру
			else std::cout << "   " << (char)219 << (char)219;	//лифт
		}
		else if (goal[a] == 'n')
		{
			if (wanted[a])
				std::cout << "     " << (char)4;	//только значёк
			else std::cout << "     ";
		}
	}
	const int getpos() { return pos; }
	const char getdir() { return condition; }
	void move();	//прототип движений за "кадр" в программе
};

std::vector<elevator> el(6);	//массив указателей чтобы можно было создавать новые обьекты в цикле

void draw()		//рисует каждую строчку обращаясь к каждому лифту
{
	for (int n = 0; n < h.getnbfl(); n++)	//для каждого этажа
	{
		if (h.getnbfl() - n < 10) std::cout << " " << h.getnbfl() - n;	//ряд цифр
		else std::cout << h.getnbfl() - n;
		for (int j = 0; j < h.getnbel(); j++)	//функция от каждого лифта
		{
			el[j].draw(h.getnbfl() - n);
		}
		std::cout << "\n";
	}
}
void addpas()	//добавить пассажира
{
	int fl, nr = 20, elev=0;
	char ch;
	while (true)	//проверка на "дурака"
	{
	std::cout << "Write floor: "; std::cin >> fl;
		if (fl != 1 && fl != h.getnbfl()) { std::cout << "Write desired direction 'u' - up, 'd' - down: "; std::cin >> ch; }
		if (fl == 1) { ch = 'u'; break; }
		if (fl == h.getnbfl()) { ch = 'd'; break; }
		if (std::cin.good() && fl <= h.getnbfl() && fl > 0 && (ch == 'u' || ch == 'd'))
			break;
		std::cin.clear();
		system("cls");
		std::cout << "Values must be integer and char, in the written range!\n";
		std::cin.ignore(2, '\n');
	}
	for (int n = 0; n < h.getnbel(); n++)	//находит лифт, который ближе всех к пассажиру и с подходящим направлением движения
	{
		if (abs(el[n].getpos() - fl) < nr && (el[n].getdir() == ch || el[n].getdir() == 's'))
		{
			nr = abs(el[n].getpos() - fl);	//наименьшее расстояние 
			elev = n;	//номер этого лифта
		}
	}
	el[elev].newgoal(fl, ch);	//команда этому лифту
}
void move()	//двигаем каждый лифт
{
	for (int n = 0; n < h.getnbel(); n++)
	{
		el[n].move();
	}
}
void selgoal(elevator* elev)	//выбрать этаж для вошедшего в лифт пассажира
{
	int nb, drfloor;
	for (int n = 0; n < h.getnbel(); n++)	//ищем порядковый номер текущего лифта
		if (elev == &el[n])
			nb = n;
	std::cout << "Elevator " << nb + 1 << " take a man. Write the desired floor: ";
	while (true)		//проверка на "дурака"
	{
		std::cin >> drfloor;
		if (std::cin.good() && drfloor < 21 && drfloor>0)
			break;
		std::cin.clear();
		system("cls");
		std::cout << "Number must be integer, in the written range!\n";
		std::cin.ignore(2, '\n');
		std::cout << "Elevator " << nb + 1 << " take a man. Write the desired floor: ";
	}
	el[nb].newwanted(drfloor);		//добавляем цель движения лифту
}
void elevator::move()	//передвижения за "кадр"
{
	for (int n = 0; n < h.getnbfl() + 1; n++)		//для каждого этажа
	{
		if (condition == 's'&&goal[n] != 'n')	//если лифт стоит, но он где-то вызван, то мы его запускаем
		{
			if (n > pos) condition = 'u';
			else condition = 'd';
		}
	}
	for (int n = 0; n < h.getnbfl() + 1; n++)		//если лифт стоит, но пассажиром нажата кнопка ехать, то мы его запускаем
	{
		if (wanted[n] && goal[n] == 'n' && !wanted[pos] && condition == 's')
		{
			if (n > pos) condition = 'u';
			else condition = 'd';
		}
	}
	switch (condition)		//смотрим, что делает лифт
	{
	case 's': if (goal[pos] != 'n')		//если лифт стоит там где заходит пассажир, запускаем функцию
	{
		goal[pos] = 'n';
		selgoal(this);
	}
			  using namespace std::chrono_literals;
			  if (wanted[pos]) { std::cout << "Passenger is delivered!"; std::this_thread::sleep_for(600ms); wanted[pos] = false; }
			  break;
	case 'd': if (goal[pos] != 'n') { goal[pos] = 'n'; condition = 's'; selgoal(this); }
			  //если лифт заехал туда, где нужно высадить пассажира, останавливаем лифт
			  else pos--;
		if (wanted[pos]) { condition = 's'; }
		break;
	case 'u': if (goal[pos] != 'n') { goal[pos] = 'n'; condition = 's'; selgoal(this); }
			  //если лифт заехал туда, где нужно высадить пассажира, останавливаем лифт
			  else pos++;
		if (wanted[pos]) { condition = 's'; }
		break;
	}
}


int main()
{
	h.getdata();
	char ans;
	do {
		system("cls");
		draw();		//рисуем
		std::cout << "Add new passenger - p"
			<< "\n Wait - w"
			<< "\n Quit - q     ";
		std::cin >> ans;
		if (ans == 'p') { addpas(); continue; }
		if (ans == 'w') move();		//двигаем
	} while (ans != 'q');		//выход при 'q'
	return 0;
}