#ifndef AGENT_H
#define AGENT_H
#include "grid.h"

class Agent{

public:
	double updateUtility(GridCell& source, GridCell& target);

private:
	double f(GridCell& cell);
	double alpha(int n);

	static const int EXPLORATION_THRESHOLD;
	static const double REWARD_PLUS;
	static const double GAMMA;
};
#endif
