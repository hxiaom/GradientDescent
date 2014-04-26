#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define DES_TIME 1000
#define DATA_SIZE 10000
#define THETA_NUM 384

double data[DATA_SIZE][385];
double testData[DATA_SIZE][THETA_NUM];
double prediction[DATA_SIZE];

double theta[THETA_NUM];
double tmpTheta[THETA_NUM];

double h[DATA_SIZE];

bool loadTrainData()
{
	string headline;
	char comma;
	int id;

	ifstream ifs("train.csv");
	//除去第一行
	ifs >> headline;
	//读取数据
	for (int i = 0; i < DATA_SIZE; i++) {
		ifs >> id >> comma;
		for (int j = 0; j < 384; j++){
			ifs >> data[i][j] >> comma;
		}
		ifs >> data[i][384];
	}

	return true;
}

bool loadTestData()
{
	string headline;
	char comma;
	int id;

	ifstream ifs_test("test.csv");

	//除去第一行
	ifs_test >> headline;
	//读取数据
	for (int i = 0; i < 10000; i++) {
		ifs_test >> id >> comma;
		for (int j = 0; j < 383; j++){
			ifs_test >> testData[i][j] >> comma;
		}
		ifs_test >> testData[i][383];
	}
	return true;
}

void initTheta()
{
	for (int i = 0; i < THETA_NUM; i++)
		theta[i] = 0.5;
}

bool gradicentDescent()
{
	int counter = 0;
	double loss = 10.0;
	for (int i = 0; i < DES_TIME && loss > 0.01; i++) {
		double error_sum = 0;

		for (int j = 0; j < 10000; j++) {
			h[j] = 0;
			for (int k = 0; k < 384; k++)
				h[j] += data[j][k + 1] * theta[k];

			error_sum = h[j] - data[j][0];
			for (int k = 0; k < 384; k++) {
				theta[k] -= (error_sum) / 10000 * 0.01 * data[j][k + 1];
			}
			//cout << theta[j] << " ";
		}


		double loss = 0;
		for (int j = 0; j < 10000; j++) {
			double sum = 0;
			for (int k = 0; k < 384; k++) {
				sum += data[j][k + 1] * theta[k];
			}
			loss += (sum - data[j][0])*(sum - data[j][0]);
		}
		counter++;
		cout << counter << "\t" << loss << endl;
	}
	return true;
}

void outputTheta()
{
	ofstream ofs("theta.csv");
	for (int i = 0; i < THETA_NUM; i++) {
		ofs << i << "," << theta[i] << endl;
	}
}

void getResult()
{
	double result[10000];

	for (int i = 0; i < 10000; i++) {
		result[i] = 0;
	}

	for (int i = 0; i < 10000; i++) {
		for (int j = 0; j < 384; j++)
			result[i] += testData[i][j] * theta[j];
	}

	ofstream ofs("result.csv");
	ofs << "Id" << "," << "reference" << endl;
	for (int i = 10001; i < 20001; i++) {
		ofs << i << "," << result[i - 10001] << endl;
	}
}

int main()
{

	loadTrainData();

	initTheta();

	gradicentDescent();

	outputTheta();

	loadTestData();

	getResult();

	return 0;
}