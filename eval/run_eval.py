import os
import argparse
from pathos.multiprocessing import ProcessPool as Pool
import subprocess
import json

def run_tests(args):
    num_core = args.num_workers
    parallel_pool = Pool(num_core)
    with open(args.config_name) as f:
        data = json.load(f)
    print('start testing with ' + str(num_core) + ' processes...')
    all_results = []
    for d in data:
        for cutoff in d['cutoff']:
            for sim_window in d['simulation_window']:
                for plan_window in d['planning_window']:
                    for s in d['seeds']:
                        for n in d['num_agents']:
                            if sim_window > plan_window:
                                continue
                            result = parallel_pool.apipe(run_1_test_wrapper, args.printInfo, d['map_name'], d['instances_name'], s, n, cutoff, sim_window, plan_window)
                            all_results.append(result)

    total_jobs = len(all_results)
    jobs_completed = 0
    while len(all_results) > 0:
        for i in range(len(all_results)):
            if all_results[i].ready():
                jobs_completed += 1
                print("{} / {}".format(jobs_completed, total_jobs))
                all_results[i].get()
                all_results.pop(i)
                break

    parallel_pool.close()


def run_1_test_wrapper(print_info, mapName, instances_name, seed, num_agents, cutoff, simulation_window, planning_window):
    s = f"./lifelong --agentNum {num_agents} --map {mapName} --instance {instances_name} --scenario PreKIVA --simulation_time 500 --log 1 --output test --seed {seed} -t {cutoff} --planning_window {planning_window} --simulation_window {simulation_window} --screen 0"
    print(s)
    if print_info:
        subprocess.run(s, stderr=subprocess.STDOUT, shell=True)
    else:
        try:
            devNull = open('/dev/null', 'w')
            subprocess.run(s, stderr=devNull, shell=True)
        except Exception:
            Warning('cannot mute info, reset printInfo to True')
            subprocess.run(s, stderr=subprocess.STDOUT, shell=True)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-c', '--config_name', default='config.json')
    parser.add_argument('-w', '--num_workers', default=1, type=int)
    parser.add_argument("--printInfo", default=False, type=bool)
    args = parser.parse_args()
    run_tests(args)
