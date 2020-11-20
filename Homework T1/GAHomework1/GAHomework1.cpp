// GAHomework1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#pragma warning(disable: 4996)
#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <fstream>
#include <thread>
#include <sstream>
#include <string>
#include <climits>
#include <random>
#include <chrono>

#define PI 3.14159265358979
#define M 10

double a = -500, b = 500;	//lower and upper limit of interval
int prec = 5;				//precision
unsigned int dimensions;	//no. of dimension
int dimLength;				//length of one dimension

double dejong(std::vector<double>& vec)
{
	double result = 0;
	for (double d : vec)
	{
		result += (d * d);
	}
	return result;
}

double schwefel(std::vector<double>& vec)
{
	double result = 0;
	for (double d : vec)
	{
		result += (-d * sin(sqrt(abs(d))));
	}
	return result;
}

double rastrigin(std::vector<double>& vec)
{
	double result = 10.0 * vec.size();
	for (double d : vec)
	{
		result += (pow(d, 2) - 10 * cos(2.0 * PI * d));
	}

	return result;
}

double michalewicz(std::vector<double>& vec)
{
	double result = 0;
	int size = vec.size();
	for (int i = 1; i <= size; ++i)
	{
		result += (sin(vec[i - 1]) * pow(sin( (i * pow(vec[i - 1], 2)) / PI ), 2 * M));
	}

	result *= -1;
	return result;
}

int length(double a, double b, int prec)
{
	return ceil(log((b - a) * pow(10, prec)) / log(2));
}

double decodeDimension(std::vector<char>::iterator itStart, std::vector<char>::iterator itEnd, int dimLength, double a, double b)
{
	//coordonata intr-o dimensiune.
	unsigned long bi = 0;
	for (auto it = itStart; it != itEnd; ++it)
	{
		bi *= 2;
		bi += *it;
	}

	double s = bi / (pow(2, dimLength) - 1);
	return s * (b - a) + a;
}

std::vector<double> decode(std::vector<char>& bits, int dimLength, unsigned int dimensions, double a, double b)
{
	std::vector<double> returnVec;
	std::vector<char>::iterator itStart, itEnd;
	for (int i = 0; i < dimensions; ++i)
	{
		itStart = bits.begin() + i * dimLength;
		itEnd = itStart + dimLength;

		double value = decodeDimension(itStart, itEnd, dimLength, a, b);
		returnVec.push_back(value);
	}

	return returnVec;
}

double hillClimbingBest(std::vector<char>& bits,unsigned int dimensions,int dimLength,int domainSize,double a,double b,std::function<double(std::vector<double>&)> func)
{
	auto decodedConfig = decode(bits, dimLength, dimensions, a, b);
	double minValue = func(decodedConfig);    // presupunem ca configuratia curenta is the best solution
	bool local;
	int closerToLocal;
	do {
		local = false;
		double initialMin = minValue;

		for (int i = 0; i < domainSize; ++i)
		{
			bits[i] = !bits[i];

			decodedConfig = decode(bits, dimLength, dimensions, a, b);
			double tempValue = func(decodedConfig);

			if (tempValue < minValue)
			{
				minValue = tempValue;
				closerToLocal = i;
			}

			bits[i] = !bits[i];
		}

		if (minValue == initialMin)
		{
			local = true;
		}
		else
		{
			// ne transformam configuratia primita intr-una mai apropiata de minimul local;
			// bits devine vecinul apropiat de minimul local
			bits[closerToLocal] = !bits[closerToLocal];
		}

	} while (!local);

	return minValue;
}

double hillClimbingFirst(std::vector<char>& bits, unsigned int dimensions, int dimLength, int domainSize, double a, double b, std::function<double(std::vector<double>&)> func)
{
	auto decodedConfig = decode(bits, dimLength, dimensions, a, b);
	double minValue = func(decodedConfig);
	bool local;
	do {
		local = false;
		double initialValue = minValue;

		for (int i = 0; i < domainSize; ++i)
		{
			bits[i] = !bits[i];

			decodedConfig = decode(bits, dimLength, dimensions, a, b);
			double tempValue = func(decodedConfig);

			if (tempValue < minValue)
			{
				minValue = tempValue;
				break;
			}

			bits[i] = !bits[i];
		}

		if (initialValue == minValue)
		{
			local = true;
		}

	} while (!local);

	return minValue;
}

