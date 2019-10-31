#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sisim.h"

int main(int argc, char *argv[])
{
  int i;
  clock_t start,stop;
  double total_time;
  
  start = clock();
  for(i=1;i<11;i++){
    /* Simulation ID, start time, stop time, seed, initial capacity */
    SiSim* s=new_SiSim(i, 0.0, 10000.0, i, 10000);
    /* Probability of infection, contact rate */
    SiSim_initialize(s,0.003,1);
    /* Population size, initial infected */
    SiSim_activate(s,10000,1);                 
    Sim_do_all((Sim*)s);
    delete_SiSim(s);
  };
  stop = clock();
  total_time += ((double) (stop - start)) / CLOCKS_PER_SEC;
  printf("   %-2.6f\n",((double) (stop - start)) / CLOCKS_PER_SEC );
  return 0;
}
