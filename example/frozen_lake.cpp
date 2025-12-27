#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <unordered_set>
#include <cstdlib>

// Create frozen lake that always has a valid path with size and hole probability paramter
// - Initializes a grid with a sentinel value
// - Defines start and goal
// - Creates a random solution path
// - Randomly fill in rest based on probability
std::vector<std::vector<char>> generateFrozenLake(int size, double hole_prob)
{
	// Init return grid
	std::vector<std::vector<char>> grid(size, std::vector<char>(size, '#'));

	// Get goal and start values and set them in grid
	int startRow = 0, startCol = 0;
	int goalRow = size - 1, goalCol = size - 1;
	grid[startRow][startCol] = 'S';
	grid[goalRow][goalCol] = 'G';

	// Create random path for solution
	int currRow = 0, currCol = 0;
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 1);

	while (currRow < goalRow || currCol < goalCol)
	{
		// Base cases = ie either row or col already match need to move 
		// other forward to hit goal
		if (currRow == goalRow) currCol++;
		else if (currCol == goalCol) currRow++;
		else 
		{
			if (dis(gen) == 0) currRow++;
			else currCol++;
		}

		if (grid[currRow][currCol] == '#')
		{
			grid[currRow][currCol] = 'F';
		}
	}

	// Randomly fill in the rest based on prob
	std::uniform_real_distribution<double> prob_dis(0.0, 1.0);
	for (size_t r{}; r < size; ++r)
	{
		for (size_t c{}; c < size; ++c)
		{
			if (grid[r][c] == '#')
			{
				if (prob_dis(gen) < hole_prob) grid[r][c] = 'H';
				else grid[r][c] = 'F';
			}
		}
	}

	return grid;
}

char generateRandomStepandValidate(std::pair<int, int> &currPos)
{
	// Get random device and see what moves are allowed based on current position
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::unordered_set<char> movesAllowed;
	if (currPos.first > 0) movesAllowed.insert('U');
	if (currPos.first < 3) movesAllowed.insert('D');
	if (currPos.second > 0) movesAllowed.insert('L');
	if (currPos.second < 3) movesAllowed.insert('R');

	std::uniform_int_distribution<> dis(0, 3);
	
	// Keep looping until a move is generated
	while (true) 
	{
		switch (dis(gen)) 
		{
			case 0:
				if (movesAllowed.contains('U')) return 'U';
				break;
			case 1:
				if (movesAllowed.contains('D')) return 'D';
				break;
			case 2:
				if (movesAllowed.contains('L')) return 'L';
				break;
			case 3:
				if (movesAllowed.contains('R')) return 'R';
				break;
		}
	}
}


int main(int argc, char** argv) 
{
	// Command line args for learning rate and discount factor
	double learningRate = std::atof(argv[1]);
	double discountFactor = std::atof(argv[2]);

	// Print grid for debugging
	auto grid = generateFrozenLake(4, 0.5);
	for (const auto &row : grid)
	{
		for (char cell : row) std::cout << cell << " ";
		std::cout << std::endl;
	}

	// Initalize Q-Table
	std::vector<std::vector<double>> qTable(16, std::vector<double>(4, 0.0));
	//for (const auto &row : qTable)
	//{
	//	for (int cell : row) std::cout << cell << " ";
	//	std::cout << std::endl;
	//}
	
	std::pair<int, int> currPos = {0, 0};
	int steps = 0;
	const int MAX_STEPS = 100;

	while (steps++ < MAX_STEPS)
	{
		int action;
		char move = generateRandomStepandValidate(currPos);
		int prevRow = currPos.first, prevCol = currPos.second;
		switch (move)
		{
			case 'U':
				action = 0;
				currPos.first--;
				break;
			case 'D':
				action = 1;
				currPos.first++;
				break;
			case 'L':
				action = 2;
				currPos.second--;
				break;
			case 'R':
				action = 3;
				currPos.second++;
				break;
		}
		int newRow = currPos.first, newCol = currPos.second;
		int prevState = prevRow * 4 + prevCol;
		int newState = newRow * 4 + newCol;

		double reward = (grid[currPos.first][currPos.second] == 'G') ? 1.0 : 0.0;

		double maxNextStep = 0.0;
		if (reward == 0.0) maxNextStep = *std::max_element(qTable[newState].begin(), qTable[newState].end());

		qTable[prevState][action] += learningRate * (reward + discountFactor * maxNextStep - qTable[prevState][action]);
		
		if (reward == 1.0 || grid[newRow][newCol] == 'H') break;
	}
	
	return 0;
}

