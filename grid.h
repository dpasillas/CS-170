#ifndef GRID_H
#define GRID_H

////////////////////////////////
//
// University of California, Riverside
// CS170 Spring 2015 Assignment 3
// grid.h
//
// You are encouraged to modify and add to this file
//////////////////////////////
// The Grid class provides a print utility and necessary accessors.
// Initialization of the Grid class is incomplete.
//
#include <vector>
#include <string>
#include <utility>
#include <stdlib.h>


///////////////
/// Utility ///
///////////////

const int NUM_DIRECTIONS = 4;

const int ro[4] = {-1,0,1,0};
const int co[4] = {0,1,0,-1};

enum Direction { NORTH, EAST, SOUTH, WEST, NONE };
Direction leftOf(Direction d);
Direction rightOf(Direction d);
std::pair<int, int> offsetBy(int row, int col, Direction d);

///////////
// Function centerStr
///////////
// In -> string s      - value of string we are centering.
//       int reserve   - # of characters (width).
//       char autofill - char to print for the left and right remaining space.
// Out -> string
std::string centerStr(const std::string & s, int reserve, char autofill = ' ');
///////////
// Function toString
///////////
// In -> double
// Out -> string
std::string toString(double value, int prec = 2);

///////////
// Function toString
///////////
// In -> Direction
// Out -> string
std::string toString(Direction d);

///////////////////////
/// GridCell Object ///
///////////////////////
//
// Contains information for a Cell in "GridWorld"
// and nice ascii printing functionality.
struct GridCell
{
    // GridCellType definitions:
    // BLANK - traversable cell
    // TERMINAL - episode ends if agent ever steps on this cell
    // OBSTACLE - non-traversable cell
    enum GridCellType { BLANK, TERMINAL, OBSTACLE };
    GridCellType type;
    
    // Reward for landing on this cell.
    double reward;
    
    // Flag which specifies whether this is a start location for the agent.
    bool start;
    
    // Current policy where the first is the direction and the second is the value.
    std::pair<Direction, double> policy;

	// The number of time the cell has been visited
	int n;
    
    // Constructors
    GridCell();
    GridCell(GridCellType type, double reward = 0.0, bool start = false);
    
    ///////////
    // Function print
    ///////////
    // In -> string pstr - agent symbol, can be the empty string if not need print.
    //       int reserve - the amount of characters (width) we can print for each line.
    // Out -> vector<string> - index by row, the output contents of this cell
    std::vector<std::string> print(const std::string & pstr, int reserve) const;
    
    ///////////
    // Function getPolicy
    ///////////
    // In ->
    // Out -> pair<Direction, double>
    std::pair<Direction, double> getPolicy() const;

	// current estimated utility of the cell
	double& u();

	Direction& dir();
};

///////////////////
/// Grid Object ///
///////////////////
//
// Contains information about "GridWorld"
// and nice ascii printing functionality.

class Agent;
class Grid
{
private:
    //NORTH, EAST, SOUTH, WEST
    //row offset, col offset
    
    // Number of rows and columns respectively
    std::pair<int,int> bounds;
    
    // Cell contents organized in a 2D vector
    std::vector<std::vector<GridCell> > grid;
    
    // starting (row,column) for the agent
    std::pair<int,int> startLocation;
public:
    // Constructors
    Grid(int n, int m);
    
    // Accessors to the 2D grid
    GridCell& operator[](const std::pair<int,int> & pos);
    const GridCell& operator[](const std::pair<int,int> & pos) const;
	std::vector<GridCell>& operator[](int row);
	const std::vector<GridCell>& operator[](int row) const;
	GridCell& neighbor(int row, int col, Direction d);
    std::pair<int,int> getBounds() const;
    int getRows() const;
    int getCols() const;
    std::pair<int,int> getStartLocation() const;
    bool valid();
    bool valid(int row, int col);
    bool legal(int row, int col, Direction d);
	void updatePolicy(Agent* agent);
	void updatePolicy(int row, int col, Agent* agent);
	GridCell& step(int row, int col, Direction d);
	
    
    ///////////
    // Function print
    ///////////
    // In -> pair<int,int> agentPos -
    //          position of agent provided. If indicies are negative or out
    //          of bounds. This parameter is ignored.
    // Out ->
    void print(const std::pair<int,int> & agentPos = std::make_pair(-1, -1)) const;
};

//increments value when constructed,
//decrements when destructed
// this guarantees desired behavior in a block with mutiple exit points
template<typename T>
class Adjuster{
private:
    T& t;
public:
    Adjuster(T& t);
    ~Adjuster();
};

#endif