void simulatedAnnealing(unsigned int dimensions,int dimLength,int domainSize,double a,double b,std::function<double(std::vector<double>&)> func, std::ofstream& fout)
{
	clock_t startTime, endTime;
	startTime = clock();

	std::vector<char> bits;
	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937_64 mt_rand(seed);

	// generating random initial best solution
	for (int i = 0; i < domainSize; ++i)
	{
		bits.push_back(mt_rand() % 2);
	}

	auto decodedConfig = decode(bits, dimLength, dimensions, a, b);
	double minValue = func(decodedConfig);

	double temperature = 150;
	while (temperature > 0.0000001)
	{
		for (int i = 0; i < 10000; ++i)
		{
			//selecting random neighbour;
			int randomNeighbour = mt_rand() % domainSize;

			bits[randomNeighbour] = !bits[randomNeighbour];

			decodedConfig = decode(bits, dimLength, dimensions, a, b);
			double compared = func(decodedConfig);

			if (compared < minValue)
			{
				minValue = compared;
			}
			else
			{
				std::uniform_real_distribution<double> subUnitValueGen(0.0, 1.0);
				double subUnitValue = subUnitValueGen(mt_rand);

				if (subUnitValue < exp(-1.0 * abs(compared - minValue) / temperature) )
				{
					minValue = compared;
				}
				else
				{
					bits[randomNeighbour] = !bits[randomNeighbour];
				}
			}
		}

		temperature *= 0.995;

	}

	endTime = clock();
	fout << "\nGlobal minimum: " << minValue << "\nPoint:\n(";
	decodedConfig = decode(bits, dimLength, dimensions, a, b);
	for (int i = 0; i < dimensions - 1; ++i)
	{
		fout << decodedConfig[i] << ", ";
	}
	fout << decodedConfig[dimensions - 1] << ")\n\n";
	fout << "Time elapsed: " << ((double)endTime - startTime) / CLOCKS_PER_SEC << " seconds.\n\n";
	fout.close();
}

int getThreadID(std::thread::id id)
{
	std::stringstream read;
	read << id;
	return stoi(read.str());
}

void iteratedHillClimbingBest(int dimensions,std::function<double(std::vector<double>&)> func, clock_t beginning, const char* functionName)
{
	clock_t startTime, endTime;
	startTime = clock();
	int id = getThreadID(std::this_thread::get_id());
	// generate random seed;

	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937_64 mt_rand(seed);

	std::string fileName = functionName;
	fileName += "/HC_BEST/";
	fileName += std::to_string(dimensions);
	fileName += "_Thread_";
	fileName += std::to_string(id);
	fileName += "_" + std::to_string(mt_rand());
	fileName += ".txt";

	std::ofstream fout(fileName);
	fout << "Thread id: " << id << "\n Dimensions: " << dimensions << '\n';
	int domainSize = dimLength * dimensions;
	double minValue = (double)INT_MAX;
	int minIteration = 0;
	
	std::vector<char> bits;
	std::vector<double> components;

	for (int iteration = 0; iteration < 10000; ++iteration)
	{
		bits.clear();
		// randomizeaza bits;
		for (int i = 0; i < domainSize; ++i)
		{
			bits.push_back(mt_rand() % 2);
		}

		double minCandidate = hillClimbingBest(bits, dimensions,dimLength, domainSize, a, b, func);
		if (minCandidate < minValue)
		{
			minValue = minCandidate;
			minIteration = iteration;
			components = decode(bits, dimLength, dimensions, a, b);
		}
	}

	fout << "\n\nGlobal minimum: " << minValue << "\nFound at iteration: " << minIteration << "\nPoint: ";
	int compSize = components.size();
	fout << "(";
	for (int i = 0; i < compSize - 1; ++i)
	{
		fout << components[i] << ", ";
	}
	fout << components[compSize - 1] << ")";

	endTime = clock();
	fout << "Time elapsed: " << ((double)endTime - startTime) / CLOCKS_PER_SEC << " seconds.\n";
	fout.close();
}

