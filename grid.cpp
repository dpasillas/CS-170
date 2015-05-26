////////////////////////////////
//
// University of California, Riverside
// CS170 Spring 2015 Assignment 3
// grid.cpp
//
// You are encouraged to modify and add to this file
//////////////////////////////
#include "grid.h"

#include <iostream>
#include <iomanip>
#include <cfloat>
#include <cmath>
#include <sstream>

#include "random.h"

#include "agent.h"

using namespace std;
///////////////
/// Utility ///
///////////////

Direction leftOf(Direction d)
{
	switch(d)
	{
	case NORTH:
		return WEST;
	case EAST:
		return NORTH;
	case SOUTH:
		return EAST;
	case WEST:
		return SOUTH;
	default:
		;
	}
	return NONE;
}
Direction rightOf(Direction d)
{
	switch(d)
	{
	case NORTH:
		return EAST;
	case EAST:
		return SOUTH;
	case SOUTH:
		return WEST;
	case WEST:
		return NORTH;
	default:
		;
	}
	return NONE;
}

std::pair<int, int> offsetBy(int row, int col, Direction d)
{
	return std::make_pair(row+ro[d], col+co[d]);
}


///////////
// Function centerStr
///////////
// In -> string s      - value of string we are centering.
//       int reserve   - # of characters (width).
//       char autofill - char to print for the left and right remaining space.
// Out -> string
//
// Returns a string with length of the reserved space with the string, s, centered.
string centerStr(const string & s, int reserve, char autofill)
{
    int blanks = reserve - s.length();
    int lbuf = blanks / 2; // Add one to favor remainder to right
    int rbuf = (blanks+1) / 2;
    stringstream ss;
    ss << setfill(autofill) << setw(lbuf) << "" << s << setw(rbuf) << "";
    return ss.str();
}

///////////
// Function toString
///////////
// In -> double
// Out -> string
//
// Returns a double converted to a string with fixed precision.
string toString(double value, int prec)
{
    stringstream ss;
    if (value > 0.0)
        ss << '+';
    ss << setprecision(prec) << fixed << value;
    return ss.str();
}

///////////
// Function toString
///////////
// In -> Direction
// Out -> string
//
// Returns an ascii version of Direction as a string.
string toString(Direction d)
{
    switch (d)
    {
        case NORTH:
            return "^";
        case EAST:
            return ">";
        case SOUTH:
            return "v";
        case WEST:
            return "<";
        default:
            return "?";
    }
}

///////////////////////
/// GridCell Object ///
///////////////////////

// Constructors
GridCell::GridCell()
    : type(BLANK), reward(0.0), start(false), policy(NONE, 0.0), n(0)
{
}

GridCell::GridCell(GridCellType type, double reward, bool start)
    : type(type), reward(reward), start(start), policy(NONE, 0.0), n(0)
{
}
    
///////////
// Function print
///////////
// In -> string pstr - agent symbol, can be the empty string if not need print.
//       int reserve - the amount of characters (width) we can print for each line.
// Out -> vector<string> - index by row, the output contents of this cell
//
// Returns the output contents of this cell in four different rows.
// Refer to Grid class for more information of the ascii-grid layout
vector<string> GridCell::print(const string & pstr, int reserve) const
{
    vector<string> ret(4);
        
    string prefix1st = "+";
    string prefixedge = "|";
    if (type == OBSTACLE)
    {
        ret[0] = prefix1st + centerStr("", reserve - prefix1st.length(), '-');
        ret[1] = prefixedge + centerStr("xxxx", reserve - prefixedge.length(), ' ');
        ret[2] = prefixedge + centerStr("xxxx", reserve - prefixedge.length(), ' ');
        ret[3] = prefixedge + centerStr("xxxx", reserve - prefixedge.length(), ' ');
    }
    else
    {
        ret[0] = prefix1st + centerStr("", reserve - prefix1st.length(), '-');
        ret[1] = prefixedge + centerStr(toString(reward + policy.second), reserve - prefixedge.length());
        ret[2] = prefixedge + centerStr(start ? pstr + "S" : pstr, reserve - prefixedge.length());
        if (type == TERMINAL)
            ret[3] = prefixedge + centerStr("*", reserve - prefixedge.length());
        else
            ret[3] = prefixedge + centerStr(toString(policy.first), reserve - prefixedge.length());
    }
        
    return ret;
}
    
