#include "PredefinedKivaSystem.h"

PredefinedKivaSystem::PredefinedKivaSystem(KivaGrid &G, MAPFSolver &solver):KivaSystem(G, solver)
{
}

PredefinedKivaSystem::~PredefinedKivaSystem() = default;

bool PredefinedKivaSystem::load_instance(std::string fname, std::string map_name)
{
    std::ifstream f(fname);
    json data = json::parse(f);
    f.close();
    map_name = map_name.substr(0, map_name.rfind('.'));
    std::cout<<map_name<<" map_name\n"<<seed<<" seed\n";
    for(auto task: data)
    {
        if(task["map_name"] == map_name and int(task["seed"]) == seed)
        {
            for(auto agent: task["task"])
            {
                //std::cout<<agent<<"\n";
                input_starts.emplace_back(std::make_pair(agent["start"][0], agent["start"][1]));
                std::vector<std::pair<int, int>> goals;
                for(auto goal: agent["goals"])
                {
                    goals.emplace_back(std::make_pair(goal[0], goal[1]));
                }
                input_all_goals.emplace_back(goals);
                if(input_starts.size() == num_of_drives)
                    break;
            }
            return true;
        }
    }
    return false;
}

void PredefinedKivaSystem::initialize_start_locations()
{
    for (int k = 0; k < num_of_drives; k++)
    {
        int orientation = -1;
        if (consider_rotation)
            orientation = rand() % 4;
        int start_id = G.get_id(input_starts[k].first, input_starts[k].second);
        starts[k] = State(start_id, 0, orientation);
        paths[k].emplace_back(starts[k]);
        finished_tasks[k].emplace_back(std::make_pair(start_id, 0));
    }
}

void PredefinedKivaSystem::initialize_goal_locations()
{
    for (int k = 0; k < num_of_drives; k++) {
        auto g = input_all_goals[k].front();
        goal_locations[k].emplace_back(std::make_pair(G.get_id(g.first, g.second), 0));
        input_all_goals[k].erase(input_all_goals[k].begin());
    }

}

void PredefinedKivaSystem::update_goal_locations()
{
    for (int k = 0; k < num_of_drives; k++)
    {
        int curr = paths[k][timestep].location; // current location
        pair<int, int> goal; // The last goal location
        if (goal_locations[k].empty())
        {
            goal = make_pair(curr, 0);
        }
        else
        {
            goal = goal_locations[k].back();
        }
        double min_timesteps = G.get_Manhattan_distance(goal.first, curr); // G.heuristics.at(goal)[curr];
        while (min_timesteps <= simulation_window)
        {
            auto g = input_all_goals[k].front();
            curr = goal_locations[k].back().first;
            goal_locations[k].emplace_back(std::make_pair(G.get_id(g.first, g.second), 0));
            input_all_goals[k].erase(input_all_goals[k].begin());
            min_timesteps += G.get_Manhattan_distance(goal_locations[k].back().first, curr);
        }
    }
}

void PredefinedKivaSystem::print_paths()
{
	std::cout<<num_of_drives<<" "<<paths.size()<<" "<<input_starts.size()<<"\n";
    for(size_t k=0; k < paths.size(); k++)
    {
        std::cout<<"Agent "<<k<<"\n";
        std::cout<<"Start ("<<input_starts[k].first<<","<<input_starts[k].second<<") "<<G.get_id(input_starts[k].first, input_starts[k].second)<<"\n";
        for(auto g:input_all_goals[k])
            std::cout<<G.get_id(g.first, g.second)<<" ";
        //for(auto s:paths[k])
        //    std::cout<<s.location<<" ";

        std::cout<<std::endl;
    }
}
