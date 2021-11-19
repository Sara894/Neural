#include <fstream>
#include <thread>//����� ������������� �����
#include <random>//��������� �����
#include <time.h>
#include <Windows.h>//������� ��� ������ �� �����
#include <iostream>

using namespace std;

//��������� ������
struct neuron
{
	double value;
	double error;
	void act()//������� ���������
	{
		value = 1 / (1 + pow(2.71828, -value));
	}
};

class network
{
public:
	int layers;//���������� ����� ��������
	neuron** neurons;//��������� ������ �������� �� ����
	double*** weights;//���������� ������ �����
	//������ 3������ 1 ������ -- ����� ����
	//2 �� ����� �������
	//3 -- ����� ����� ������� �� ��������� �����
	int* size;//������������ ������,
	//�������� ����� �������� � ������ ����
	int theadsNum;//���������� �������
	//����������� ��������
	double sigm_pro(double x)
	{
		if ((fabs(x - 1) < 1e-9) || (fabs(x) < 1e-9)) return 0.0;
		double res = x * (1.0 - x);
		return res;

	}
	//������������
	double predict(double x)
	{
		if (x >= 0.8)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	//���� �� ����� ������ �������, SetLayers
	void SetLayers(int n, int* p)//������ size
	{
		srand(time(0));//���������� ��������� �����
		layers = n;
		neurons = new neuron * [n];



	}

};