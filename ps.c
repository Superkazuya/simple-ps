#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_DIM 3
#define NUM_AGENTS 20
#define MAX_ITER 1000
#define BIGNUMBER 65535

#define RAND(a, b) (((a)-(b))*(2.0*rand()/RAND_MAX-1)+(b))

int g_best_pos[NUM_DIM];
int g_best_val = 1<<30;

typedef struct
{
  int pos_best[NUM_DIM];
  int pos_curr[NUM_DIM];
  int velocity[NUM_DIM];
  int val_best;
} agent;

//int domain[NUM_DIM][2];

static void eval(agent* );
static void init_agents(agent* );
static void init_one(agent* p);
static void update(agent* );

void
eval(agent* array)
{
  unsigned int i, j;
  int val;
  agent* p;
  for(i = 0; i < NUM_AGENTS; i++)
  {
    val = 0;
    p = array+i;
    for(j = 0; j < NUM_DIM; j++)
      val += abs(p->pos_curr[j]-50);
    if(val < p->val_best)
    {
      p->val_best = val;
      memcpy(p->pos_best, p->pos_curr, sizeof(p->pos_best));
      if(val < g_best_val)
      {
	g_best_val = val;
	memcpy(g_best_pos, p->pos_curr, sizeof(g_best_pos));
      }
    }
  }
}

void
init_one(agent* p)
{
  unsigned j;
  for(j = 0; j < NUM_DIM; j++)
  {
    p->pos_curr[j] = RAND(100, 0);
    p->velocity[j] = RAND(10, 0);
  }
}

void
init_agents(agent* array)
{
  unsigned int i, j;
  for(i = 0; i < NUM_AGENTS; i++)
  {
    array[i].val_best = g_best_val;
    init_one(array+i);
  }
}

void
update(agent* array)
{
  unsigned int i, j;
  static float inherit_weight = 1;
  float weight1;
  float weight2;
  for(i = 0; i < NUM_AGENTS; i++)
  {
    weight1 = RAND(2, 0);
    weight2 = RAND(2, 0);
    for(j = 0; j < NUM_DIM; j++)
    {
      array[i].velocity[j] = inherit_weight*array[i].velocity[j] 
	+ weight1*(array[i].pos_best[j] - array[i].pos_curr[j])
	+ weight2*(g_best_pos[j] - array[i].pos_curr[j]);
      array[i].pos_curr[j] += array[i].velocity[j];
      //check domain
      if(abs(array[i].velocity[j]) >= BIGNUMBER || abs(array[i].pos_curr[j]) >= BIGNUMBER)
	init_one(array+i);
    }
  }
  inherit_weight *= 0.9999;
}

int
main()
{
  agent agents[NUM_AGENTS];
  unsigned int i, counter = 0;
  init_agents(agents);
  while(counter++ < MAX_ITER && g_best_val >= 0.001)
  {
    eval(agents);
    update(agents);
    printf("@iteration %d, global best@: (", counter);
    for(i = 0; i < NUM_DIM; i++)
      printf("%d ", g_best_pos[i]);
    printf("), global best: %d\n", g_best_val);
  }
  exit(EXIT_SUCCESS);
}

