#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <errno.h>
#include <Windows.h>

using namespace std;

struct Goods
{
	char name[100];
	double price;
	double del_time;
};

struct Company
{
	char name[100];
	double amount;
};

struct Product
{
	Goods goods;
	Company company;
};

void Create(char* fname);
void Print(char* fname);
void AddProd(char* fname, const char* gname);
void EditProd(char* fname);
void DelProd(char* fname, const char* gname);
void EditFile(char* fname, const char* gname);
bool FiltrDays(char* fname, string towar);
void FiltrPrice(char* fname);
void SortDelivery(char* fname);

int main()
{
	SetConsoleCP(1251); // встановлення сторінки win-cp1251 в потік вводу
	SetConsoleOutputCP(1251); // встановлення сторінки win-cp1251 в потік виводу
	char fname[61];
	char gname[61];
	char ch;
	string towar;
	do
	{
		cout << endl;
		cout << "Головне меню:	" << endl;
		cout << "[1] - введення та збереження данних;" << endl;
		cout << "[2] - загрузка та вивід данних;" << endl;
		cout << "[3] - редагування данних" << endl << endl;
		cout << "[4] - фірми, які поставлять заданий товар за 2 дні" << endl;
		cout << "[5] - фільтрація по цінам в заданій фірмі" << endl;
		cout << "[6] - сортування за терміном поставки товарів" << endl << endl;

		cout << "[0] - завершення роботи." << endl << endl;
		cout << "Ваш вибір: "; cin >> ch;
		switch (ch)
		{
		case '0':
			break;
		case '1':
			cin.get(); // очищуємо буфер клавіатури – щоб не було символу
			cin.sync(); // "кінець рядка", який залишився після вводу числа
			cout << endl;
			cout << "Введіть назву файлу: "; cin.getline(fname, sizeof(fname));
			Create(fname);
			break;
		case '2':
			cin.get(); // очищуємо буфер клавіатури – щоб не було символу
			cin.sync(); // "кінець рядка", який залишився після вводу числа
			cout << endl;
			cout << "Введіть назву файлу: "; cin.getline(fname, sizeof(fname));
			Print(fname);
			break;
		case '3':
			cin.get(); // очищуємо буфер клавіатури – щоб не було символу
			cin.sync(); // "кінець рядка", який залишився після вводу числа
			cout << endl;
			cout << "Введіть назву файлу: "; cin.getline(fname, sizeof(fname));
			EditFile(fname, "helper.dat");
			cout << endl;
			break;
		case '4':
			cin.get(); // очищуємо буфер клавіатури – щоб не було символу
			cin.sync(); // "кінець рядка", який залишився після вводу числа
			cout << endl;
			cout << "Введіть назву файлу: "; cin.getline(fname, sizeof(fname));
			cout << endl;
			cout << "Будь ласка, введіть назву товару: "; cin.sync(); getline(cin, towar);
			cout << endl;
			FiltrDays(fname, towar);
			cout << endl;
			break;
		case '5':
			cin.get(); // очищуємо буфер клавіатури – щоб не було символу
			cin.sync(); // "кінець рядка", який залишився після вводу числа
			cout << endl;
			cout << "Введіть назву файлу: "; cin.getline(fname, sizeof(fname));
			FiltrPrice(fname);
			cout << endl;
			break;
		case '6':
			cin.get(); // очищуємо буфер клавіатури – щоб не було символу
			cin.sync(); // "кінець рядка", який залишився після вводу числа
			cout << endl;
			cout << "Введіть назву файлу: "; cin.getline(fname, sizeof(fname));
			SortDelivery(fname);
			cout << endl;
			break;
		default:
			cout << "Помилка вводу! ";
		}
	} while (ch != '0');
	return 0;
}

void Create(char* fname) // створення файлу з введених рядків
{
	ofstream fout(fname, ios::binary); // відкрили файл для запису
	char ch; // відповідь користувача – чи продовжувати введення
	Product prod;
	do
	{
		cout << "Назва товару: "; cin >> prod.goods.name;
		cout << "Назва компанії постачальника: "; cin >> prod.company.name;
		cout << "Ціна(UAH): "; cin >> prod.goods.price;
		cout << "Кількість товару на складі: "; cin >> prod.company.amount;
		cout << "Терміни доставки(в днях): "; cin >> prod.goods.del_time;
		if (!fout.write((char*)&prod, sizeof(Product)))
		{
			cerr << "Помилка запису у файл." << endl;
		}
		cout << "Продовжити? (y/n): "; cin >> ch;
	} while (ch == 'y' || ch == 'Y');
	cout << endl;
	fout.close();
}

