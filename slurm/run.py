import subprocess

def run_experiment(cores, template, experiment, experiment_type, extra_params=[], exec_script='ssbatch'):
    try:
        bin_folder='../bin'
        str_params_list = [str(item) for item in extra_params]
        str_params = " ".join(str_params_list)
        experiment_name=experiment+"_"+experiment_type
        job_name = experiment_name+"-job"
        experiment_script=bin_folder+'/'+experiment_name
        # Use subprocess.run() to run the Bash script
        command = [exec_script,
                    '--cores-per-socket='+str(cores),
                    '--cpus-per-task='+str(cores),
                    '--job-name='+job_name,
                    template,
                    experiment_script,
                    str_params]
        str_command = " ".join(command)
        process = subprocess.run([str_command],shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        if process.returncode != 0:
            raise Exception("return code was: "+str(process.returncode))
           
        # Get the job ID from the output (assuming the job ID is the first word in the output)
        process_results = process.stdout.split()
        job_id = process_results[len(process_results)-1].strip()
        return job_id

    except subprocess.CalledProcessError as e:
        print("Error executing the script: {"+str(e)+"}")


def run_experiments(experiment_type='time', experiment='3mm'):
    cores_list = [1, 2, 4, 8, 10]
    # cores_list=[1, 2]
    tile_sizes = range(16,2016,16)
    # tile_sizes=range(16,32,16)
    template = 'experiment.sh'
    created_jobs=[]
    
    # baseline
    job = run_experiment(1, template, experiment, experiment_type,[])
    created_jobs.append(job)

    # parallel baseline
    job = run_experiment(1, template, experiment+"_parallel", experiment_type,[])
    created_jobs.append(job)

    for core in cores_list:
        job = run_experiment(core, template, experiment+"_parallel", experiment_type,[])
        created_jobs.append(job)    
        for tile_size in tile_sizes:
            job = run_experiment(core, template, experiment+'_tiled-lv1', experiment_type, [tile_size])
            created_jobs.append(job)

            job = run_experiment(core, template, experiment+'_tiled-lv2', experiment_type, [tile_size])
            created_jobs.append(job)
    print(str(created_jobs))


if __name__ == "__main__":
    run_experiments()
    # run_experiments('papi')
