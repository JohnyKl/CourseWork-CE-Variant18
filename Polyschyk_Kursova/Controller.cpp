#include "stdafx.h"
#include "Controller.h"

using namespace System::Windows::Forms;
using namespace System;
using namespace Polyschyk_Kursova;

Controller::Controller(std::string coordFilePath)//����������� ����� � ��������� ������ �� ����� � ������������ � ����� ���������
{
	bTriangle = gcnew Triangle(); 
	lTriangle = gcnew Triangle();

	isIntersection = false;

	prepareController(coordFilePath);
}

void Controller::readCoordFromFile(std::string path)//�������, �� ����� ���������� � �����
{
	std::ifstream ifs(path); //������� �������� ����

	std::vector<int*> coords;//�������� ������, � ����� �������������� ������ �������� ���������

	if(ifs.is_open())//���� �������� ���� �������
	{
		while (! ifs.eof() )//���� �� ��������� ���� �����
		{
			std::string line = "";//���������� �� ����������� ������ ��� ���������� � �����
			std::getline(ifs,line);//��������� ���� ������ � �����
			
			int* coordsFromLine = new int[2];//���������� �� ����������� ������ ��� ���������� �������� ������� ����������
			findCoordsInLine(line, coordsFromLine);//������ ���������� � ������

			if(coordsFromLine[0] != -1 && coordsFromLine[1] != -1)//���� ���������� ��������
			{
				coords.push_back(coordsFromLine);//������ ���������� � ������
			}	
		}

		int pointsNumber = coords.size();//������� ����� ���� ������� ���������

		points = System::Array::CreateInstance(Point::typeid, pointsNumber);//������������ ����� �����

		for (int i = 0; i < pointsNumber; i++)
		{
			points->SetValue(Point(coords[i][0],coords[i][1]),i);//��������� ����� ����� � �������� ���������
		}
	}

	ifs.close();//������� �������� ����
}

void Controller::findCoordsInLine(std::string line, int* coords)
{
	size_t posX = line.find_first_of("=") + 1; //������ ������� ������� ������� ���� ������� ��������� ������� "="
	size_t posY = line.find_last_of("=") + 1;//������ ������� ������� ������� ���� ���������� ��������� ������� "="

	coords[0] = -1;//������������ ���������� ����� ����� ��� ������������� ��������� ��������� ���������
	coords[1] = -1;

	if(posX > 0 && posY > 0)//���� ������� ��������
	{
		size_t countX = line.find_first_of(",") - posX;//������ ������ ������� ��������� ������� "," �� ������ �� ����� ������� ����� ����� ����� ����������, ��� ������ ������� �����
		size_t countY = line.length() - posY;//������ ������ ���������� ��������� ������� "," �� ������ �� ����� ������� ����� ����� ����� ����������, ��� ������ ������� �����		

		if(countX > 0 && countY > 0)//���� ������� ��������
		{
			int x = stoi(line.substr(posX,countX));//����������� � ����� ������� ������� ������ � ������������
			int y = stoi(line.substr(posY,countY));

			coords[0] = x;//��������� ����� ����������
			coords[1] = y;
		}
	}
}

void Controller::createTriangles()//�������� �� ������ ����������
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

				if(!a.Equals(b) && !b.Equals(c) && !a.Equals(c))//�������� �� �� ������������ ���� � �� ���� ����� ���� �� ��� �������
				{
					triangles->SetValue(gcnew Triangle(a,b,c),trianglesIndex);//������ ����� ���������

					trianglesIndex++;//�������� �������� ������� ��������� ����������
				}
			}
		}
	}
	
	resizeTrianglesArray(trianglesIndex);//������ ����� ������ ���������� �� �������, ���� �������� ����� ����������, ��� ������������� ����� ������� ����� � �.�.
}

void Controller::resizeTrianglesArray(int size)
{
	System::Array ^trianglesDump = triangles;//�������� �������� �� �������� ����� ����������

	triangles = System::Array::CreateInstance(Triangle::typeid, size);//�������������� ����� ������� ��������� ������

	for(int i = 0; i < size; i++)
	{
		triangles->SetValue((Triangle^)trianglesDump->GetValue(i),i);//��������� ����� ����� � �����������
	}
}

void Controller::findTheBiggerAndTheLeast()//������ ������������ ���������� ��������� � ��������� ������
{
	bTriangle = (Triangle^)triangles->GetValue(0);//������������ ��������� �� ��������� �� ������ ������ ��������� � ����� ����������
	lTriangle = bTriangle;
	
	for(int i = 1; i < triangles->Length; i++)
	{
		Triangle^ trg = (Triangle^)triangles->GetValue(i);//�������� �������� �� ��������� � ������

		if(trg->isIsoscelesAndObtuseAngled())//���� �� ���������� ������������ ���������
		{
			if(trg->getSquare() > bTriangle->getSquare())//���� ����� ����� �� ����� ��������� "����������"
			{
				bTriangle = trg;
			}
			if(trg->getSquare() < lTriangle->getSquare())//���� ����� ����� �� ����� ��������� "����������"
			{
				lTriangle = trg;
			}
		}		
	}
}

void Controller::prepareController(std::string path)//���������� ���������
{
	readCoordFromFile(path);
	createTriangles();
	findTheBiggerAndTheLeast();
	checkIntersection();
}

void Controller::drawTwoTriangles(System::Windows::Forms::PaintEventArgs^ e)//�������, ��� ����� ��� ���������� � ��������� � ��������� ������
{
	bTriangle->drawTriangle(e);
	lTriangle->drawTriangle(e);
}

void Controller::drawPoints(System::Windows::Forms::PaintEventArgs^ e)//�������, ��� ����� �� �����, ������ � �����
{
	Pen^ pen = gcnew Pen( Color::Green,4.0f);

	for(int i = 0; i < points->Length; i++)
	{
		Point point = (Point)points->GetValue(i);

		e->Graphics->DrawEllipse(pen, (int)(point.X - 1), (int)(point.Y - 1), 2, 2);
	}
}

void Controller::drawTwoInscribedCircles(System::Windows::Forms::PaintEventArgs^ e)//�������, ��� ����� ��� ������ � ���������� ����� ���������� ����
{
	bTriangle->drawInscribedCircle(e);
	lTriangle->drawInscribedCircle(e);
}

void Controller::showMessage()//������ ����������� ��� ��, �� ������������� ������ ����
{
	if(!lTriangle->isEmpty() && !bTriangle->isEmpty())
	{
		if(isIntersection) 
		{		
			MessageBox::Show("������ ���� �������������","������� ��",MessageBoxButtons::OK,MessageBoxIcon::Information);
		}
		else
		{
			MessageBox::Show("������ ���� �� �������������","������� ��",MessageBoxButtons::OK,MessageBoxIcon::Information);
		}
	}
	
}

void Controller::checkIntersection()//���������, �� ������������� ������ ����
{
	Point^ centerB = bTriangle->getCenterPoint();
	Point^ centerL = lTriangle->getCenterPoint();

	int len = System::Math::Sqrt((centerB->X - centerL->X)*(centerB->X - centerL->X) + (centerB->Y - centerL->Y)*(centerB->Y - centerL->Y));//���������� ������� �� �������� ��

	if(len <= bTriangle->getRadius() + lTriangle->getRadius() && len > bTriangle->getRadius() - lTriangle->getRadius())//���� �������������, ���� ������� �� �� ������� ����������� �� ����� �� ������� �� ������
	{
		isIntersection = true;
	}	
}

