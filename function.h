#ifndef FUNCTION_H
#define FUNCTION_H
#include <vector>
using std::string;
using std::vector;

struct Event
{
	string type;
	double weight;
	vector<double> measure;
	double avg;
	double stdDeviation;
	vector<int> testMeasure;
};
void readEvents(char* fileName);
void readBaseData(char* fileName);
void readTestEvents(char* fileName);
void calcAvg(int days);
void calcStd(int days);
double calcThreshold(double sum);
void printBase();

#endif
