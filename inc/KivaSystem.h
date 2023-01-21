#pragma once
#include "BasicSystem.h"
#include "KivaGraph.h"

class KivaSystem :
	public BasicSystem
{
public:
	KivaSystem(KivaGrid& G, MAPFSolver& solver);
	~KivaSystem();

	void simulate(int simulation_time);


protected:
	KivaGrid& G;
	unordered_set<int> held_endpoints;

	void initialize();
    virtual void initialize_start_locations();
    virtual void initialize_goal_locations();
    virtual void update_goal_locations();
};

