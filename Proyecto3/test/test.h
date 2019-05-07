// Custom includes
#include "../task.h"

// Main tests
int TestRM (Task * tasks, int tasks_length);
int TestEDF (Task * tasks, int tasks_length);

// For presentation & helpers
double CalculateMiu (Task * tasks, int tasks_length); // To show in the presentation.
double CalculateMaxUtilizationRM (int tasks_length);
double CalculateMaxUtilizationEDF ();