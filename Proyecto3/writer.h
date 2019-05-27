#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

static const char AlgorithmTemplate[] = "/algorithmTemplate.tex";
static const char OUTPUT[] =   "/algorithmsFile.tex";

static const int detectLeisureTime=0;
struct range
{
    int start;
    int end;
    struct range * next;
};
typedef struct range Range;

struct task
{
    char title[100];
    Range * listOfRanges;
    struct task * next;
};
typedef struct task TaskBeamer;

struct algorithm
{
    char name[100];
    int totalTime;
    struct task * listOfTasks;
    struct algorithm * next;
};
typedef struct algorithm Algorithm;



struct beamerBuilder
{
   Algorithm * listOfAlgorithms;
};
typedef struct beamerBuilder BeamerPresentation;




Algorithm* pickAlgorithmByName(BeamerPresentation*presentation, char* nameOfAlgorithm);
TaskBeamer* pickTaskByName(Algorithm * algorithm, char* nameOfTask);

Range * rangeBuilder(int start,int end);
TaskBeamer * taskBuilder(char * name, Range* listOfRanges);
Algorithm * algorithmBuilder(char * name, int totalTime, TaskBeamer* listOfTasks);
BeamerPresentation * beamerBuilder();
Range * addRangeToList(int start,int end, Range* listOfRanges);
TaskBeamer * addTaskToList(char * name, Range * listOfRanges, TaskBeamer* listOfTasks);
char *str_replace(char *orig, char *rep, char *with);
char* getFullTextFromFile(char* fileName);
void writeFile(char* fileName, char* content);
char * generateLatexForRange(Range*range);
char * generateLatexForTask(TaskBeamer*task);
char* generateLatexForAlgorithm(Algorithm* algorithm);
void calculateLeisureRanges(Algorithm* algorithm_);
void generateLatexForBeamer(BeamerPresentation * beamerBuilder );


void insertNewSequenceRange(BeamerPresentation* presentation,char* algorithmName,char* taskName,int cTime,int period,int max);
void insertNewRange(BeamerPresentation* presentation,char* algorithmName,char* taskName,int start,int end);
void insertNewTask(BeamerPresentation* presentation,char* algorithmName,char* newTaskName);
void insertNewAlgorithm(BeamerPresentation* presentation,char* algorithmName,int totalTime);