///////////
// Function getPolicy
///////////
// In ->
// Out -> pair<Direction, double>
//
// Returns the current policy in the form of the pair,
// The first being the action and the the second being the value.
pair<Direction, double> GridCell::getPolicy() const
{
    return policy;
}

double& GridCell::u()
{
	return policy.second;
}

Direction& GridCell::dir()
{
	return policy.first;
}


///////////////////
/// Grid Object ///
///////////////////
    
///////////////
// Constructors
///////////////
// In -> int n, parameter described by assignment prompt
//       int m, parameter described by assignment prompt
// Out ->
Grid::Grid(int n, int m)
{
    bounds = pair<int,int>(n, n);
    
	UniformRandomInt randCell(0,n*n-1);
    //std::default_random_engine generator(time(0));
	//std::uniform_int_distribution<int> distribution(0,n*n-1);
	//auto randCell = std::bind(distribution, generator);
    
    
    do
    {
        grid = vector<vector<GridCell> >(bounds.first, vector<GridCell>(bounds.second));

        //generate walls
        int walls = 0;
        while(walls < n){
            int loc = randCell();
            GridCell& cell = grid[loc/n][loc%n];
            if(cell.type == GridCell::BLANK)
                cell.type = GridCell::OBSTACLE, walls++;
        }
        //generate positive rewards
        int rewards = 0;
        while(rewards < m){
            int loc = randCell();
            GridCell& cell = grid[loc/n][loc%n];
            if(cell.type == GridCell::BLANK)
                cell.type = GridCell::TERMINAL, rewards++, cell.reward=10;
        }
        //generate negative rewards
        rewards = 0;
        while(rewards < m){
            int loc = randCell();
            GridCell& cell = grid[loc/n][loc%n];
            if(cell.type == GridCell::BLANK)
                cell.type = GridCell::TERMINAL, rewards++, cell.reward=-10;
        }

        //finally, generate start location
        while(true){
            int loc = randCell();
            GridCell& cell = grid[loc/n][loc%n];
            if(cell.type == GridCell::BLANK){
                startLocation = pair<int,int>(loc/n, loc%n);
                grid[loc/n][loc%n].start = true;
                break;
            }
        }
    }while(!valid());

	UniformRandomInt randDir(0,3);
	//std::uniform_int_distribution<int> d_distribution(0,3);
	//auto randDir = std::bind(d_distribution, generator);

	//now that the board has been generated, create random policy
	for(int loc = 0; loc < n*n; ++loc)
	{
		GridCell& cell = grid[loc/n][loc%n];
		if(cell.type == GridCell::BLANK){
			cell.dir() = (Direction)randDir();
			cell.u() = cell.reward;
		}
	}
}
    
// Accessors to the 2D grid

//grid[row][col]
//grid[std::make_pair(row,col)];
GridCell& Grid::operator[](const pair<int,int> & pos)
{
    return grid[pos.first][pos.second];
}
const GridCell& Grid::operator[](const pair<int,int> & pos) const
{
    return grid[pos.first][pos.second];
}
std::vector<GridCell>& Grid::operator[](int row)
{
	return grid[row];
}
const std::vector<GridCell>& Grid::operator[](int row) const
{
	return grid[row];
}
GridCell& Grid::neighbor(int row, int col, Direction d){
	return operator [] (offsetBy(row,col,d));
}

pair<int,int> Grid::getBounds() const
{
    return bounds;
}
int Grid::getRows() const
{
    return bounds.first;
}
int Grid::getCols() const
{
    return bounds.second;
}
pair<int,int> Grid::getStartLocation() const
{
    return startLocation;
}
    
