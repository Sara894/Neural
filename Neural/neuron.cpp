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
	int threadsNum;//���������� �������
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
	//��� ����� �������� ��� ������ ������ � ���������
	void Show()
	{
		setlocale(LC_ALL, "ru");
		cout << "���������� ���� ����������: " << thread::hardware_concurrency << endl;
		cout << "��������� ���� ����� �����������: ";
		for (int i = 0; i < layers; i++)
		{
			cout << size[i];
			if (i < layers - 1)
			{
				cout << " - ";
			}
		}
		cout << endl;
		for (int i = 0; i < layers; i++)
		{
			cout << "\n #���� " << i + 1 << "\n\n";
			for (int j = 0; j < size[i]; j++)
			{
				cout << "������ #" << j + 1 << ":\n";
				cout << "��������: " << neurons[i][j].value << endl;
				if (i < layers - 1)
				{
					cout << "���� : \n";
					for (int k = 0; k < size[i + 1]; k++)
					{
						cout << "#" << k + 1 << ":  ";
						cout << weights[i][j][k] << endl;
					}
				}
			}
		}
	}

	//������� �����
	void LayersCleaner(int LayerNumber, int start, int stop)//��� ���������� ����� ����� ������ ������ ������� ������ ���������� ��������
	{
		srand(time(0));
		for (int i = start; i < stop; i++)
		{
			neurons[LayerNumber][i].value = 0;
		}
	}

	//��������������� ������� ��� ������� ForwardFeed
	void ForwardFeeder(int LayerNumber, int start, int stop)
	{
		for (int j = start; j < stop; j++)
		{
			for (int k = 0; k < size[LayerNumber - 1]; k++)
			{
				neurons[LayerNumber][j].value += neurons[LayerNumber - 1][k].value * weights[LayerNumber - 1][k][j];
			}
			neurons[LayerNumber][j].act();
		}
	}

	double ForwardFeed()
	{
		setlocale(LC_ALL, "ru");
		for (int i = 1; i < layers; i++)
		{
			if (threadsNum == 1)
			{
				//���������� ������� ���� ������ �����
				thread th1([&]() {
					LayersCleaner(i, 0, size[i]);
					});
				th1.join();
			}

			if (threadsNum == 2)
			{
				//���������� ������� ���� ������ ����� � ������ �����
				thread th1([&]() {
					LayersCleaner(i, 0, int(floor(size[i] / 2)));
					});
				thread th2([&]() {
					LayersCleaner(i, int(floor(size[i] / 2)), size[i]);
					});
				th1.join();
				th2.join();
			}

			if (threadsNum == 4) {
				if (size[i] == 1)
				{
					cout << "�������� ������� ���� ������ �����...\n";
					thread th1([&]() {
						LayersCleaner(i, 0, size[i]);
						});
					th1.join();
				}

				if (size[i] == 2 || size[i] == 3) {
					cout << "�������� ������� ���� ����� �����...\n";
					thread th1([&]() {
						LayersCleaner(i, 0, size[i]);
						});
					thread th2([&]() {
						LayersCleaner(i, int(floor(size[i] / 2)), size[i]);
						});
					th1.join();
					th2.join();
				}

				if (size[i] >= 4)
				{
					cout << "4 ���� ��� �� ��� ��� ����)))" << endl;
					int start1 = 0;
					int stop1 = int(size[i] / 4);
					int start2 = int(size[i] / 4);
					int stop2 = int(size[i] / 2);
					int start3 = int(size[i] / 2);
					int stop3 = int(size[i] - floor(size[i] / 4));
					int start4 = int(size[i] - floor(size[i] / 4));
					int stop4 = size[i];
					thread th1([&]() { LayersCleaner(i, start1, stop1);  });
					thread th2([&]() { LayersCleaner(i, start2, stop2);  });
					thread th3([&]() { LayersCleaner(i, start3, stop3);  });
					thread th4([&]() { LayersCleaner(i, start4, stop4);  });
					th1.join();
					th2.join();
					th3.join();
					th4.join();
				}

				if (threadsNum == 1) {
					thread th1([&]() {
						ForwardFeeder(i, 0, size[i]);
						});
					th1.join();
				}

				if (threadsNum == 2) {
					thread th1([&]() {
						ForwardFeeder(i, 0, int(floor(size[i] / 2)));
						});
					thread th2([&]() {
						ForwardFeeder(i, int(floor(size[i] / 2)), size[i]);
						});
					th1.join();
					th2.join();
				}

				if (threadsNum == 4)
				{
					if (size[i] == 2 || size[i] == 3) {
						thread th1([&]() {
							ForwardFeeder(i, 0, int(floor(size[i] / 2)));
							});
						thread th2([&]() {
							ForwardFeeder(i, int(floor(size[i] / 2)), size[i]);
							});
						th1.join();
						th2.join();
					}
					if (size[i] >= 4) {
						int start1 = 0;
						int stop1 = int(size[i] / 4);
						int start2 = int(size[i] / 4);
						int stop2 = int(size[i] / 2);
						int start3 = int(size[i] / 2);
						int stop3 = int(size[i] - floor(size[i] / 4));
						int start4 = int(size[i] - floor(size[i] / 4));
						int stop4 = size[i];
						thread th1([&]() { ForwardFeeder(i, start1, stop1);  });
						thread th2([&]() { ForwardFeeder(i, start2, stop2);  });
						thread th3([&]() { ForwardFeeder(i, start3, stop3);  });
						thread th4([&]() { ForwardFeeder(i, start4, stop4);  });
						th1.join();
						th2.join();
						th3.join();
						th4.join();

					}
				}
			}
		}
		double max = 0;
		double prediction = 0;
		for (int i = 0; i < size[layers - 1]; i++) {
			if (neurons[layers - 1][i].value > max) {
				max = neurons[layers - 1][i].value;
				prediction = i;
			}
		}
		return prediction;
	}

	//������� ������ ��� ������� �������
	void ErrorCounter(int LayerNumber, int start, int stop, double prediction, double rresult, double lr)
	{
		if (LayerNumber == layers - 1) {
			for (int j = start; j < stop; j++)
			{
				if (j != int(rresult)) {
					neurons[LayerNumber][j].error = -(neurons[LayerNumber][j].value);
				}
				else {
					neurons[LayerNumber][j].error = 1.0 - (neurons[LayerNumber][j].value);
				}
			}
		}
		else {
			for (int j = start; j < stop; j++) {
				double error = 0.0;
				for (int k = 0; k < size[LayerNumber + 1]; k++) {
					error += neurons[LayerNumber + 1][k].error * weights[LayerNumber][j][k];
				}
				neurons[LayerNumber][j].error = error;
			}
		}
	}

	//��������� ���� �� ������������� �������
	void WeightsUpdater(int start, int stop, int LayerNum, int lr) {
		int i = LayerNum;
		for (int j = start; j < stop; j++) {
			for (int k = 0; k < size[i + 1]; k++) {
				weights[i][j][k] += lr * neurons[i + 1][k].error * sigm_pro(neurons[i + 1][k].value) * neurons[i][j].value;
			}
		}
	}

	//���������� ������� BackPropogation
	void BackPropogation(double prediction, double rresult, double lr) {
		for (int i = layers - 1; i > 0; i--) {
			if (threadsNum == 1) {
				if (i = layers - 1) {
					for (int j = 0; j < size[i]; j++) {
						if (j != int(rresult)) {
							neurons[i][j].error = -(neurons[i][j].value);
						}
						else {
							neurons[i][j].error = 1.0 - (neurons[i][j].value);
						}
					}
				}
				else {
					for (int j = 0; j < size[i]; j++) {
						double error = 0.0;
						for (int k = 0; k < size[i + 1]; k++) {
							error += neurons[i + 1][k].error * weights[i][j][k];
						}
						neurons[i][j].error = error;
					}
				}
			}

			if (threadsNum == 2) {
				thread th1([&]() {
					ErrorCounter(i, 0, int(size[i] / 2), prediction, rresult, lr);
					});
				thread th2([&]() {
					ErrorCounter(i, int(size[i] / 2), size[i], prediction, rresult, lr);
					});
				th1.join();
				th2.join();
			}

			if (threadsNum == 4) {
				if (size[i] < 4) {
					if (i == layers - 1) {
						for (int j = 0; j < size[i]; j++) {
							if (j != int(rresult)) {
								neurons[i][j].error = -(neurons[i][j].value);
							}
							else {
								neurons[i][j].error = 1.0 - (neurons[i][j].value);
							}
						}
					}
					else {
						for (int j = 0; j < size[i]; j++) {
							double error = 0.0;
							for (int k = 0; k < size[i + 1]; k++) {
								error += neurons[i + 1][k].error * weights[i][j][k];
							}
							neurons[i][j].error = error;
						}
					}
				}

				if (size[i] >= 4) {
					int start1 = 0;
					int stop1 = int(size[i] / 4);
					int start2 = int(size[i] / 4);
					int stop2 = int(size[i] / 2);
					int start3 = int(size[i] / 2);
					int stop3 = int(size[i] - floor(size[i] / 4));
					int start4 = int(size[i] - floor(size[i] / 4));
					int stop4 = size[i];
					thread th1([&]() { ErrorCounter(i, start1, stop1, prediction, rresult, lr);  });
					thread th2([&]() { ErrorCounter(i, start2, stop2, prediction, rresult, lr);  });
					thread th3([&]() { ErrorCounter(i, start3, stop3, prediction, rresult, lr);  });
					thread th4([&]() { ErrorCounter(i, start4, stop4, prediction, rresult, lr);  });
					th1.join();
					th2.join();
					th3.join();
					th4.join();
				}
			}
		}

		for (int i = 0; i < layers - 1; i++) {
			if (threadsNum == 1) {
				for (int j = 0; j < size[i]; j++) {
					for (int k = 0; k < size[i + 1]; k++) {
						weights[i][j][k] += lr * neurons[i + 1][k].error * sigm_pro(neurons[i + 1][k].value) * neurons[i][j].value;
					}
				}
			}

			if (threadsNum == 2) {
				thread th1([&]() {
					WeightsUpdater(0, int(size[i] / 2), i, lr);
					});
				thread th2([&]() {
					WeightsUpdater(int(size[i] / 2), size[i], i, lr);
					});
				th1.join();
				th2.join();
			}

			if (threadsNum == 4) {
				if (size[i] < 4) {
					for (int j = 0; j < size[i]; j++) {
						for (int k = 0; k < size[i + 1]; k++) {
							weights[i][j][k] += lr * neurons[i + 1][k].error * sigm_pro(neurons[i + 1][k].value) * neurons[i][j].value;
						}
					}
				}
			}
		}
	}

	bool SaveWeights() {
		ofstream fout;
		fout.open("weights.text");
		for (int i = 0; i < layers; i++) {
			if (i < layers - 1) {
				for (int j = 0; j < size[i]; j++) {
					for (int k = 0; k < size[i + 1]; k++) {
						fout << weights[i][j][k] << " ";

					}
				}
			}
		}

		fout.close();
		return 1;
	}
};


