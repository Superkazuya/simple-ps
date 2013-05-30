#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_DIM 3
#define NUM_AGENTS 100
#define MAX_ITER 10000

#define RAND(a, b) (((a)-(b))*(2.0*rand()/RAND_MAX-1)+(b))
#define NORM    ((double)rand()/RAND_MAX)

int g_best_pos[NUM_DIM];
int g_best_val = -1<<31;

typedef struct
{
  int pos_best[NUM_DIM];
  int pos_curr[NUM_DIM];
  int velocity[NUM_DIM];
  int val_best;
} agent;

int domain[NUM_DIM][2];

static void eval(agent* );
static void init_agents(agent* );

void
eval(agent* array)
{
  unsigned int i, j, val;
  agent* p;
  for(i = 0; i < NUM_AGENTS; i++)
  {
    val = 0;
    p = array+i;
    for(j = 0; j < NUM_DIM; j++)
      val -= (p->pos_curr[j]-50)*(p->pos_curr[j]-50);
    if(val > p->val_best)
    {
      p->val_best = val;
      memcpy(p->pos_best, p->pos_curr, sizeof(p->pos_best));
      if(val > g_best_val)
      {
	g_best_val = val;
	memcpy(g_best_pos, p->pos_curr, sizeof(g_best_pos));
      }
    }
  }
}

void
init_agents(agent* array)
{
  unsigned i, j;
  for(i = 0; i < NUM_AGENTS; i++)
  {
    array[i].val_best = 0;
    for(j = 0; j < NUM_DIM; j++)
    {
      array[i].pos_curr[j] = RAND(100, 0);
      array[i].velocity[j] = RAND(10, 0);
    }
  }
}

void
update_velocity(agent* array)
{
  unsigned i, j;
  static float inherit_weight = 1;
  float weight1;
  float weight2;
  for(i = 0; i < NUM_AGENTS; i++)
  {
    weight1 = RAND(2, 0);
    weight2 = RAND(2, 0);
    for(j = 0; j < NUM_DIM; j++)
      array[i].velocity[j] = inherit_weight*array[i].velocity[j] 
	+ weight1*(array[i].pos_best[j] - array[i].pos_curr[j])
	+ weight2*(g_best_pos[j] - array[i].pos_curr[j]);
  }
  inherit_weight *= 0.99;
}

void
update_position(agent* array)
{
  unsigned i, j;
  for(i = 0; i < NUM_AGENTS; i++)
    for(j = 0; j < NUM_DIM; j++)
      array[i].pos_curr[j] += array[i].velocity[j];
}

int
main()
{
  agent agents[NUM_AGENTS];
  unsigned int i, counter = 0;
  while(counter++ < MAX_ITER && g_best_val != 0)
  {
    init_agents(agents);
    eval(agents);
    update_velocity(agents);
    update_position(agents);
    printf("@iteration %d, global best position: (", counter);
    for(i = 0; i < NUM_DIM; i++)
      printf("%d ", g_best_pos[i]);
    printf("), global best: %d\n", g_best_val);
  }
  exit(EXIT_SUCCESS);
}

