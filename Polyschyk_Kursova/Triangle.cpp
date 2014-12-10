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

	rInscribedCircle = 0.0f;//����� ��������� ����
	triangleSquare = 0.0f;//����� ����������
	halfPerimeter = 0.0f;//���������

	isIsosceles = false;//�� ������������
	isObtuseAngled = false;//�� ����������

	countProperties();
}

bool Triangle::isEmpty()//���� ��������� �� ���������� ��� 0, ������� ������� true, �� �������, �� ��������� ������
{
	bool isEmptyA = vA.X == 0 && vA.Y == 0;
	bool isEmptyB = vB.X == 0 && vB.Y == 0;
	bool isEmptyC = vC.X == 0 && vC.Y == 0;

	return isEmptyA && isEmptyB && isEmptyC;
}

void Triangle::countProperties()//������ �������, �� ����������� �� ���������� ����������
{
	countTriangleSides();
	countHalfPerimeter();
	countSquare();
	countRadiusOfInscribedCircle();
	countCircleCenter();

	checkIsosceles();
	checkObtuseAngle();
}

void Triangle::countTriangleSides()//���������� ������� �����
{
	sAB = countVectorAbs(vA, vB);
	sBC = countVectorAbs(vB, vC);
	sAC = countVectorAbs(vA, vC);
}

void Triangle::countRadiusOfInscribedCircle()//���������� ����� ��������� ����
{
	rInscribedCircle = triangleSquare/halfPerimeter;
}

void Triangle::countSquare()//���������� �����
{
	triangleSquare = System::Math::Sqrt(halfPerimeter*(halfPerimeter - sAB)*(halfPerimeter - sBC)*(halfPerimeter - sAC));
}

void Triangle::countHalfPerimeter()//���������� ����������
{
	halfPerimeter = (sAB + sBC + sAC)/2;
}

void Triangle::checkIsosceles()//��������� �� ������������
{
	isIsosceles = sAB.Equals(sBC) || sBC.Equals(sAC) || sAC.Equals(sAB);
}

void Triangle::countCircleCenter()//��������� ���������� ������ ��������� ����
{
	float xC = (sBC*vA.X + sAC*vB.X + sAB*vC.X)/(sBC + sAC + sAB);
	float yC = (sBC*vA.Y + sAC*vB.Y + sAB*vC.Y)/(sBC + sAC + sAB);

	vCircle = gcnew Point(xC, yC);
}

void Triangle::checkObtuseAngle()//��������� �� ����������
{
	float sqrAB = sAB*sAB;
	float sqrBC = sBC*sBC;
	float sqrAC = sAC*sAC;

	isObtuseAngled = sqrAB > (sqrAC + sqrBC) || sqrAC > (sqrAB + sqrBC) || sqrBC > (sqrAC + sqrAB);//��������� ����������, ���� ������� ���� ������� ������ �� ���� �������� ���� �����
}

float Triangle::countVectorAbs(Point pA, Point pB)//���������� ������ ������� ��� ����������� ���� �������
{
	float sqrX = (pB.X - pA.X)*(pB.X - pA.X);
	float sqrY = (pB.Y - pA.Y)*(pB.Y - pA.Y);

	return System::Math::Sqrt(sqrX + sqrY);
}

void Triangle::drawTriangle(System::Windows::Forms::PaintEventArgs^ e)//���������� ���������
{
	Pen^ blackPen = gcnew Pen( Color::Black,3.0f);

	e->Graphics->DrawLine(blackPen, vA, vB);//������� ��	
	e->Graphics->DrawLine(blackPen, vC, vB);//������� ��
	e->Graphics->DrawLine(blackPen, vA, vC);//������� ��
}

void Triangle::drawInscribedCircle(System::Windows::Forms::PaintEventArgs^ e)//���������� ������� ���� 
{
	Pen^ bluePen = gcnew Pen(Color::Blue,2.0f);

	float x = vCircle->X - rInscribedCircle;
	float y = vCircle->Y - rInscribedCircle;

	e->Graphics->DrawEllipse(bluePen,x, y, 2*rInscribedCircle, 2*rInscribedCircle);
}

float Triangle::getSquare()//�������� ����� ����������
{
	return triangleSquare;
}

bool Triangle::isIsoscelesAndObtuseAngled()//��������, �� ��������� ������������ � ����������
{
	return isIsosceles && isObtuseAngled;
}

float Triangle::getRadius()//�������� ����� ��������� ����
{
	return rInscribedCircle;
}

Point^ Triangle::getCenterPoint()//�������� ���������� ����������� ����
{
	return vCircle;
}