///////////
// Function print
///////////
// In -> pair<int,int> agentPos -
//          position of agent provided. If indicies are negative or out
//          of bounds. This parameter is ignored.
// Out ->
//
// Prints each cell content with special information centered on its row in the following format:
//
// +------
// |value
// |start
// |policy
//
// value - displays the value of the current policy in the format [+/-]##.##
// start - displays 'S' for whether this cell is a start location. A 'P' is also appended
//          if the agentPos is located at that cell.
// policy - displays '^' '>' 'v' '<' if the best policy is NORTH, EAST, SOUTH, or WEST respectively.
//              If policy is set to NONE, it will display '?'
//
// Special cases:
// - If cell is an obstacle, the contents of the cell for each line contain "xxxx" instead
// - if cell is a terminal, the policy displayed for that cell will be '*' instead
//
// Once each cell is printed, it will append a final right and bottom edge
// to complete the ascii table.
void Grid::print(const pair<int,int> & agentPos) const
{
    const int RESERVE = 7;
    for (int i = 0; i < bounds.first; ++i)
    {
        vector<string> outputs(4);
            
        for (int j = 0; j < bounds.second; ++j)
        {
            vector<string> ret;
            if (i == agentPos.first && j == agentPos.second)
                ret = grid[i][j].print("P", RESERVE);
            else
                ret = grid[i][j].print(" ", RESERVE);
            outputs[0] += ret[0];
            outputs[1] += ret[1];
            outputs[2] += ret[2];
            outputs[3] += ret[3];
        }
        cout << outputs[0] << "+" << endl
        << outputs[1] << "|" << endl
        << outputs[2] << "|" << endl
        << outputs[3] << "|" << endl;
    }
    cout << left << setfill('-');
    for (int j = 0; j < bounds.second; ++j)
        cout << setw(RESERVE) << "+";
    cout << "+";
    cout << endl;
}

bool Grid::valid()
{
    return valid(startLocation.first, startLocation.second);
}

bool Grid::valid(int row, int col)
{
    if(grid[row][col].n){
            return false;
    }
    Adjuster<int> adj(grid[row][col].n);
    
    if(grid[row][col].type == GridCell::TERMINAL)
            return true;
    //no backtracking!
    for(Direction d = NORTH; d != NONE; d = (Direction)(d+1) )
    {
        if(legal(row,col,d) && valid(row+ro[d], col+co[d]))
            return true;
    }
    
    return false;
}

bool Grid::legal(int row, int col, Direction d)
{
    int nrow = row + ro[d];
    int ncol = col + co[d];

	//out of bounds
    if(nrow >= getRows() || nrow < 0 || ncol >= getCols() || ncol < 0)
        return false;
    
	//hit a wall
    if(grid[nrow][ncol].type == GridCell::OBSTACLE)
        return false;
    
    return true;
}
//  0  1  2
//  3  4  5
//  6  7  9
//  9 10 11
// 12 13 14
void Grid::updatePolicy(Agent* agent)
{
	int rows = bounds.first;
	int cols = bounds.second;
	for(int loc = 0; loc < rows*cols; ++loc)
		updatePolicy(loc/cols, loc%cols, agent);
}

void Grid::updatePolicy(int row, int col, Agent* agent)
{
	Direction bestDir = NORTH;
	double bestVal = -11;	
	for(int dir = 0; dir < NONE; ++dir)
	{
		//if(!legal(row,col,(Direction)dir))
		//	continue;

		GridCell& cell = step(row,col,(Direction)dir);

		double val = agent->f(cell);

		if(val > bestVal){
			bestDir = (Direction)dir;
			bestVal = val;
		}
	}
	grid[row][col].dir() = bestDir;
}

GridCell& Grid::step(int row, int col, Direction d)
{
	return (legal(row,col,d)
				?(operator[](offsetBy(row,col,d)))
				:(grid[row][col]));
}

template<typename T>
    Adjuster<T>::Adjuster(T& t) :t(t)
    {t++;}
                 
template<typename T>
    Adjuster<T>::~Adjuster()
    {t--;}
