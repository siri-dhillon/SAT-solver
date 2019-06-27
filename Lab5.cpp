
//Lab5 ENSC 351 
//Sirpreet Kaur Dhillon
//Jaime Lopez Raichs 

//Topic: SAT solving done in parallel

#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm> 
#include <cmath>
#include <omp.h>

using namespace std;

int nbvar = 1024;
int nbclauses = 1024;
vector<unsigned long long int> someClause;

struct clause{
		vector<unsigned long long int> activeVar = someClause; // this si for the variables present in a particular clause 
		vector<unsigned long long int> clauseVar = someClause; // these are the clauses themselves with values initialized fpr the active variables 
};

vector <clause> ListOfClauses;


//function declarations
void setClauseVar(int a, int k);
void incrementVect(vector<unsigned long long int> &vect);
bool checkAssumption(vector<unsigned long long int> const &assumptions);
void printResult(vector<unsigned long long int> const &assumptions, int const numVar);
bool checkEmptyClause(int clauseNum);


int main(){
	
	ifstream inFile; //for input file 
	string filename = "input.txt"; // change this name to your input file name
	inFile.open(filename);
	
	string ignore; //to ignore the first two strings of the input file 
	
	bool resultFound = false;
	int temp; 
	int k = 0; 
	vector <unsigned long long int> maxA(16);
	vector <unsigned long long int> assumptions(16);
	vector <unsigned long long int> result(16);
	
	inFile >> ignore; 
	inFile >> ignore; 
	
	inFile >> nbvar;
	inFile >> nbclauses;
	
	someClause.resize(16);
	ListOfClauses.resize(nbclauses);
	
	int tempNbVar = nbvar;
	
	//vector <clause> ListOfClauses(nbclauses);
	for(int i = 0; i < nbclauses ; i++){
		//going through all the clauses 
		for(int j = 0; j < 16; j++){
			//putiing zeros in active variables
			ListOfClauses[i].activeVar[j] = 0; 
			
			//putting zeroes for all the variables in the clauses
			ListOfClauses[i].clauseVar[j] = 0; 
		}
	}
	
	
	for(int j = 0; j < 16; j++){
		//initializing the assumptions for the variables to all false
		assumptions[j] = 0;
		
		if(tempNbVar >= 64){
			maxA[j] = ~0;
			tempNbVar -= 64;
		}
		else if(tempNbVar != 0){
			maxA[j] = pow(2, tempNbVar) - 1;
			tempNbVar = 0;
		}else{
			maxA[j] = 0;
		}
	}
			
	int cl = 0; 
	//reading the file and initializing the clauses to check with assumptions later 
	while (!(inFile.eof())){ 
		if(!(inFile >> temp)){
			break;
		}
		if (temp == 0){
			if(checkEmptyClause(k))
			{
				cerr << "\n\nERROR: Clause number "<< k+1 << " is empty.\n\n";
				return 1;
			}
			k++;
			if(k >= nbclauses)
			{
				cout << "Reading Successful." << endl;
				cl = 1;
				break;
			}
			else{
				continue;
			}
		} 
		if(cl != 1){
			setClauseVar(temp, k);
		}
	}
	
	//checking and changing begins 	
	//with threading 
	cout << "Looking for solution..." << endl;
	#pragma omp for
	for(int i = 0; i < 16; i++){
		for(unsigned long long int j = 0; j <= maxA[i] && !resultFound; j++){
			if(checkAssumption(assumptions)){			
				resultFound = true;
				result = assumptions;
				break;
			}
			if(!resultFound){
				incrementVect(assumptions);
			}
		}
	}
	
	if(!resultFound){
		cout << "\nThe problem given does not have a solution.\n\n";
	}else{
		cout << "\nSolution Found!\n\n";
		printResult(result, nbvar);
	}

	inFile.close();
	
	return 0; 
		
}


