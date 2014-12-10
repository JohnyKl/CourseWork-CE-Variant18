#include "stdafx.h"
#include "Controller.h"

using namespace System::Windows::Forms;
using namespace System;
using namespace Polyschyk_Kursova;

Controller::Controller(std::string coordFilePath)//конструктор класу з переданим шляхом до файлу з координатами в якості параметра
{
	bTriangle = gcnew Triangle(); 
	lTriangle = gcnew Triangle();

	isIntersection = false;

	prepareController(coordFilePath);
}

void Controller::readCoordFromFile(std::string path)//функція, що зчитує координати з файлу
{
	std::ifstream ifs(path); //відкрити файловий потік

	std::vector<int*> coords;//створити вектор, в якому зберігатимуться зчитані значення координат

	if(ifs.is_open())//якщо файловий потік відкрито
	{
		while (! ifs.eof() )//поки не досягнуто кінця файла
		{
			std::string line = "";//оголошення та ініціалізація стрічки для зчитування з файлу
			std::getline(ifs,line);//отримання однієї стрічки з файлу
			
			int* coordsFromLine = new int[2];//оголошення та ініціалізація масиву для збереження зчитаних значень координати
			findCoordsInLine(line, coordsFromLine);//знайти координати в стрічці

			if(coordsFromLine[0] != -1 && coordsFromLine[1] != -1)//якщо координату знайдено
			{
				coords.push_back(coordsFromLine);//додати координату в вектор
			}	
		}

		int pointsNumber = coords.size();//кількість точок рівна кількості координат

		points = System::Array::CreateInstance(Point::typeid, pointsNumber);//ініціалізувати масив точок

		for (int i = 0; i < pointsNumber; i++)
		{
			points->SetValue(Point(coords[i][0],coords[i][1]),i);//заповнити масив точок з зчитаних координат
		}
	}

	ifs.close();//закрити файловий потік
}

void Controller::findCoordsInLine(std::string line, int* coords)
{
	size_t posX = line.find_first_of("=") + 1; //знайти позицію першого символа після першого входження символа "="
	size_t posY = line.find_last_of("=") + 1;//знайти позицію першого символа після останнього входження символа "="

	coords[0] = -1;//ініціалізувати значеннями масив точок для ідентифікації відсутності знайдених координат
	coords[1] = -1;

	if(posX > 0 && posY > 0)//якщо символи знайдено
	{
		size_t countX = line.find_first_of(",") - posX;//знайти індекс першого входження символу "," та відняти від нього позицію першої цифри числа координати, щоб знайти довжину числа
		size_t countY = line.length() - posY;//знайти індекс останнього входження символу "," та відняти від нього позицію першої цифри числа координати, щоб знайти довжину числа		

		if(countX > 0 && countY > 0)//якщо символи знайдено
		{
			int x = stoi(line.substr(posX,countX));//перетворити в число потрібну частину стрічки з координатами
			int y = stoi(line.substr(posY,countY));

			coords[0] = x;//заповнити масив значеннями
			coords[1] = y;
		}
	}
}

void Controller::createTriangles()//створити усі можливі трикутники
{	
	int pointsCount = points->Length;
	triangles = System::Array::CreateInstance(Triangle::typeid, pointsCount*(pointsCount - 1)*(pointsCount - 2));

	int trianglesIndex = 0;

	for(int i = 0; i < points->Length - 2; i++)
	{
		for(int j = i + 1; j < points->Length - 1; j++)
		{
			for(int k = j + 1; k < points->Length; k++)
			{
				Point a = (Point)points->GetValue(i);
				Point b = (Point)points->GetValue(j);
				Point c = (Point)points->GetValue(k);

				if(!a.Equals(b) && !b.Equals(c) && !a.Equals(c))//перевірка чи не повторюється одна і та сама точка двічі як різні вершини
				{
					triangles->SetValue(gcnew Triangle(a,b,c),trianglesIndex);//додати новий трикутник

					trianglesIndex++;//збільшити лічильник кількості створених трикутників
				}
			}
		}
	}
	
	resizeTrianglesArray(trianglesIndex);//змінити розмір масиву трикутників на випадок, якщо створено менше трикутників, чим передбачалось через повтори точок і т.д.
}

