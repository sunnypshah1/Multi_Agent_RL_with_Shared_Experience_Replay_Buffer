#include <iostream>
#include <vector>
#include <string>
#include <random>

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
	std::random_device rd;
	std::mt19937 gen(rd());
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

int main() 
{
auto grid = generateFrozenLake(4, 0.5);
	for (const auto &row : grid)
	{
		for (char cell : row) std::cout << cell << " ";
		std::cout << std::endl;
	}

	return 0;
}

