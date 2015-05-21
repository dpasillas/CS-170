#include "agent.h"


const int Agent::EXPLORATION_THRESHOLD = 50;
const double Agent::REWARD_PLUS = 10;
const double Agent::GAMMA = 1;

double Agent::updateUtility(GridCell& source, GridCell& target){
	return source.u() + alpha(source.n) * ( source.reward + GAMMA * f(target) - f(source));
}

double Agent::f(GridCell& cell){
	if(cell.n < EXPLORATION_THRESHOLD)
		return REWARD_PLUS;
	return cell.u();
}

double Agent::alpha(int n)
{
	return 1.0/(n+1);
}