void iteratedHillClimbingFirst(int dimensions, std::function<double(std::vector<double>&)> func, clock_t beginning, const char* functionName)
{
	clock_t startTime, endTime;
	startTime = clock();
	int id = getThreadID(std::this_thread::get_id());
	// generate random seed;

	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937_64 mt_rand(seed);

	std::string fileName = functionName;
	fileName += "/HC_FIRST/";
	fileName += std::to_string(dimensions);
	fileName += "_Thread_";
	fileName += std::to_string(id);
	fileName += "_" + std::to_string(mt_rand());
	fileName += ".txt";

	std::ofstream fout(fileName);
	fout << "Thread id: " << id << "\n Dimensions: " << dimensions << '\n';
	int domainSize = dimLength * dimensions;
	double minValue = (double)INT_MAX;
	int minIteration = 0;

	std::vector<char> bits;
	std::vector<double> components;

	for (int iteration = 0; iteration < 10000; ++iteration)
	{
		bits.clear();
		// randomizeaza bits;
		for (int i = 0; i < domainSize; ++i)
		{
			bits.push_back(mt_rand() % 2);
		}

		double minCandidate = hillClimbingFirst(bits, dimensions, dimLength, domainSize, a, b, func);
		if (minCandidate < minValue)
		{
			minValue = minCandidate;
			minIteration = iteration;
			components = decode(bits, dimLength, dimensions, a, b);
		}
	}

	fout << "\n\nGlobal minimum: " << minValue << "\nFound at iteration: " << minIteration << "\nPoint: ";
	int compSize = components.size();
	fout << "(";
	for (int i = 0; i < compSize - 1; ++i)
	{
		fout << components[i] << ", ";
	}
	fout << components[compSize - 1] << ")";

	endTime = clock();
	fout << "Time elapsed: " << ((double)endTime - startTime) / CLOCKS_PER_SEC << " seconds.\n";
	fout.close();
}

void callingSimulatedAnnealing(int dimensions, std::function<double(std::vector<double>&)> func, clock_t beginning, const char* functionName)
{
	int id = getThreadID(std::this_thread::get_id());
	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937_64 mt_rand(seed);

	std::string fileName = functionName;
	fileName += "/SA/";
	fileName += std::to_string(dimensions);
	fileName += "_Thread_";
	fileName += std::to_string(id);
	fileName += "_" + std::to_string(mt_rand());
	fileName += ".txt";

	std::ofstream fout(fileName);
	fout << "Thread id: " << id << "\nDimensions: " << dimensions << "\n\n";
	int domainSize = dimensions * dimLength;
	
	simulatedAnnealing(dimensions, dimLength, domainSize, a, b, func, fout);
	fout.close();
}

