 #include <fstream>
#include <thread>//сетка многопоточная будет
#include <random>//случайные числа
#include <time.h>
#include <Windows.h>//библиша для вывода на экран
#include <iostream>

using namespace std;

//эмулируем нейрон
struct neuron
{
	double value;
	double error;
	void act()//функция активации
	{
		value = 1 / (1 + pow(2.71828, -value));
	}
};

class network
{
public:
	int layers;//количество слоев нейронки
	neuron** neurons;//двумерный массив нейронов по сути
	double*** weights;//трехмерный массив весов
	//Почему 3мерный 1 разряд -- номер слоя
	//2 ой номер нейрона
	//3 -- номер связи нейрона со следующим слоем
	int* size;//динамический массив,
	//содержит колво нейронов в каждом слое
	int theadsNum;//количество потоков
	//производная сигмоиды
	double sigm_pro(double x)
	{
		if ((fabs(x - 1) < 1e-9) || (fabs(x) < 1e-9)) return 0.0;
		double res = x * (1.0 - x);
		return res;

	}
	//предсказание
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

	//одна из самых важных функций, SetLayers
	void setLayers(int n, int* p)//массив size
	{
		srand(time(0));//генерируем рандомные числа
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
	
	//принимает входные значения нейросети
	void setInput(double* p)
	{
		for (int i = 0; i < size[0]; i++)//значения от 0 до 255 - градация серого
		{
			neurons[0][i].value = p[i];
		}
	}

	//вспомогательная функция
	void Show()
	{
		setlocale(LC_ALL, "ru");
		cout << "Количество ядер процессора: " << endl;
		cout << "Нейронная сеть имеет архитектуру: "
	}

};