//������� �������

int main() {
	srand(time(0));
	setlocale(LC_ALL, "Russian");
	ifstream fin;
	ofstream fout;
	const int l = 4;//���������� ����� ��������� ����
	const int input_l = 4096;//64*64 ������������ �������� 64 �� 64 
	int size[l] = { input_l, 64, 32, 26 };// 26 ���������� ���� � ���������� ��������

	network nn;//������� ������ ������ ������

	double input[input_l];
	char rresult;// right result
	double result;//����� ������� � ������������ ��������� 
	double ra = 0;// right answer ���������� ���������� ������� �� �����
	int maxra = 0;// max right answer ������������ ���������� ���������� ������� �� �����
	int maxraepoch = 0;
	const int n = 52;//���������� �������� ��� �������� ���������
	bool to_study = 0;//���� � ����� -- ���� ������� ��������� ��� ���?

	cout << "����������� �������� ��� ���? ";
	cin >> to_study;

	double time = 0;

	if (to_study) {
		nn.setLayers(1, size);
		for (int e = 0; ra / n * 100 < 100; e++) {
			fout << "Epoch # " << e << endl;
			double epoch_start = clock();
			ra = 0;
			double w_delta = 0;

			fin.open("lib.txt");

			for (int i = 0; i < n; i++) {
				double start = clock();
				for (int j = 0; j < input_l; j++) {
					fin >> input[j];
				}
				fin >> rresult;
				double stop = clock();
				time += stop - start;
				rresult -= 65;
				nn.setInput(input);

				result = nn.ForwardFeed();
				if (result == rresult) {
					cout << "������ ����� " << char(rresult + 65) << "\t\t\t****" << endl;
					ra++;
				}
				else {
					nn.BackPropogation(result, rresult, 0.6);
				}
			}

			fin.close();
			double epoch_stop = clock();
			cout << "Right answers: " << ra / n * 100 << " % \t Max RA: " << double(maxra) / n * 100 << " ( epoch " << maxraepoch << " ) " << endl;
			cout << "Time needed to fin: " << time / 100 << " ms\t\t\tEpoch time: " << epoch_stop - epoch_start << endl;
			time = 0;

			if (ra > maxra) {
				maxra = ra;
				maxraepoch = e;
			}

			if (maxraepoch < e - 250) {
				maxra = 0;
			}
		}

		if (nn.SaveWeights()) {
			cout << "���� ���������!";
		}
	}






	return 0;

}