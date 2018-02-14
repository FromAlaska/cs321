// Jim Samson
// Homework #2 cs321

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <thread>
using std::thread;
#include <vector>
using std::vector;
#include <mutex>
using std::mutex;
#include <string>
using std::string;

#include "sa2a.h"

#define NUM_THREADS 6

// Global variables
int counter=0;
vector<thread> threads(NUM_THREADS);
vector<int> input, output, processID;
mutex threadHandler;

void workerThread() {
  while(!counter) {
    continue;
  }
  while(true) {
    threadHandler.lock();
    if(input.size() == 0) {
      threadHandler.unlock();
      return;
    }
    int buffer = input[input.size()-1];
    input.pop_back();
    threadHandler.unlock();
    int hashValue = sa2a(buffer);
    threadHandler.lock();
    processID.push_back(buffer);
    output.push_back(hashValue);
    threadHandler.unlock();
  }
}

int main() {
  for (int index = 0; index < NUM_THREADS; index++) {
		threads[index] = thread(workerThread);
	}

	do {
		cout << "Enter a positive integer (or 0 to end input): ";
    string stringInput;
		std::getline(cin, stringInput);
		if (stringInput.length() > 0)
			input.push_back(std::stoi(stringInput));

	} while(input.back() > 0);

	input.pop_back();
	threadHandler.lock();
	counter = input.size();
	threadHandler.unlock();

	int numberOfPrints = 0;
	while(true) {
		threadHandler.lock();
		if (output.size() == 0) {
			threadHandler.unlock();
			continue;
		}

		cout << "sa2a(" << processID[processID.size()-1]<<") = " << output[output.size()-1] << endl;
		numberOfPrints++;
		processID.pop_back();
		output.pop_back();

		if (counter == numberOfPrints){
			threadHandler.unlock();
			break;
		}
		threadHandler.unlock();
	}

	for (int index=0; index< NUM_THREADS; index++) {
		threads[index].join();
  }
  return 0;
}