void doTheThing(double lowerLimit, double upperLimit, int precision, std::function<double(std::vector<double>&)> func, const char* functionName)
{
	a = lowerLimit;
	b = upperLimit;
	prec = precision;
	dimLength = length(a, b, prec);

	clock_t startTime, endTime;
	
	// HC_BEST
	startTime = clock();
	std::cout << "Fac " << functionName;
	std::cout << "\nHC_BEST pe 5 dimensiuni.\n\n";
	{
		for (int i = 0; i < 3; ++i)
		{
			std::thread t1(iteratedHillClimbingBest, 5, func, startTime, functionName);
			std::thread t2(iteratedHillClimbingBest, 5, func, startTime, functionName);
			std::thread t3(iteratedHillClimbingBest, 5, func, startTime, functionName);
			std::thread t4(iteratedHillClimbingBest, 5, func, startTime, functionName);
			std::thread t5(iteratedHillClimbingBest, 5, func, startTime, functionName);
			std::thread t6(iteratedHillClimbingBest, 5, func, startTime, functionName);
			std::thread t7(iteratedHillClimbingBest, 5, func, startTime, functionName);
			std::thread t8(iteratedHillClimbingBest, 5, func, startTime, functionName);
			std::thread t9(iteratedHillClimbingBest, 5, func, startTime, functionName);
			std::thread t10(iteratedHillClimbingBest, 5, func, startTime, functionName);

			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			t6.join();
			t7.join();
			t8.join();
			t9.join();
			t10.join();

			std::cout << "Finished iteration " << i << "\n\n";
		}

		std::cout << "Fac " << functionName;
		std::cout << "\nHC_BEST pe 10 dimensiuni.\n\n";
		for (int i = 0; i < 3; ++i)
		{
			std::thread t1(iteratedHillClimbingBest, 10, func, startTime, functionName);
			std::thread t2(iteratedHillClimbingBest, 10, func, startTime, functionName);
			std::thread t3(iteratedHillClimbingBest, 10, func, startTime, functionName);
			std::thread t4(iteratedHillClimbingBest, 10, func, startTime, functionName);
			std::thread t5(iteratedHillClimbingBest, 10, func, startTime, functionName);
			std::thread t6(iteratedHillClimbingBest, 10, func, startTime, functionName);
			std::thread t7(iteratedHillClimbingBest, 10, func, startTime, functionName);
			std::thread t8(iteratedHillClimbingBest, 10, func, startTime, functionName);
			std::thread t9(iteratedHillClimbingBest, 10, func, startTime, functionName);
			std::thread t10(iteratedHillClimbingBest, 10, func, startTime, functionName);

			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			t6.join();
			t7.join();
			t8.join();
			t9.join();
			t10.join();

			std::cout << "Finished iteration " << i << "\n\n";
		}

		std::cout << "Fac " << functionName;
		std::cout << "\nHC_BEST pe 30 dimensiuni.\n\n";
		for (int i = 0; i < 3; ++i)
		{
			std::thread t1(iteratedHillClimbingBest, 30, func, startTime, functionName);
			std::thread t2(iteratedHillClimbingBest, 30, func, startTime, functionName);
			std::thread t3(iteratedHillClimbingBest, 30, func, startTime, functionName);
			std::thread t4(iteratedHillClimbingBest, 30, func, startTime, functionName);
			std::thread t5(iteratedHillClimbingBest, 30, func, startTime, functionName);
			std::thread t6(iteratedHillClimbingBest, 30, func, startTime, functionName);
			std::thread t7(iteratedHillClimbingBest, 30, func, startTime, functionName);
			std::thread t8(iteratedHillClimbingBest, 30, func, startTime, functionName);
			std::thread t9(iteratedHillClimbingBest, 30, func, startTime, functionName);
			std::thread t10(iteratedHillClimbingBest, 30, func, startTime, functionName);

			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			t6.join();
			t7.join();
			t8.join();
			t9.join();
			t10.join();

			std::cout << "Finished iteration " << i << "\n\n";
		}
		endTime = clock();
		std::cout << functionName << " : Hill Climbing BEST improvement took " << ((double)endTime - startTime) / CLOCKS_PER_SEC << " seconds.\n\n";

	}

	// HC_FIRST
	startTime = clock();
	std::cout << "Fac " << functionName;
	std::cout << "\nHC_FIRST pe 5 dimensiuni.\n\n";
	{
		for (int i = 0; i < 3; ++i)
		{
			std::thread t1(iteratedHillClimbingFirst, 5, func, startTime, functionName);
			std::thread t2(iteratedHillClimbingFirst, 5, func, startTime, functionName);
			std::thread t3(iteratedHillClimbingFirst, 5, func, startTime, functionName);
			std::thread t4(iteratedHillClimbingFirst, 5, func, startTime, functionName);
			std::thread t5(iteratedHillClimbingFirst, 5, func, startTime, functionName);
			std::thread t6(iteratedHillClimbingFirst, 5, func, startTime, functionName);
			std::thread t7(iteratedHillClimbingFirst, 5, func, startTime, functionName);
			std::thread t8(iteratedHillClimbingFirst, 5, func, startTime, functionName);
			std::thread t9(iteratedHillClimbingFirst, 5, func, startTime, functionName);
			std::thread t10(iteratedHillClimbingFirst, 5, func, startTime, functionName);

			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			t6.join();
			t7.join();
			t8.join();
			t9.join();
			t10.join();

			std::cout << "Finished iteration " << i << "\n\n";
		}

		std::cout << "Fac " << functionName;
		std::cout << "\nHC_FIRST pe 10 dimensiuni.\n\n";
		for (int i = 0; i < 3; ++i)
		{
			std::thread t1(iteratedHillClimbingFirst, 10, func, startTime, functionName);
			std::thread t2(iteratedHillClimbingFirst, 10, func, startTime, functionName);
			std::thread t3(iteratedHillClimbingFirst, 10, func, startTime, functionName);
			std::thread t4(iteratedHillClimbingFirst, 10, func, startTime, functionName);
			std::thread t5(iteratedHillClimbingFirst, 10, func, startTime, functionName);
			std::thread t6(iteratedHillClimbingFirst, 10, func, startTime, functionName);
			std::thread t7(iteratedHillClimbingFirst, 10, func, startTime, functionName);
			std::thread t8(iteratedHillClimbingFirst, 10, func, startTime, functionName);
			std::thread t9(iteratedHillClimbingFirst, 10, func, startTime, functionName);
			std::thread t10(iteratedHillClimbingFirst, 10, func, startTime, functionName);

			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			t6.join();
			t7.join();
			t8.join();
			t9.join();
			t10.join();

			std::cout << "Finished iteration " << i << "\n\n";
		}

		std::cout << "Fac " << functionName;
		std::cout << "\nHC_FIRST pe 30 dimensiuni.\n\n";
		for (int i = 0; i < 3; ++i)
		{
			std::thread t1(iteratedHillClimbingFirst, 30, func, startTime, functionName);
			std::thread t2(iteratedHillClimbingFirst, 30, func, startTime, functionName);
			std::thread t3(iteratedHillClimbingFirst, 30, func, startTime, functionName);
			std::thread t4(iteratedHillClimbingFirst, 30, func, startTime, functionName);
			std::thread t5(iteratedHillClimbingFirst, 30, func, startTime, functionName);
			std::thread t6(iteratedHillClimbingFirst, 30, func, startTime, functionName);
			std::thread t7(iteratedHillClimbingFirst, 30, func, startTime, functionName);
			std::thread t8(iteratedHillClimbingFirst, 30, func, startTime, functionName);
			std::thread t9(iteratedHillClimbingFirst, 30, func, startTime, functionName);
			std::thread t10(iteratedHillClimbingFirst, 30, func, startTime, functionName);

			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			t6.join();
			t7.join();
			t8.join();
			t9.join();
			t10.join();

			std::cout << "Finished iteration " << i << "\n\n";
		}

		endTime = clock();
		std::cout << functionName << " Hill Climbing FIRST improvement took " << ((double)endTime - startTime) / CLOCKS_PER_SEC << " seconds.\n\n\n";

	}
	
	//SIMULATED ANNEALING;
	startTime = clock();
	std::cout << "Fac " << functionName << "\n";
	{
		std::cout << "Simulated Annealing pe 5 dimensiuni.\n\n";
		for (int i = 0; i < 3; ++i)
		{
			// callingSimulatedAnnealing(int dimensions, std::function<double(std::vector<double>&)> func, clock_t beginning, const char* functionName)
			std::thread t1(callingSimulatedAnnealing, 5, func, startTime, functionName);
			std::thread t2(callingSimulatedAnnealing, 5, func, startTime, functionName);
			std::thread t3(callingSimulatedAnnealing, 5, func, startTime, functionName);
			std::thread t4(callingSimulatedAnnealing, 5, func, startTime, functionName);
			std::thread t5(callingSimulatedAnnealing, 5, func, startTime, functionName);
			std::thread t6(callingSimulatedAnnealing, 5, func, startTime, functionName);
			std::thread t7(callingSimulatedAnnealing, 5, func, startTime, functionName);
			std::thread t8(callingSimulatedAnnealing, 5, func, startTime, functionName);
			std::thread t9(callingSimulatedAnnealing, 5, func, startTime, functionName);
			std::thread t10(callingSimulatedAnnealing, 5, func, startTime, functionName);

			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			t6.join();
			t7.join();
			t8.join();
			t9.join();
			t10.join();

			std::cout << "Finished iteration " << i << "\n\n";

		}

		
		std::cout << "Fac " << functionName << "\n";
		std::cout << "Simulated Annealing pe 10 dimensiuni.\n\n";
		for (int i = 0; i < 3; ++i)
		{
			// callingSimulatedAnnealing(int dimensions, std::function<double(std::vector<double>&)> func, clock_t beginning, const char* functionName)
			std::thread t1(callingSimulatedAnnealing, 10, func, startTime, functionName);
			std::thread t2(callingSimulatedAnnealing, 10, func, startTime, functionName);
			std::thread t3(callingSimulatedAnnealing, 10, func, startTime, functionName);
			std::thread t4(callingSimulatedAnnealing, 10, func, startTime, functionName);
			std::thread t5(callingSimulatedAnnealing, 10, func, startTime, functionName);
			std::thread t6(callingSimulatedAnnealing, 10, func, startTime, functionName);
			std::thread t7(callingSimulatedAnnealing, 10, func, startTime, functionName);
			std::thread t8(callingSimulatedAnnealing, 10, func, startTime, functionName);
			std::thread t9(callingSimulatedAnnealing, 10, func, startTime, functionName);
			std::thread t10(callingSimulatedAnnealing, 10, func, startTime, functionName);

			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			t6.join();
			t7.join();
			t8.join();
			t9.join();
			t10.join();

			std::cout << "Finished iteration " << i << "\n\n";

		}

		std::cout << "Fac " << functionName << "\n";
		std::cout << "Simulated Annealing pe 30 dimensiuni.\n\n";
		for (int i = 0; i < 3; ++i)
		{
			// callingSimulatedAnnealing(int dimensions, std::function<double(std::vector<double>&)> func, clock_t beginning, const char* functionName)
			std::thread t1(callingSimulatedAnnealing, 30, func, startTime, functionName);
			std::thread t2(callingSimulatedAnnealing, 30, func, startTime, functionName);
			std::thread t3(callingSimulatedAnnealing, 30, func, startTime, functionName);
			std::thread t4(callingSimulatedAnnealing, 30, func, startTime, functionName);
			std::thread t5(callingSimulatedAnnealing, 30, func, startTime, functionName);
			std::thread t6(callingSimulatedAnnealing, 30, func, startTime, functionName);
			std::thread t7(callingSimulatedAnnealing, 30, func, startTime, functionName);
			std::thread t8(callingSimulatedAnnealing, 30, func, startTime, functionName);
			std::thread t9(callingSimulatedAnnealing, 30, func, startTime, functionName);
			std::thread t10(callingSimulatedAnnealing, 30, func, startTime, functionName);

			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			t6.join();
			t7.join();
			t8.join();
			t9.join();
			t10.join();

			std::cout << "Finished iteration " << i << "\n\n";

		}

		endTime = clock();
		std::cout << functionName << " Simulated Annealing took " << ((double)endTime - startTime) / CLOCKS_PER_SEC << " seconds.\n\n\n";
	}
}


int main()
{
	// De Jong
	doTheThing(-5.12, 5.12, 5, dejong, "DeJong");

	// Schwefel
	doTheThing(-500.0, 500.0, 5, schwefel, "Schwefel");

	//Rastrigin
	doTheThing(-5.12, 5.12, 5, rastrigin, "Rastrigin");

	// Michalewicz
	doTheThing(0.0, PI, 5, michalewicz, "Michalewicz");

	int ceva;
	std::cin >> ceva;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
