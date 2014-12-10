#pragma once

using namespace System::Drawing;

namespace Polyschyk_Kursova
{
	ref class Triangle
	{
	private:
		Point vA, vB, vC;//���������� ������ ����������
		Point ^vCircle;//���������� ������ ��������� ����

		float sAB, sBC, sAC;//������� ����� ����������
		float rInscribedCircle;//����� ��������� ����
		float triangleSquare;//����� ����������
		float halfPerimeter;//���������

		bool isIsosceles;//�� ������������
		bool isObtuseAngled;//�� ����������

		void countTriangleSides();//���������� ������� �����
		void countRadiusOfInscribedCircle();//���������� ����� ��������� ����
		void countSquare();//���������� �����
		void countHalfPerimeter();//���������� ����������

		void checkIsosceles();//��������� �� ������������
		void checkObtuseAngle();//��������� �� ����������
		void countCircleCenter();//��������� ���������� ������ ��������� ����

		float countVectorAbs(Point pA, Point pB);//���������� ������ �������

		void countProperties();//������ �������, �� ����������� �� ���������� ����������

	public:
		Triangle(Point a,Point b,Point c);//����������� � ����������� - ��������� ����������
		Triangle(void);

		void drawTriangle(System::Windows::Forms::PaintEventArgs^ e);//���������� ���������
		void drawInscribedCircle(System::Windows::Forms::PaintEventArgs^ e);//���������� ������� ����

		bool isEmpty();//��������, �� ���������� ������� �������� ������
		float getSquare();//�������� ����� ����������
		float getRadius();
		Point^ getCenterPoint();
		bool isIsoscelesAndObtuseAngled();//��������, �� ��������� ������������ � ����������
	};

}