// Custom includes
#include "../task.h"
#include "../queue-item.h"

enum Algorithm {RM, EDF, LLF};

void InitScheduler (int p_tasks_length, Task * p_tasks, int p_algorithm);
int RunScheduling ();

QueueItem * GetHistory ();
int GetHistorySize ();
int HasMissedDeadlines ();