void Controller::resizeTrianglesArray(int size)
{
	System::Array ^trianglesDump = triangles;//зберегти вказівник на поточний масив трикутників

	triangles = System::Array::CreateInstance(Triangle::typeid, size);//проініціалізувати новим масивом потрібного розміру

	for(int i = 0; i < size; i++)
	{
		triangles->SetValue((Triangle^)trianglesDump->GetValue(i),i);//заповнити новий масив зі збереженого
	}
}

void Controller::findTheBiggerAndTheLeast()//знайти рівнобедрений тупокутний трикутник з найбільшою площею
{
	bTriangle = (Triangle^)triangles->GetValue(0);//ініціалізувати найбільший та найменший за площею першим значенням в масиві трикутників
	lTriangle = bTriangle;
	
	for(int i = 1; i < triangles->Length; i++)
	{
		Triangle^ trg = (Triangle^)triangles->GetValue(i);//отримати вказівник на трикутник з масиву

		if(trg->isIsoscelesAndObtuseAngled())//якщо це тупокутний рівнобедрений трикутник
		{
			if(trg->getSquare() > bTriangle->getSquare())//якщо площа більша за площу поточного "найбільшого"
			{
				bTriangle = trg;
			}
			if(trg->getSquare() < lTriangle->getSquare())//якщо площа менша за площу поточного "найменшого"
			{
				lTriangle = trg;
			}
		}		
	}
}

void Controller::prepareController(std::string path)//підготувати контролер
{
	readCoordFromFile(path);
	createTriangles();
	findTheBiggerAndTheLeast();
	checkIntersection();
}

void Controller::drawTwoTriangles(System::Windows::Forms::PaintEventArgs^ e)//функція, яка малює два трикутника з найбільшою і найменшою площею
{
	bTriangle->drawTriangle(e);
	lTriangle->drawTriangle(e);
}

void Controller::drawPoints(System::Windows::Forms::PaintEventArgs^ e)//функція, яка малює усі точки, зчитані з файла
{
	Pen^ pen = gcnew Pen( Color::Green,4.0f);

	for(int i = 0; i < points->Length; i++)
	{
		Point point = (Point)points->GetValue(i);

		e->Graphics->DrawEllipse(pen, (int)(point.X - 1), (int)(point.Y - 1), 2, 2);
	}
}

void Controller::drawTwoInscribedCircles(System::Windows::Forms::PaintEventArgs^ e)//функція, яка малює два вписані в намальовані раніше трикутники кола
{
	bTriangle->drawInscribedCircle(e);
	lTriangle->drawInscribedCircle(e);
}

void Controller::showMessage()//показує повідомлення про те, чи перетинаються вписані кола
{
	if(!lTriangle->isEmpty() && !bTriangle->isEmpty())
	{
		if(isIntersection) 
		{		
			MessageBox::Show("Вписані кола перетинаються","Перетин кіл",MessageBoxButtons::OK,MessageBoxIcon::Information);
		}
		else
		{
			MessageBox::Show("Вписані кола не перетинаються","Перетин кіл",MessageBoxButtons::OK,MessageBoxIcon::Information);
		}
	}
	
}

void Controller::checkIntersection()//перевірити, чи перетинаються вписані кола
{
	Point^ centerB = bTriangle->getCenterPoint();
	Point^ centerL = lTriangle->getCenterPoint();

	int len = System::Math::Sqrt((centerB->X - centerL->X)*(centerB->X - centerL->X) + (centerB->Y - centerL->Y)*(centerB->Y - centerL->Y));//обрахувати відстань між центрами кіл

	if(len <= bTriangle->getRadius() + lTriangle->getRadius() && len > bTriangle->getRadius() - lTriangle->getRadius())//кола перетинаються, якщо відстань між їх точками знаходиться між сумою та різницею їх радіусів
	{
		isIntersection = true;
	}	
}

