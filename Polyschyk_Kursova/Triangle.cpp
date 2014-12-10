#include "stdafx.h"
#include "Triangle.h"

using namespace Polyschyk_Kursova;

Triangle::Triangle(void)
{
	vA = Point();
	vB = Point();
	vC = Point();
}

Triangle::Triangle(Point a,Point b,Point c)
{
	vA = a;
	vB = b;
	vC = c;

	sAB = 0.0f;
	sBC = 0.0f;
	sAC = 0.0f;

	rInscribedCircle = 0.0f;//радіус вписаного кола
	triangleSquare = 0.0f;//площа трикутника
	halfPerimeter = 0.0f;//півпеример

	isIsosceles = false;//чи рівнобедрений
	isObtuseAngled = false;//чи тупокутний

	countProperties();
}

bool Triangle::isEmpty()//якщо одночасно всі координати рівні 0, функція повертає true, що значить, що трикутник пустий
{
	bool isEmptyA = vA.X == 0 && vA.Y == 0;
	bool isEmptyB = vB.X == 0 && vB.Y == 0;
	bool isEmptyC = vC.X == 0 && vC.Y == 0;

	return isEmptyA && isEmptyB && isEmptyC;
}

void Triangle::countProperties()//виклик функцій, які обраховують усі властивості трикутника
{
	countTriangleSides();
	countHalfPerimeter();
	countSquare();
	countRadiusOfInscribedCircle();
	countCircleCenter();

	checkIsosceles();
	checkObtuseAngle();
}

void Triangle::countTriangleSides()//підрахувати довжини сторін
{
	sAB = countVectorAbs(vA, vB);
	sBC = countVectorAbs(vB, vC);
	sAC = countVectorAbs(vA, vC);
}

void Triangle::countRadiusOfInscribedCircle()//підрахувати радіус вписаного кола
{
	rInscribedCircle = triangleSquare/halfPerimeter;
}

void Triangle::countSquare()//підрахувати площу
{
	triangleSquare = System::Math::Sqrt(halfPerimeter*(halfPerimeter - sAB)*(halfPerimeter - sBC)*(halfPerimeter - sAC));
}

void Triangle::countHalfPerimeter()//підрахувати півпериметр
{
	halfPerimeter = (sAB + sBC + sAC)/2;
}

void Triangle::checkIsosceles()//перевірити чи рівнобедрений
{
	isIsosceles = sAB.Equals(sBC) || sBC.Equals(sAC) || sAC.Equals(sAB);
}

void Triangle::countCircleCenter()//обчислити координати центра вписаного кола
{
	float xC = (sBC*vA.X + sAC*vB.X + sAB*vC.X)/(sBC + sAC + sAB);
	float yC = (sBC*vA.Y + sAC*vB.Y + sAB*vC.Y)/(sBC + sAC + sAB);

	vCircle = gcnew Point(xC, yC);
}

void Triangle::checkObtuseAngle()//перевірити чи тупокутний
{
	float sqrAB = sAB*sAB;
	float sqrBC = sBC*sBC;
	float sqrAC = sAC*sAC;

	isObtuseAngled = sqrAB > (sqrAC + sqrBC) || sqrAC > (sqrAB + sqrBC) || sqrBC > (sqrAC + sqrAB);//трикутник тупокутний, якщо квадрат однієї сторони більший за суму квадратів двох інших
}

float Triangle::countVectorAbs(Point pA, Point pB)//підрахувати модуль вектора для знаходження його довжини
{
	float sqrX = (pB.X - pA.X)*(pB.X - pA.X);
	float sqrY = (pB.Y - pA.Y)*(pB.Y - pA.Y);

	return System::Math::Sqrt(sqrX + sqrY);
}

void Triangle::drawTriangle(System::Windows::Forms::PaintEventArgs^ e)//намалювати трикутник
{
	Pen^ blackPen = gcnew Pen( Color::Black,3.0f);

	e->Graphics->DrawLine(blackPen, vA, vB);//сторона АВ	
	e->Graphics->DrawLine(blackPen, vC, vB);//сторона ВС
	e->Graphics->DrawLine(blackPen, vA, vC);//сторона АС
}

void Triangle::drawInscribedCircle(System::Windows::Forms::PaintEventArgs^ e)//намалювати вписане коло 
{
	Pen^ bluePen = gcnew Pen(Color::Blue,2.0f);

	float x = vCircle->X - rInscribedCircle;
	float y = vCircle->Y - rInscribedCircle;

	e->Graphics->DrawEllipse(bluePen,x, y, 2*rInscribedCircle, 2*rInscribedCircle);
}

float Triangle::getSquare()//отримати площу трикутника
{
	return triangleSquare;
}

bool Triangle::isIsoscelesAndObtuseAngled()//дізнатись, чи трикутник рівнобедрений і тупокутний
{
	return isIsosceles && isObtuseAngled;
}

float Triangle::getRadius()//отримати радіус вписаного кола
{
	return rInscribedCircle;
}

Point^ Triangle::getCenterPoint()//отримати координати точкицентра кола
{
	return vCircle;
}