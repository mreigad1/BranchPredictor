#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#define LINESIZE 3
#define MAX(a,b) ((a > b)?(a):(b))
#define MIN(a,b) ((a < b)?(a):(b))
#define MAX_SIX ((1 << 6) - 1)
#define TESTING true
using namespace std;

int localCounters[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int selector[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int globalCounters[64];
int six_seq = false;
int local_correctness = 0;
int global_correctness = 0;
int selector_correctness = 0;
char localPrediction = 'n';
char globalPrediction = 'n';
char selectorUsed = 'l';
char finalPrediction = 'n';
char actualDirection = 'n';

void updateLocal(int src, int dst, bool taken){
	if(localCounters[src] > 1){	//if predicting taken then...
		localPrediction = 't';
	}else{				//else if not predicting taken...
		localPrediction = 'n';
	}
	if(taken == (localCounters[src] > 1)){	//if correct prediction then...
		local_correctness++;
	}
	if(taken){
		localCounters[src]++;
		localCounters[src] = MIN(localCounters[src], 4);
	}else{
		localCounters[src]--;
		localCounters[src] = MAX(localCounters[src], 0);
	}
}

void updateGlobal(int src, int dst, bool taken){
	if(globalCounters[six_seq] > 1){	//if predicting taken then...
		globalPrediction = 't';
	}else{					//if predicting not taken...
		globalPrediction = 'n';
	}
	if(taken == (globalCounters[six_seq] > 1)){
		global_correctness++;
	}
	if(taken){
		globalCounters[six_seq]++;
		globalCounters[six_seq] = MIN(globalCounters[six_seq], 4);
	}else{
		globalCounters[six_seq]--;
		globalCounters[six_seq] = MAX(globalCounters[six_seq], 0);
	}
	six_seq = six_seq << 1;
	six_seq = six_seq & MAX_SIX;
	if(taken){
		six_seq++;
	}
}

void updateSelector(int src, int dst, bool taken){
	if(selector[src] > 1){	//if selector set to use globalData then...
		selectorUsed = 'g';
	}else{			//if selector set to use localData then...
		selectorUsed = 'l';
	}

	bool local_taken = (localCounters[src] > 1);
	bool global_taken = (globalCounters[six_seq] > 1);
	if(global_taken != local_taken){	//if selection prediction mismatch then...
		if(global_taken == taken){	//if global predicted correctly then...
			selector[src]++;
			selector[src] = MIN(selector[src], 4);
		}else if(local_taken == taken){	//if local predicted correctly...
			selector[src]--;
			selector[src] = MAX(selector[src], 0);
		}
	}

}

void updatePredictors(int src, int dst, bool taken){
	updateSelector(src, dst, taken);	//this happens first to update selector on old states
	updateLocal(src, dst, taken);		//update arrays now
	updateGlobal(src, dst, taken);
	switch(selectorUsed){
		case 'l':
			finalPrediction = localPrediction;
		break;
		case 'g':
			finalPrediction = globalPrediction;
		break;
		default:
			cout << "Invalid selector state.  Now exiting.\n";
			exit(0);
		break;
	}

	actualDirection = ((taken)?('t'):('n'));

	if(actualDirection == finalPrediction){
		selector_correctness++;
	}

	cout << src << localPrediction << globalPrediction << selectorUsed << finalPrediction << actualDirection << endl;
}

int main(){

	for(int i = 0; i < 64; i++){
		globalCounters[i] = 0;
	}

	int lineCount = 0;
	while(cin.good() && !cin.eof()){
		char line[3];
		int src;
		bool taken;
		int dst;
		cin >> line[0];
		cin >> line[1];
		cin >> line[2];
		if(!cin.good() || cin.eof()){
			continue;
		}
		lineCount++;
		if(MIN(line[0],line[2]) < '0' || MAX(line[0], line[2]) > '9'){
			cout << "Improper src/dst.\n";
			return 0;
		}else{
			src = line[0] - '0';
			dst = line[2] - '0';
		}
		switch(line[1]){
			case 't':
			case 'T':
				taken = true;
			break;
			case 'n':
			case 'N':
				taken = false;
			break;
			default:
				cout << "Invalid taken/not-taken character.\n";
				return 0;
			break;
		}
		updatePredictors(src, dst, taken);	//update predictors before advancing line
	}
	if(TESTING){
		cerr << "Global Correct:\t" << global_correctness << "/" << lineCount << endl;
		cerr << "Local Correct:\t" << local_correctness << "/" << lineCount << endl;
		cerr << "Tournament correct:\t" << selector_correctness << "/" << lineCount << endl;
	}
}
