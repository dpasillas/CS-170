#ifndef AGENT_H
#define AGENT_H
#include "grid.h"

class Agent{

public:
	Agent(Grid& g);
	~Agent();
	void run();
private:
	friend class Grid;

	double updateUtility(int row, int col, Direction dir);	
	double updateUtility(int row, int col);
	double f(GridCell& cell);
	double alpha(int n);
	double expected(int row, int col, Direction d);
	double bestExpected(int row, int col);
	Direction step(Direction d);
	void move(Direction d);

	static const int ITERATION_THRESHOLD;
	static const int POLICY_THRESHOLD;
	static const int MIN_THRESHOLD;
	static const int EXPLORATION_THRESHOLD;
	static const double UPDATE_THRESHOLD;
	static const double REWARD_PLUS;
	static const double GAMMA;
	static const double SUCCESS;
	static const double FAILURE;
	Grid& grid;
	int cRow, cCol;
};
#endif
