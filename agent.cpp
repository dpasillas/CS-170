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
{}

Agent::~Agent()
{}

void Agent::run()
{
	int steps = 0;
	double diff = 0;
	while(steps++ < ITERATION_THRESHOLD)
	{
		if(steps%POLICY_THRESHOLD == 0)
			grid.updatePolicy(this);

		//if MIN_THRESHOLD steps have been taken
		// check the improvement
		if(steps%(MIN_THRESHOLD) == 0){	
			if(diff < UPDATE_THRESHOLD){
				break;
			}
			diff = 0;
		}
		GridCell& cell = grid[cRow][cCol];

		//increment visits to the current GridCell
		cell.n++;

		//calculate new utility of current cell
		double newU = updateUtility(cRow,cCol);

		//accumulate the "improvement"
		diff += abs(newU - cell.u());

		//update utility
		cell.u() = newU;

		//get direction
		// 0.8 in direction of policy
		// 0.1 left of policy
		// 0.1 right of policy
		Direction d = step(cell.dir());

		//take a step in that direction
		move(d);

		//if the agent moved to a terminal state, move to the start state
		if(grid[cRow][cCol].type == GridCell::TERMINAL)
		{
			cRow = grid.getStartLocation().first;
			cCol = grid.getStartLocation().second;
		}		
		//grid.print(std::make_pair(cRow,cCol));
	}
}

// temporal difference, unused
double Agent::updateUtility(int row, int col, Direction dir){
	GridCell& source = grid[row][col];
	return source.u() + alpha(source.n) * ( source.reward + GAMMA * expected(row,col,dir) - source.u());
}

// temporal difference checking all directions
double Agent::updateUtility(int row, int col){
	GridCell& source = grid[row][col];
	return source.u() + alpha(source.n) * ( source.reward + GAMMA * bestExpected(row,col) - source.u());
}

//exploration function
double Agent::f(GridCell& cell){
	if(cell.type == GridCell::TERMINAL)
		return cell.reward;

	if(cell.n < EXPLORATION_THRESHOLD)
		return REWARD_PLUS;
	return cell.u();
}

//decreases learning over time
double Agent::alpha(int n)
{
	return 1.0/(n+1);
}

//expected value of taking a step in a single direction, unused
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

//max expected value for all directions
double Agent::bestExpected(int row, int col)
{
	double u = grid[row][col].u();

	//if a move is illegal, assume the agent stays in place
	// we don't use the exploration function if staying in place
	double utilities[4] = {
		grid.legal(row,col,NORTH)?f(grid.step(row,col,NORTH)):u,
		grid.legal(row,col,EAST)?f(grid.step(row,col,EAST)):u,
		grid.legal(row,col,SOUTH)?f(grid.step(row,col,SOUTH)):u,
		grid.legal(row,col,WEST)?f(grid.step(row,col,WEST)):u
	};

	//expected values for all directions
	double e[4] = {
		utilities[0] * SUCCESS + utilities[3] * FAILURE / 2 + utilities[1] * FAILURE / 2,
		utilities[1] * SUCCESS + utilities[0] * FAILURE / 2 + utilities[2] * FAILURE / 2,
		utilities[2] * SUCCESS + utilities[1] * FAILURE / 2 + utilities[3] * FAILURE / 2,
		utilities[3] * SUCCESS + utilities[2] * FAILURE / 2 + utilities[0] * FAILURE / 2
	};

	//choose the maximum value of these
	double best = e[0];
	for(int i = 1; i < 4; ++i)
		if(e[i] > best)
			best = e[i];
	return best;
}

Direction Agent::step(Direction d)
{
	/////////////////////////////////////////////////////
	// c++11, doesn't work on well.cs.ucr.edu
	//     g++ is too many versions behind
	/////////////////////////////////////////////////////
	/*the generator is created ONCE, so our numbers won't repeat
		if this function is called quickly
	static std::default_random_engine generator(time(0));
	static std::discrete_distribution<int> distribution {SUCCESS, FAILURE/2, FAILURE/2};
	static auto randCase = std::bind(distribution, generator);*/
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

	//error code, shouldn't happen
	std::cout << "ERROR!" << std::endl;
	return NONE;
}

//take a step in the d direction
void Agent::move(Direction d)
{
	//if the move is illegal, don't move
	if(!grid.legal(cRow,cCol,d))
		return;

	//otherwise, take a step
	std::pair<int, int> newLoc = offsetBy(cRow, cCol, d);
	cRow = newLoc.first;
	cCol = newLoc.second;
}
