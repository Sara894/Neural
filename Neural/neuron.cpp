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
	void SetLayers(int n, int* p)//массив size
	{
		srand(time(0));//генерируем рандомные числа
		layers = n;
		neurons = new neuron * [n];



	}

};