void Print(char* fname) // виведення файлу на екран
{
	ifstream fin(fname, ios::binary); // відкрили файл для зчитування
	if (fin.fail())
	{
		cout << "Помилка відкриття файлу '" << fname << "'!" << endl;
		return;
	}
	cout << endl;
	Product prod;
	cout << "========================================================================="
		<< endl;
	cout << "|  №  |  Товар  |  Компанія  |  Ціна  |  Кількість  |  Термін доставки  |"
		<< endl;
	cout << "-------------------------------------------------------------------------"
		<< endl;
	int i = 0;
	while (fin.read((char*)&prod, sizeof(Product)))
	{
		cout << "| " << setw(2) << right << i + 1 << "  "; i++;
		cout << "|   " << setw(7) << left << prod.goods.name
			<< "|   " << setw(6) << left << prod.company.name << "  "
			<< "|   " << setw(5) << left << prod.goods.price
			<< setw(4) << "|     " << setw(6) << left << prod.company.amount << "  "
			<< setw(6) << "|       " << setw(12) << left << prod.goods.del_time << "|";
		cout << endl;
	}
	cout << "========================================================================= "
		<< endl;
	cout << endl;
	cout << endl;
	fin.close();
}

void EditFile(char* fname, const char* gname) {
	ifstream f(fname, ios::binary);
	if (!f)
	{
		cerr << "Помилка відкриття файлу '" << fname << "'" << endl;
		f.close();
		return;
	}
	char ch;
	f.seekg(0, ios::end);
	int size = f.tellg();
	size = size / sizeof(Product);
	f.seekg(0, ios::beg);
	cout << endl;
	cout << "Кількість товарів у списку: " << size << endl << endl;
	cout << "[1] - добавити новий продукт" << endl;
	cout << "[2] - редагувати інформацію про продукт" << endl;
	cout << "[3] - вилучити продукт" << endl << endl;

	cout << "[0] - вихід" << endl << endl;
	cout << "Ваш вибір: "; cin >> ch;
	switch (ch) {
	case '0':
		f.close();
		return;
		break;
	case '1':
		AddProd(fname, gname);
		break;

	case '2':
		EditProd(fname);
		break;
	case '3':
		DelProd(fname, gname);
		break;
	default:
		cout << "Помилка вводу! " << endl;
	}

}

void AddProd(char* fname, const char* gname) {
	ifstream f(fname, ios::binary);
	f.seekg(0, ios::end);
	int size = f.tellg();
	size = size / sizeof(Product);
	f.seekg(0, ios::beg);
	ofstream g(gname, ios::binary);
	cout << endl;
	Product prod;
	int k = 0;
	while (k < size) {
		f.read((char*)&prod, sizeof(Product));
		g.write((char*)&prod, sizeof(Product));
		k++;
	}
	cout << "Назва товару: "; cin >> prod.goods.name;
	cout << "Компанія - постачальник: "; cin >> prod.company.name;
	cout << "Ціна(UAH): "; cin >> prod.goods.price;
	cout << "Кількість на складі: "; cin >> prod.company.amount;
	cout << "Термін доставки(в днях): "; cin >> prod.goods.del_time;
	if (!g.write((char*)&prod, sizeof(Product)))
	{
		cerr << "Помилка запису у файл." << endl;
	}
	f.close();
	g.close();
	ofstream file(fname, ios::binary);
	ifstream gfile(gname, ios::binary);
	while (gfile.read((char*)&prod, sizeof(Product))) {
		file.write((char*)&prod, sizeof(Product));
	}
	file.close();
	gfile.close();
	cout << endl;
	cout << "Товар успішно додано" << endl;
	cout << endl;
}


void EditProd(char* fname) {
	ifstream f(fname, ios::binary);
	f.seekg(0, ios::end);
	int size = f.tellg();
	size = size / sizeof(Product);
	f.seekg(0, ios::beg);
	Product* prod = new Product[size];
	Product temp;
	f.read((char*)prod, size * sizeof(Product));
	int numb;
	cout << "Номер товару для редагування: "; cin >> numb;
	if (numb > size) {
		cout << endl;
		cout << "Схоже ви ввели неправильний номер товару, будь ласка повторіть спробу з правильним номером товару." << endl << endl;
	}
	else {
		cout << endl;
		cout << "Назва товару: "; cin >> prod[numb - 1].goods.name;
		cout << "Компанія - постачальник: "; cin >> prod[numb - 1].company.name;
		cout << "Ціна(UAH): "; cin >> prod[numb - 1].goods.price;
		cout << "Кількість товару на складі: "; cin >> prod[numb - 1].company.amount;
		cout << "Термін доставки(в днях): "; cin >> prod[numb - 1].goods.del_time;
		cout << endl;
	}
	f.close();
	ofstream file(fname, ios::binary);
	file.write((char*)prod, size * sizeof(Product));
	cout << endl;
	cout << "Данні про продукт успішно змінені." << endl << endl;
	file.close();
}

