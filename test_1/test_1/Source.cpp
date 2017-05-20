#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <ctime>
#include <mutex>
using namespace std;

typedef map<int, vector<int>> Map;
Map factors;
mutex result_mutex;

//struct to help us with the starting point and ending point
struct working
{
	int start;
	int end;
};

//produce and return a vecotr of the randomly generated numbers to find the factors of
vector<int> generator()
{
	int random;
	vector<int> numbers;
	for (int i = 1; i < 100000; i++) //change back to 100000
	{
		random = rand() % 10000; // change back to 10000
		numbers.push_back(random);
	}
	return numbers;
};

//values to use that are random numbers from generator
vector<int> random = generator();

void getFactors(struct working* work)
{
	//give me the value that I will be factoring first
	int value = random[0];
	vector<int> temp;
	
	for (int j = work->start; j < work->end; j++)
	{
		value = random[j]; //change to get the next value to factor depending of where you are starting and ending
		//only need to factor up to half the value since there won't be factors larger than that
		for (int i = 1; i < (value / 2) + 1; i++)
		{
			if (value % i == 0)
			{
				temp.push_back(i);
			}
		}
		{
			//locking this whole section, I think I could possibly only need to lock the factors insert section
			//but just to be safe and prevent race conditoins locked the section for checking the map as well
			lock_guard<mutex> g(result_mutex);
			if (factors.count(value) == 0)
			{
				cout << "not in map so far: \n";
				factors.insert(pair<int, vector<int>>(value, temp));		//this is where the lock will need to go
			}
			else
			{
				cout << "number already exists in map \n";
			}
		}
		//clear temp vector so as to re-populate
		temp.clear();
	}


};

//basically your code here Pete, trying to spawn 500 threads then doing 200 things with each one, finishing by joining all of them
void allFactors()
{
	const int threadCount = 500;
	thread threads[threadCount];
	struct working sws[threadCount];

	for (int i = 0; i < threadCount; i++)
	{
		sws[i].start = i * 200;
		sws[i].end = sws[1].start + 200;
		threads[i] = thread(getFactors, &sws[i]);
	}

	for (int i = 0; i < threadCount; i++)
	{
		threads[i].join();
	}
};


int main()
{
	//call the function to do the work
	allFactors();
	srand(time(NULL));
	Map::iterator it;
	//iterat over the map telling you what the key (number we are facotring) and its values (what the factors are)
	for (it = factors.begin(); it != factors.end(); it++)
	{
		cout << " Key value is: " << it->first;
		cout << " Factors are: ";

		for (int i = 0; i < it->second.size(); i++)
		{
			cout << it->second[i] << " ";
		}
		cout << it->first << endl;
	}

	return 0;
}