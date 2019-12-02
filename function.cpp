#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iterator> 
#include <iomanip>
#include <math.h>
#include "function.h"
#define MAX_EVENTS 20
using namespace std;

Event eventArray[MAX_EVENTS];
int numberOfMonitoredEvents;
double sumOfWeight;
double threshold;

void readEvents(char* fileName)
{
	ifstream eventFile;
	eventFile.open(fileName);

	if(!eventFile.good())
	{
		cerr << "Could not open event fi le." << endl;
	}

	eventFile >> numberOfMonitoredEvents;
	//So getline does not give us a blank line.
	eventFile.ignore(100,'\n');

	string event;
	int counter = 0;
	int indexEvent = 0;
	int indexWeight = 0;

	//Getting the event and weight and assigning to struct attribute
	while(!eventFile.eof())
	{	
		if(counter % 2 == 0)
		{
			getline(eventFile,event,':');
			//Getting rid of the extra line after every 4th event.
			if(event[1] == '\n')
			{
				event[1] = '\0';
			}
			eventArray[indexEvent++].type = event;
			counter++;

		}else{//So we can assign the weight seperately from the event
			getline(eventFile,event,':');
			eventArray[indexWeight++].weight = atof(event.c_str());
			sumOfWeight += atof(event.c_str());
			counter++;
		}
	}

	eventFile.close();
	threshold = calcThreshold(sumOfWeight);
}

void readBaseData(char* fileName)
{
	ifstream baseFile;
	baseFile.open(fileName);

	if(!baseFile.good())
	{
		cerr << "Could not open base file." << endl;
	}

	string eventMeasure;
	int counter = -1;
	int dayCounter = 0;

	while(!baseFile.eof())
	{
		getline(baseFile, eventMeasure,':');
		
		counter++;	
		//For indexing
		if(counter != numberOfMonitoredEvents)
		{
			//Keeping track of the measures to then find mean and stdev
			eventArray[counter].measure.push_back(atoi(eventMeasure.c_str()));
			//cout << eventMeasure << endl;
		}else{
			//Go back to the start index.
			counter = 0;
			eventArray[counter].measure.push_back(atoi(eventMeasure.c_str()));
			dayCounter++;
			//cout << eventMeasure << endl;
		}
		
	}

	calcAvg(dayCounter);
	calcStd(dayCounter);

	//Part 1
	printBase();
	baseFile.close();
}


void readTestEvents(char* fileName)
{
	ifstream testFile;
	testFile.open(fileName);

	if(!testFile.good())
	{
		cerr << "Could not open test file." << endl;
		exit(1);
	}

	string eventMeasure;
	int counter = -1;
	int dayCounter = 0;

	while(!testFile.eof())
	{
		getline(testFile, eventMeasure,':');
		
		counter++;	
		//For indexing
		if(counter != numberOfMonitoredEvents)
		{
			//Keeping track of the measures to then find mean and stdev
			eventArray[counter].testMeasure.push_back(atoi(eventMeasure.c_str()));
		}else{
			//Go back to the start index.
			counter = 0;
			eventArray[counter].testMeasure.push_back(atoi(eventMeasure.c_str()));
			dayCounter++;
		}
		
	}

	// Part 3
	double Totaldistance = 0;
	cout << "\nChecking For Intrusions..." << endl;
	for(int j = 0; j < dayCounter; j++)
    {
    	for(int i = 0; i < numberOfMonitoredEvents; i++)
    	{
    		cout << eventArray[i].testMeasure[j] << ":";
    	}

    	cout << endl;
    }

    //Loop per day
    for(int j = 0; j < dayCounter; j++)
    {
  
  		//Start fresh on new day
  		bool alarm = false;
    	double distance;
    	double distanceForDay = 0;
    	cout << "\n***************Day: " << j+1 << "***************" << endl;

    	//Loop per event in each day
    	for(int i = 0; i < numberOfMonitoredEvents; i++)
    	{	
    		// Formula weight * ((amount - avg) / standard deviation)
      		distance = (eventArray[i].testMeasure[j] - eventArray[i].avg) / eventArray[i].stdDeviation;
    		if(distance < 0)
    		{
    			//Making it positive
    			distance *= (-1);
    		}
    		distance *= eventArray[i].weight;

    		cout << endl;
    		cout << eventArray[i].type << ": " << "Contributions " << distance << endl;
    		distanceForDay += distance;
    		cout << "Accumulated Contribution: " << distanceForDay << endl;
    	}

    	//Checking if it will break the threshold.
		if(distanceForDay >= threshold)
		{
			alarm = true;
		}
		
		//Converting to positive value
		double finalDistance = (distanceForDay - threshold);
		if(finalDistance < 0)
		{
			finalDistance *= (-1);
		}

		if(alarm)
    	{
    		cout << "\n----------Conclusion----------" << endl;
    		cout << "\nThreshold: " << threshold << endl;
    		cout << "Total Distance: " << distanceForDay << endl;
			//cout << "Distance: " << finalDistance << endl;
    		cout << "Alarm: Yes" << endl;
    		cout << "------------------------------" << endl;
    		cout << endl;
    	}else {
    		cout << "\n----------Conclusion----------" << endl;
    		cout << "\nThreshold: " << threshold << endl;
    		cout << "Total Distance: " << distanceForDay << endl;
			//cout << "Distance: " << finalDistance << endl;
    		cout << "Alarm: No" << endl;
    		cout << "------------------------------" << endl;
    		cout << endl;
    	}
    }

	testFile.close();
}


double calcThreshold(double sum)
{
	return 2 * sum;
}

//Using Mean Formula
void calcAvg(int days)
{

	for(int i = 0; i < numberOfMonitoredEvents; i++)
	{
		for(int j = 0; j < days; j++)
		{
			eventArray[i].avg += eventArray[i].measure[j];
		}

		eventArray[i].avg /= days;
	}
}

//Using standard Deviation Formula
void calcStd(int days)
{
    float standardDeviation = 0;
    for(int i = 0; i < numberOfMonitoredEvents; i++)
    {
    	for(int j = 0; j < days; j++)
    	{
			// Sample Standard Deviation
    		standardDeviation += pow(eventArray[i].measure[j] - eventArray[i].avg, 2);
    		eventArray[i].stdDeviation = sqrt(standardDeviation/(days - 1)); 
    	}
    	//Reset
    	standardDeviation = 0;
    }

}

void printBase()
{
	//Output for part 1
	cout << "\nGenerating Base Report..." << endl;
	for(int i = 0; i < numberOfMonitoredEvents;i++)
    {
    	cout << eventArray[i].type << endl;
    	cout << "Weight: " << eventArray[i].weight << endl;
    	cout << "Average: " << eventArray[i].avg << endl;

    	// Adding 0.005 to stdDev for rounding to 2 Decimal Places.
    	eventArray[i].stdDeviation += 0.005;
    	cout << fixed;
    	cout << setprecision(2);
   		cout << "Standard Deviation: " << eventArray[i].stdDeviation << " (Rounded 2 Decimal Places)" << endl;
   		// Setting it back to its correct value after print.
   		eventArray[i].stdDeviation -= 0.005;
   		cout << endl;
    }
	
    cout << "Threshold: " << threshold << endl;
}
