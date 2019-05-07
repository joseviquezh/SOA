// Custom includes
#include "../task.h"

enum Algorithm {RM, EDF, LLF};

void InitScheduler (int p_tasks_length, Task * p_tasks, int p_algorithm);
int RunScheduling ();