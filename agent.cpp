#include "agent.h"
#include <math.h>
#include <iostream>

#include "random.h"

const int Agent::EXPLORATION_THRESHOLD = 5;
const int Agent::ITERATION_THRESHOLD = 100000;
const int Agent::POLICY_THRESHOLD = 1;
const int Agent::MIN_THRESHOLD = 1000;
const double Agent::UPDATE_THRESHOLD = 0.0001;
const double Agent::REWARD_PLUS = 10;
const double Agent::GAMMA = 0.9;
const double Agent::SUCCESS = 0.8;
const double Agent::FAILURE = 1 - SUCCESS;

Agent::Agent(Grid& grid):grid(grid),cRow(grid.getStartLocation().first),
									cCol(grid.getStartLocation().second)
{
}

Agent::~Agent()
{
}

void Agent::run()
{
	int steps = 0;
	double diff = 0;
	while(steps++ < ITERATION_THRESHOLD)
	{

		if(steps%POLICY_THRESHOLD == 0)
			grid.updatePolicy(this);
		if(steps%(MIN_THRESHOLD) == 0){	
			if(diff < UPDATE_THRESHOLD){
				std::cout << "Diff: " << diff << std::endl;
				break;
			}
			diff = 0;
		}
		GridCell& cell = grid[cRow][cCol];

		if(cell.type == GridCell::TERMINAL){
			std::cout << "ERROR!" << std::endl;
			return;
		}
		cell.n++;
		//double newU = updateUtility(cRow,cCol,NORTH);
		//for(int d = 1; d < NONE; ++d)
		//	newU = std::max(newU, updateUtility(cRow,cCol,(Direction)d));
		double newU = updateUtility(cRow,cCol);
		diff += abs(newU - cell.u());
		cell.u() = newU;

		Direction d = step(cell.dir());
		move(d);

		if(grid[cRow][cCol].type == GridCell::TERMINAL)
		{
			cRow = grid.getStartLocation().first;
			cCol = grid.getStartLocation().second;
		}		
		//std::cout << cRow << ' ' << cCol << std::endl;
		//grid.print(std::make_pair(cRow,cCol));
	}
}
double Agent::updateUtility(int row, int col, Direction dir){
	GridCell& source = grid[row][col];
	return source.u() + alpha(source.n) * ( source.reward + GAMMA * expected(row,col,dir) - source.u());
}

double Agent::updateUtility(int row, int col){
	GridCell& source = grid[row][col];
	return source.u() + alpha(source.n) * ( source.reward + GAMMA * bestExpected(row,col) - source.u());
}

double Agent::f(GridCell& cell){
	if(cell.type == GridCell::TERMINAL)
		return cell.reward;

	if(cell.n < EXPLORATION_THRESHOLD)
		return REWARD_PLUS;
	return cell.u();
}

double Agent::alpha(int n)
{
	return 1.0/(n+1);
}

double Agent::expected(int row, int col, Direction d)
{
	Direction l = leftOf(d);
	Direction r = rightOf(d);
	double forward = SUCCESS * (grid.legal(row,col,d)
						?f(grid[offsetBy(row,col,d)])
						:grid[row][col].u());
	double left = FAILURE/2 * (grid.legal(row,col,l)
						?f(grid[offsetBy(row,col,l)])
						:grid[row][col].u());
	double right = FAILURE/2 * (grid.legal(row,col,r)
						?f(grid[offsetBy(row,col,r)])
						:grid[row][col].u());
	return forward+left+right;
}

double Agent::bestExpected(int row, int col)
{
	double u = grid[row][col].u();
	double utilities[4] = {
		grid.legal(row,col,NORTH)?f(grid.step(row,col,NORTH)):u,
		grid.legal(row,col,EAST)?f(grid.step(row,col,EAST)):u,
		grid.legal(row,col,SOUTH)?f(grid.step(row,col,SOUTH)):u,
		grid.legal(row,col,WEST)?f(grid.step(row,col,WEST)):u
	};

	double e[4] = {
		utilities[0] * SUCCESS + utilities[3] * FAILURE / 2 + utilities[1] * FAILURE / 2,
		utilities[1] * SUCCESS + utilities[0] * FAILURE / 2 + utilities[2] * FAILURE / 2,
		utilities[2] * SUCCESS + utilities[1] * FAILURE / 2 + utilities[3] * FAILURE / 2,
		utilities[3] * SUCCESS + utilities[2] * FAILURE / 2 + utilities[0] * FAILURE / 2
	};

	double best = e[0];
	for(int i = 1; i < 4; ++i)
		if(e[i] > best)
			best = e[i];
	return best;
}

Direction Agent::step(Direction d)
{
	//the generator is created ONCE, so our numbers won't repeat
	//	if this function is called quickly
	//static std::default_random_engine generator(time(0));
	//static std::discrete_distribution<int> distribution {SUCCESS, FAILURE/2, FAILURE/2};
	//static auto randCase = std::bind(distribution, generator);
	double weights[] = {SUCCESS, FAILURE/2, FAILURE/2};
	DiscreteRandomInt randCase(weights,3);


	switch(randCase())
	{
	case 0:
		return d;
	case 1:
		return leftOf(d);
	case 2:
		return rightOf(d);
	default:
		;
	}
	std::cout << "ERROR!" << std::endl;
	return NONE;
}

void Agent::move(Direction d)
{
	if(!grid.legal(cRow,cCol,d))
		return;

	std::pair<int, int> newLoc = offsetBy(cRow, cCol, d);
	cRow = newLoc.first;
	cCol = newLoc.second;
}
