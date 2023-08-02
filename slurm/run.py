import subprocess


def run_experiment(cores, template, experiment, experiment_type, extra_params=[], exec_script='ssbatch'):
    try:
        bin_folder='../bin'
        str_params = [str(item) for item in extra_params]

        # Use subprocess.run() to run the Bash script
        subprocess.run([exec_script,
                        '--cores-per-socket='+str(cores), template,
                        bin_folder+'/'+experiment+"_"+experiment_type,
                        " ".join(str_params)], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error executing the script: {e}")


def run_experiments(experiment='3mm',experiment_type='time'):
    # cores_list = [1, 2, 4, 8, 10]
    cores_list=[1,2]
    # tile_sizes = range(16,2016,16)
    tile_sizes=range(16,32,16)
    template = 'experiment.sh'
    for core in cores_list:
        run_experiment(core, template, experiment, experiment_type,[])
        for tile_size in tile_sizes:
            run_experiment(core, template, experiment+'_tiled-lv1', experiment_type, [tile_size])
            run_experiment(core, template, experiment+'_tiled-lv2', experiment_type, [tile_size])


if __name__ == "__main__":
    run_experiments()
    # run_experiments('papi')