void DelProd(char* fname, const char* gname) {
	ifstream f(fname, ios::binary);
	f.seekg(0, ios::end);
	int size = f.tellg();
	size = size / sizeof(Product);
	f.seekg(0, ios::beg);
	ofstream g(gname, ios::binary);
	cout << endl;
	int numb;
	cout << "Введіть номер товару, який потрібно видалити: "; cin >> numb;
	Product prod;
	int k = 1;
	while (k <= size) {
		if (k != numb) {
			f.read((char*)&prod, sizeof(Product));
			g.write((char*)&prod, sizeof(Product));
		}
		k++;
	}
	f.close();
	g.close();
	ofstream file(fname, ios::binary);
	ifstream gfile(gname, ios::binary);
	while (gfile.read((char*)&prod, sizeof(Product))) {
		file.write((char*)&prod, sizeof(Product));
	}
	file.close();
	gfile.close();
	cout << endl;
	cout << "Товар успішно видалено" << endl;
	cout << endl;
}

bool FiltrDays(char* fname, string towar) {
	ifstream fin(fname, ios::binary);
	if (!fin)
	{
		cout << "Помилка відкриття файлу '" << fname << "'!" << endl;
		return false;
	}
	cout << endl;
	Product prod;
	int k = 0;
	cout << "========================================================================="
		<< endl; 
	cout << "|  №  |  Товар  |  Компанія  |  Ціна  |  Кількість  |  Термін поставки  |"
		<< endl;
	cout << "-------------------------------------------------------------------------"
		<< endl;
	int i = 0;
	while (fin.read((char*)&prod, sizeof(Product)))
	{
		if (prod.goods.del_time == 2 && prod.goods.name == towar) {
			cout << "| " << setw(2) << right << i + 1 << "  "; i++;
			cout << "|   " << setw(6) << left << prod.goods.name
				<< "|   " << setw(7) << left << prod.company.name << "  "
				<< "|   " << setw(5) << left << prod.goods.price
				<< setw(4) << "|     " << setw(6) << left << prod.company.amount << "  "
				<< setw(6) << "|       " << setw(12) << left << prod.goods.del_time << "|";
			cout << endl;
			k = 1;
		}
	}
	if (k == 0) {
		cout << "Схоже, компаній, які зможуть поставити цей товар в 2 - денний срок не знайдено." << endl;
		return false;
	}
	cout << "========================================================================= "
		<< endl;
	cout << endl;
	cout << endl;
	fin.close();
	return true;
}

void FiltrPrice(char* fname) {
	ifstream fin(fname, ios::binary);
	if (!fin)
	{
		cout << "Помилка відкриття файлу '" << fname << "'!" << endl;
		return;
	}
	cout << endl;
	Product prod;
	string comp;
	cout << "Будь ласка, введіть назву фірми: "; cin.sync(); getline(cin, comp);
	double max_cost;
	cout << endl << endl;
	cout << "Будь ласка, введіть мінімальну ціну на товар: "; cin >> max_cost;
	int k = 0;
	cout << "========================================================================="
		<< endl;
	cout << "|  №  |  Товар  |  Компанія  |  Ціна  |  Кількість  |  Термін поставки  |"
		<< endl;
	cout << "-------------------------------------------------------------------------"
		<< endl;
	int i = 0;
	while (fin.read((char*)&prod, sizeof(Product)))
	{
		if (prod.goods.price <= max_cost && prod.company.name == comp) {
			cout << "| " << setw(2) << right << i + 1 << "  "; i++;
			cout << "|   " << setw(6) << left << prod.goods.name
				<< "|   " << setw(7) << left << prod.company.name << "  "
				<< "|   " << setw(5) << left << prod.goods.price
				<< setw(4) << "|     " << setw(6) << left << prod.company.amount << "  "
				<< setw(6) << "|       " << setw(12) << left << prod.goods.del_time << "|";
			cout << endl;
			k = 1;
		}
	}
	if (k == 0) {
		cout << "Схоже, в данній фірмі таких товарів не знайдено." << endl;
		return;
	}
	cout << "========================================================================= "
		<< endl;
	cout << endl;
	cout << endl;
	fin.close();
	return;
}

void SortDelivery(char* fname) {
	ifstream f(fname, ios::binary);
	if (!f)
	{
		cerr << "Помилка відкриття файлу '" << fname << "'" << endl;
		f.close();
		return;
	}
	f.seekg(0, ios::end);
	int size = f.tellg();
	size = size / sizeof(Product);
	f.seekg(0, ios::beg);
	Product* prod = new Product[size];
	Product temp;
	f.read((char*)prod, size * sizeof(Product));
	for (int i = 0; i < size - 1; i++) { // Сортування бульбашкою
		for (int j = size - 1; j > i; j--) {
			if ((prod[j].goods.del_time < prod[j-1].goods.del_time))
			{
				temp = prod[j];
				prod[j] = prod[j - 1];
				prod[j - 1] = temp;
			}
		}
	}
	f.close();
	ofstream file(fname, ios::binary);
	file.write((char*)prod, size * sizeof(Product));
	file.close();
	cout << endl;
	cout << "Файл відсортовано за зростанням терміну поставки товару." << endl;
}