#pragma once
#include "KivaSystem.h"
#include "json.hpp"
using json = nlohmann::json;

class PredefinedKivaSystem: public KivaSystem
{
public:
    PredefinedKivaSystem(KivaGrid& G, MAPFSolver& solver);
    ~PredefinedKivaSystem();
    bool load_instance(std::string fname, std::string map_name);
    void initialize_start_locations();
    void initialize_goal_locations();
    void update_goal_locations();
    void print_paths();
    std::vector<std::pair<int, int>> input_starts;
    std::vector<std::vector<std::pair<int, int>>> input_all_goals;
};
