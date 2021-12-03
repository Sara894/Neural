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
	void setLayers(int n, int* p)//������ size
	{
		srand(time(0));//���������� ��������� �����
		layers = n;
		neurons = new neuron * [n];
		weights = new double** [n - 1];
		size = new int[n];
		for (int i = 0; i < n; i++)
		{
			size[i] = p[i];
			neurons[i] = new neuron[p[i]];
			if (i < n - 1)
			{
				weights[i] = new double* [p[i]];
				for (int j = 0; j < p[i]; j++)
				{
					weights[i][j] = new double[p[i + 1]];
					for (int k = 0; k < p[i + 1]; k++)
					{
						weights[i][j][k] = ((rand() % 100) * 0.01) / size[i];
					}
				}
			}
		}
	}
	
	//��������� ������� �������� ���������
	void setInput(double* p)
	{
		for (int i = 0; i < size[0]; i++)//�������� �� 0 �� 255 - �������� ������
		{
			neurons[0][i].value = p[i];
		}
	}

	//��������������� �������
	void Show()
	{
		setlocale(LC_ALL, "ru");
		cout << "���������� ���� ����������: " << endl;
		cout << "��������� ���� ����� �����������: "
	}

};