#pragma once

#include "Triangle.h"

namespace Polyschyk_Kursova
{
	ref class Controller
	{
	private:	
		System::Array ^points;//масив координат, зчитаних з файлу
		System::Array ^triangles;//масив усіх можливих трикутників

		Triangle ^bTriangle, ^lTriangle;

		bool isIntersection;//показує, чи перетинаються вписані у два трикутники кола
		
		void findCoordsInLine(std::string line, int* coords);
		void readCoordFromFile(std::string path);//функція, що зчитує координати з файлу
		void createTriangles();//створити усі можливі трикутники
		void findTheBiggerAndTheLeast();//знайти рівнобедрений тупокутний трикутник з найбільшою і найменшою площею

		void prepareController(std::string path);//підготувати контролер
		void checkIntersection();//перевірити, чи перетинаються вписані кола
		void resizeTrianglesArray(int size);

	public:
		Controller(std::string coordFilePath);//конструктор класу з переданим шляхом до файлу з координатами в якості параметра

		void drawTwoTriangles(System::Windows::Forms::PaintEventArgs^ e);//функція, яка малює два трикутника з найбільшою і найменшою площею
		void drawPoints(System::Windows::Forms::PaintEventArgs^ e);//функція, яка малює усі точки, зчитані з файла
		void drawTwoInscribedCircles(System::Windows::Forms::PaintEventArgs^ e);//функція, яка малює два вписані в намальовані раніше трикутники кола
		void showMessage();//показує повідомлення про те, чи перетинаються вписані кола
	};

}