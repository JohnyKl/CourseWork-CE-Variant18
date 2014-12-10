#pragma once

#include "Triangle.h"

namespace Polyschyk_Kursova
{
	ref class Controller
	{
	private:	
		System::Array ^points;//����� ���������, �������� � �����
		System::Array ^triangles;//����� ��� �������� ����������

		Triangle ^bTriangle, ^lTriangle;

		bool isIntersection;//������, �� ������������� ������ � ��� ���������� ����
		
		void findCoordsInLine(std::string line, int* coords);
		void readCoordFromFile(std::string path);//�������, �� ����� ���������� � �����
		void createTriangles();//�������� �� ������ ����������
		void findTheBiggerAndTheLeast();//������ ������������ ���������� ��������� � ��������� � ��������� ������

		void prepareController(std::string path);//���������� ���������
		void checkIntersection();//���������, �� ������������� ������ ����
		void resizeTrianglesArray(int size);

	public:
		Controller(std::string coordFilePath);//����������� ����� � ��������� ������ �� ����� � ������������ � ����� ���������

		void drawTwoTriangles(System::Windows::Forms::PaintEventArgs^ e);//�������, ��� ����� ��� ���������� � ��������� � ��������� ������
		void drawPoints(System::Windows::Forms::PaintEventArgs^ e);//�������, ��� ����� �� �����, ������ � �����
		void drawTwoInscribedCircles(System::Windows::Forms::PaintEventArgs^ e);//�������, ��� ����� ��� ������ � ���������� ����� ���������� ����
		void showMessage();//������ ����������� ��� ��, �� ������������� ������ ����
	};

}