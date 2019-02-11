#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

const int N = 9;
const string LINE("-------------------------------------------------------------\n");

// Return 1d distance
int checkReward(int answer, int response) {
	int ret = -(abs(answer - response));
	return ret;
}	

// Generate randomly distributed query table
vector < pair < int,int > > stateGen() {
	bool check[N+1][N+1];
	vector < pair < int,int > > ret;
	for(int i=1; i<=N; i++) {
		for(int j=1; j<=N; j++) {
			check[i][j] = false;
		}
	}
	
	int cnt = 81;
	while(cnt) {
		int x=rand()%9+1, y=rand()%9+1;
		if(check[x][y]) continue;
		check[x][y] = true;
		ret.push_back({x,y});
		cnt--;
	}
	
	return ret;
}

// generated randomly shuffled vector
vector < int > random_Permuatation() {
	srand(time(NULL));
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	vector<int> ret;

	for(int i=2; i<=81; i++)
		ret.push_back(i);

	shuffle(ret.begin(),ret.end(),default_random_engine(seed));
	Sleep(10);
	return ret;
}

int main() {

	// Set File I/O
	fstream out;
	out.open("result.txt",fstream::out);
	
	// answerTable[i][j] = i*j
	// valueTable[i][j][k] = currently estimated q((i,j), k)
	// predictionTable[i][j] = (k that q(i,j) = valueTable[i][j][k])
	int answerTable[N+1][N+1], valueTable[N+1][N+1][N*N+1], predictionTable[N+1][N+1];
	
	// init
	for(int i=1; i<=N; i++) {
		for(int j=1; j<=N; j++) {
			for(int k=1; k<=N*N; k++) {
				valueTable[i][j][k] = 0;
			}
			answerTable[i][j] = i*j;
			predictionTable[i][j] = 1;
		}
	}
	
	// Start evolution
	int generation = 1;
	while(1) {
		vector < pair < int,int > > query = stateGen();
		out << "Generation " << generation << "\n\n";
		int answerCount = 0; 
		cout << generation << '\n';

		// complete table with given query
		for(auto& v : query) {
			int x = v.first, y=v.second;
			int target = predictionTable[x][y];
			int targetValue = valueTable[x][y][target];
			vector <int> searchList = random_Permuatation();
			for(int i : searchList) {
				//cout << i << " ";
				if(valueTable[x][y][i] > targetValue) {
					targetValue = valueTable[x][y][i];
					target = i;
				}
			}
			//cout << endl;
			
			// Check reward
			valueTable[x][y][target] = checkReward(answerTable[x][y],target);

			if(valueTable[x][y][target] == 0) {
				answerCount++;
			}

			predictionTable[x][y] = target;
		}

		// print table
		for(int i=1; i<=9; i++) {
			for(int j=1; j<=9; j++) {
				out << predictionTable[i][j] << " ";
			}
			out << '\n';
		}

		// If all answers are correct
		if(answerCount == N*N) {
			out << "\nLearning finished" << '\n';
			break;
		}

		// Evaluate accuracy
		out << "Accuracy: " << answerCount << "/81\n" << LINE;
		generation++;
	}
	
	
	return 0;
}