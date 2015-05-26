Assumptions
	-if taking an action yields an illegal step, the agent stays in place
	-if neighboring utilities are tied, the the tie can be broken arbitrarily
		in my case, it is chosen by direction in the following order: NORTH, EAST, SOUTH, WEST
		That is, if NORTH and WEST are tied, NORTH is given preference.
		The assumption is that utilities will still converge
	-a grid is valid is ANY terminal state can be reached from the start state
		by taking legal moves.
		this may be determined by a depth-first search

Compile
	run 'make' from the terminal

Run
	type './main.out [n] [m]' where n and m are integer values
	alternatively, 
	type 'make run n=[val] m=[val]'