//This function initializes or saves all the clauses to a variable and this result is later used to find a solution 
// a = Variable (this can be positive or negative)
// k = Clause number
// sets variable a in clause number k 
// if a = -ve, ath varible in kth clause is set to true otherwise it is set to false 
void setClauseVar(int a, int k)
{
	int b = abs (a);
	int mask = 1;
	int vectNum = 0; 
	if(b <= 64){
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	else if(b <= 128){
		vectNum = 1;
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	else if(b <= 192){
		vectNum = 2;
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	else if(b <= 256){
		vectNum = 3;
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	else if(b <= 320){
		vectNum = 4;
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	else if(b <= 384){
		vectNum = 5;
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	else if(b <= 448){
		vectNum = 6;
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	else if(b <= 512){
		vectNum = 7;
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	else if(b <= 576){
		vectNum = 8;
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	else if(b <= 640){
		vectNum = 9;
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	else if(b <= 704){
		vectNum = 10;
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	else if(b <= 768){
		vectNum = 11;
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	else if(b <= 832){
		vectNum = 12;
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	else if(b <= 896){
		vectNum = 13;
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	else if(b <= 960){
		vectNum = 14;
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	else if(b <= 1024){
		vectNum = 15;
		ListOfClauses[k].activeVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].activeVar[vectNum]); 
		if(a < 0){
			ListOfClauses[k].clauseVar[vectNum] = (~((mask) << (b-1))) & (ListOfClauses[k].clauseVar[vectNum]);
		}
		else {
			ListOfClauses[k].clauseVar[vectNum] = ((mask) << (b-1)) | (ListOfClauses[k].clauseVar[vectNum]); 
		}		
	}
	 return;
		
}

//This function increments the vector and goes through all the possibilities for the solution 
void incrementVect(vector<unsigned long long int> &vect){
	int size = vect.size();
	if (size == 0){
		cerr << "ERROR: Vector is empty" << endl;
		return;
	}
	else if(size > 0){
		for(int i = 0; i < size; i++ ){
			if(vect[i] == (~0))
			{
				vect[i] = 0;
			}
			else{
				vect[i]++;
				break;
			}
		}
	}
}


// This function checks if a particular assumption meets all the clause requirements for all teh clauses in the file 
//returns true if it does and returns false otherwise
bool checkAssumption(vector<unsigned long long int> const &assumptions){
	int sizeAssumptions = assumptions.size();
	int sizeClauses = ListOfClauses.size();
	unsigned long long int tempClauseResult; 
	bool clauseResult = false;
	 
	if (sizeAssumptions == 0){
		cerr << "ERROR: Assumption Vector is empty" << endl;
		return false;
	}
	
	if (sizeClauses== 0){
		cerr << "ERROR: Clause Vector is empty" << endl;
		return false;
	}
	
	for(int i = 0; i < sizeClauses; i++){
		for(int j = 0; j < sizeAssumptions; j++ ){
			tempClauseResult = (~(assumptions[j]^ListOfClauses[i].clauseVar[j]));
			clauseResult = (tempClauseResult & ListOfClauses[i].activeVar[j]);
			if(clauseResult){
				break;
			}
		}
		if(!clauseResult){
			return false;
		}	
	}
	return true;
}

//Prints the result
void printResult(vector<unsigned long long int> const &assumptions, int numVar){
	int size = assumptions.size();
	int totalVar = numVar+1;
	unsigned long long int bitMask = 1;
	for(int i = 0; i < size; i++){
		for(int j = 0; j < 64 && numVar > 0; j++)
		{
			cout << "\nx" << totalVar-numVar << " = " << ((assumptions[i] & (bitMask << j)) >> j);
			numVar--;
		}
	}
	cout << "\n\n*1 means True\n**0 means False\n\nEnd Of Result List\n\n\n";
}

//checks if the clause that was just read is empty 
//It is empty if there are no variables in it 
bool checkEmptyClause(int clauseNum){
	for(int i = 0; i < 16; i++){
		if(ListOfClauses[clauseNum].activeVar[i] != 0)
		{
			return false;
		}
	}
	return true;
}
