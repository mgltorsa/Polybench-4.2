#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

void logger(const char *format, ...);

char* get_experiment_type(); 

char* slurm_get_node_id();

void printe(char *algorithm, char *version, long size, int tile_size, int cache_size);

#endif