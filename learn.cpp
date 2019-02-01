#include <bits/stdc++.h>
using namespace std;

const int N = 9;

int checkReward(int answer, int response) {
	int ret = -(abs(answer - response));
	return ret;
}	

vector < pair < int,int > > stateGen() {
	srand(time(NULL));
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

vector < int > random_Permuatation() {
	srand(time(NULL));
	vector<int> ret;
	bool check[N*N+1];

	for(int i=2; i<=81; i++)
		check[i] = false;

	int cnt = 80;
	while(cnt) {
		int x = rand()%80 + 2;
		if(check[x]) continue;
		check[x] = true;
		ret.push_back(x);
		cnt--;
	}
	return ret;
}

int main() {

	fstream out;
	out.open("result.txt",fstream::out);
	
	int answerTable[N+1][N+1], valueTable[N+1][N+1][N*N+1], predictionTable[N+1][N+1];
	
	const string LINE("-------------------------------------------------------------\n");
	
	for(int i=1; i<=N; i++) {
		for(int j=1; j<=N; j++) {
			for(int k=1; k<=N*N; k++) {
				valueTable[i][j][k] = 0;
			}
			answerTable[i][j] = i*j;
			predictionTable[i][j] = 1;
		}
	}
	
	int generation = 1;
	while(1) {
		vector < pair < int,int > > query = stateGen();
		out << "Generation " << generation << "\n\n";
		int answerCount = 0; 
		for(auto& v : query) {
			int x = v.first, y=v.second;
			int target = predictionTable[x][y];
			int targetValue = valueTable[x][y][target];
			vector <int> searchList = random_Permuatation();
			for(int i : searchList) {
				cout << i << " ";
				if(valueTable[x][y][i] > targetValue) {
					targetValue = valueTable[x][y][i];
					target = i;
				}
			}
			cout << endl;
			
			valueTable[x][y][target] = checkReward(answerTable[x][y],target);

			if(valueTable[x][y][target] == 0) {
				answerCount++;
			}

			predictionTable[x][y] = target;
		}

		for(int i=1; i<=9; i++) {
			for(int j=1; j<=9; j++) {
				out << predictionTable[i][j] << " ";
			}
			out << '\n';
		}

		if(answerCount == N*N) {
			out << "\nLearning finished" << '\n';
			break;
		}
		out << "\nAccuracy: " << answerCount << "/81\n" << LINE;
		generation++;
	}
	
	
	return 0;
}