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
        for s in d['seeds']:
            for n in d['num_agents']:
                result = parallel_pool.apipe(run_1_test_wrapper, args.printInfo, d['map_name'], args.instances_name, s, n, args.cutoff)
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


def run_1_test_wrapper(print_info, mapName, instances_name, seed, num_agents, cutoff):
    s = f"./lifelong --agentNum {num_agents} --map {mapName} --instance {instances_name} --scenario PreKIVA --simulation_time 510 --log 1 --output test --seed {seed} -t {cutoff}"
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
    parser.add_argument('-t', '--cutoff', default=60, type=int)
    parser.add_argument('-c', '--config_name', default='config.json')
    parser.add_argument('-i', '--instances_name', default='instances.json')
    parser.add_argument('-w', '--num_workers', default=1, type=int)
    parser.add_argument("--printInfo", default=True, type=bool)
    args = parser.parse_args()
    run_tests(args)
