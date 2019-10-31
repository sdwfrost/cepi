#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sirsim.h"

int main(int argc, char *argv[])
{
  int i;
  int runs;
  clock_t start,stop;
  double total_time;
  SirSim* s;
  
  runs=100;
  start = clock();
  for(i=1;i<(runs+1);i++){
    printf("Starting run %i of %i\n",i,runs);
    /* Simulation ID, start time, stop time, seed, initial capacity, dynamics filename, phylogeny filename */
    s=new_SirSim(i, 0.0, 40.0, i, 10000, "dyn.txt", "phylo.txt");
    /* Probability of infection, contact rate, recovery rate*/
    SirSim_initialize(s, 1.0, 2, 0.3);
    /* Population size, initial infected */
    SirSim_activate(s, 10000, 1, 0);                 
    Sim_do_all((Sim*)s);
    printf("Finishing run %i of %i\n",i,runs);
    delete_SirSim(s);
  };
  stop = clock();
  total_time=0;
  total_time += ((double) (stop - start)) / CLOCKS_PER_SEC;
  printf("   %-2.6f\n",((double) (stop - start)) / CLOCKS_PER_SEC );
  return 0;
}
