#pragma once

using namespace System::Drawing;

namespace Polyschyk_Kursova
{
	ref class Triangle
	{
	private:
		Point vA, vB, vC;//координати вершин трикутника
		Point ^vCircle;//координата центра вписаного кола

		float sAB, sBC, sAC;//довжини сторін трикутника
		float rInscribedCircle;//радіус вписаного кола
		float triangleSquare;//площа трикутника
		float halfPerimeter;//півпеример

		bool isIsosceles;//чи рівнобедрений
		bool isObtuseAngled;//чи тупокутний

		void countTriangleSides();//підрахувати довжини сторін
		void countRadiusOfInscribedCircle();//підрахувати радіус вписаного кола
		void countSquare();//підрахувати площу
		void countHalfPerimeter();//підрахувати півпериметр

		void checkIsosceles();//перевірити чи рівнобедрений
		void checkObtuseAngle();//перевірити чи тупокутний
		void countCircleCenter();//обчислити координати центра вписаного кола

		float countVectorAbs(Point pA, Point pB);//підрахувати модуль вектора

		void countProperties();//виклик функцій, які обраховують усі властивості трикутника

	public:
		Triangle(Point a,Point b,Point c);//конструктор з параметрами - вершинами трикутника
		Triangle(void);

		void drawTriangle(System::Windows::Forms::PaintEventArgs^ e);//намалювати трикутник
		void drawInscribedCircle(System::Windows::Forms::PaintEventArgs^ e);//намалювати вписане коло

		bool isEmpty();//перевіряє, чи трикутнику передані значення вершин
		float getSquare();//отримати площу трикутника
		float getRadius();
		Point^ getCenterPoint();
		bool isIsoscelesAndObtuseAngled();//дізнатись, чи трикутник рівнобедрений і тупокутний
	};

}