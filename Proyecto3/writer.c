#include "writer.h"




Range * rangeBuilder(int start,int end){
 if(start>end){
    printf("ERROR: RANGE START CAN NOT BET MORE END--- START: %d    END: %d",start,end);
    return NULL;       
 }
 Range* node=(Range*) malloc(sizeof (Range));

 node->start=start;
 node->end=end;
 node->next=NULL;
 return node;
}
TaskBeamer * taskBuilder(char * name, Range* listOfRanges){
 TaskBeamer* node=(TaskBeamer*) malloc(sizeof (TaskBeamer));
 strcpy(node->title,name);
 node->listOfRanges=listOfRanges;
 node->next=NULL;
 return node;
}

Algorithm * algorithmBuilder(char * name, int totalTime, TaskBeamer* listOfTasks){
 Algorithm* node=(Algorithm*) malloc(sizeof (Algorithm));
 strcpy(node->name,name);
 node->listOfTasks=listOfTasks;
 node->totalTime=totalTime;
 node->next=NULL;
 return node;
}

BeamerPresentation * beamerBuilder(){
    BeamerPresentation* node=(BeamerPresentation*) malloc(sizeof (BeamerPresentation));
    node->listOfAlgorithms=NULL;
    return node;
}

Range * addRangeToList(int start,int end, Range* listOfRanges){
  if(start>40||end>40)return NULL;
  Range* cursor=listOfRanges;
  while(cursor->next!=NULL){
      cursor=cursor->next;
  }
  cursor->next=rangeBuilder(start,end);
  return cursor->next;
}

TaskBeamer * addTaskToList(char * name, Range * listOfRanges, TaskBeamer* listOfTasks){
  TaskBeamer* cursor=listOfTasks;
  while(cursor->next!=NULL){
      cursor=cursor->next;
  }
  cursor->next=taskBuilder(name,listOfRanges);
  return cursor->next;
}

Algorithm * addAlgorithmToList(char * name,int totalTime, TaskBeamer* listOfTasks, Algorithm* listOfAlgorithms){
  Algorithm* cursor=listOfAlgorithms;
  while(cursor->next!=NULL){
      cursor=cursor->next;
  }
  cursor->next=algorithmBuilder(name, totalTime,listOfTasks);
  return cursor->next;
}


char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}


char* getFullTextFromFile(char* fileName){
    FILE    *infile;
    char    *buffer;
    long    numbytes;
    /* open an existing file for reading */
    infile = fopen(fileName, "r");
    if(infile == NULL)
    {
        printf("File does not exits: %s", fileName);
        return NULL;
    }
    /* Get the number of bytes */
    fseek(infile, 0L, SEEK_END);
    numbytes = ftell(infile);

    /* reset the file position indicator to
    the beginning of the file */
    fseek(infile, 0L, SEEK_SET);

    /* grab sufficient memory for the
    buffer to hold the text */
    buffer = (char*)calloc(numbytes, sizeof(char));

    /* memory error */
    if(buffer == NULL)
        return NULL;

    /* copy all the text into the buffer */
    fread(buffer, sizeof(char), numbytes, infile);
    fclose(infile);

    return buffer;
}


void writeFile(char* fileName, char* content){
    // Open a file for writing.
    // (This will replace any existing file. Use "w+" for appending)
    FILE *file = fopen(fileName, "w");
    int results = fputs(content, file);
    if (results == EOF) {
        printf("Fail saving file %s", fileName);
    }
    fclose(file);
}





Algorithm* pickAlgorithmByName(BeamerPresentation*presentation, char* nameOfAlgorithm){
    Algorithm* cursor=presentation->listOfAlgorithms;
    while(cursor!=NULL){
        if(strcmp(cursor->name,nameOfAlgorithm)==0){
            return  cursor;
        }
        cursor=cursor->next;
    }
    printf("ERROR!!!! THERE IS NO ALGORITHM CALLED: %s",nameOfAlgorithm);
    return NULL;
}
TaskBeamer* pickTaskByName(Algorithm * algorithm, char* nameOfTask){
    TaskBeamer* cursor=algorithm->listOfTasks;
    while(cursor!=NULL){
        if(strcmp(cursor->title,nameOfTask)==0){
            return  cursor;
        }
        cursor=cursor->next;
    }
    if(algorithm!=NULL)
        printf("ERROR!!!! THERE IS NO TASK CALLED: %s \n IN THE ALGORITHM CALLED: %s",nameOfTask, algorithm->name);
    else
        printf("ERROR!!!! Algoritm can not be null");
    return NULL;
}



void insertNewSequenceRange(BeamerPresentation* presentation,char* algorithmName,char* taskName,int cTime,int period,int max){
    Algorithm* algorithm=pickAlgorithmByName(presentation,algorithmName);
    TaskBeamer* task_beamer=pickTaskByName(algorithm,taskName);
    int indicator=period;
    while (indicator+cTime<max)
    {
        if(task_beamer->listOfRanges==NULL){
             task_beamer->listOfRanges=rangeBuilder(indicator,indicator+cTime);
        }else{
             addRangeToList(indicator,indicator+cTime,task_beamer->listOfRanges);
        }
        indicator+=period;
    }


}


void insertNewRange(BeamerPresentation* presentation,char* algorithmName,char* taskName,int start,int end){
    Algorithm* algorithm=pickAlgorithmByName(presentation,algorithmName);
    TaskBeamer* task_beamer=pickTaskByName(algorithm,taskName);
    if(task_beamer->listOfRanges==NULL){
        task_beamer->listOfRanges=rangeBuilder(start,end);
    }else{
        addRangeToList(start,end,task_beamer->listOfRanges);
    }
}

void insertNewTask(BeamerPresentation* presentation,char* algorithmName,char* newTaskName){
    Algorithm* algorithm=pickAlgorithmByName(presentation,algorithmName);
    if(algorithm->listOfTasks==NULL){
        algorithm->listOfTasks=taskBuilder(newTaskName,NULL);
    }else{
        addTaskToList(newTaskName,NULL,algorithm->listOfTasks);
    }
}


void insertNewAlgorithm(BeamerPresentation* presentation,char* algorithmName,int totalTime){
    if(presentation->listOfAlgorithms==NULL){
        presentation->listOfAlgorithms=algorithmBuilder(algorithmName,totalTime,NULL);
    }else
    {
        addAlgorithmToList(algorithmName,totalTime,NULL,presentation->listOfAlgorithms);
    }
}









char * generateLatexForRange(Range * range){
    char * result;
    char str[10];
    char * extraLine="\n\\ganttbar[inline, bar/.append style={fill=<color>}]{}{<start>}{<end>}";
    /*if(range==NULL){
        printf("ERROR: RANGE EXPECTED BUT GOT NULL");
        return NULL;
    }
    if(range->start<0&&range->end<0){
        if(range->start<-1000&&range->end<-1000)
        {
            result=str_replace(extraLine,"<color>","red!20, rounded corners=7pt, draw=none");
            sprintf(str, "%d", range->start*-1-1000);
            result=str_replace(result,"<start>",str);
            sprintf(str, "%d", range->end*-1-1000);
            result=str_replace(result,"<end>",str);
        }else
        {
            result=str_replace(extraLine,"<color>","yellow!20, draw=none");
            sprintf(str, "%d", range->start*-1);
            result=str_replace(result,"<start>",str);
            sprintf(str, "%d", range->end*-1);
            result=str_replace(result,"<end>",str);
        }
    }
    else{*/
        printf("Start: %d    End: %d",range->start,range->end);
        result=str_replace(extraLine,"<color>","aclv");
        sprintf(str, "%d", range->start);
        result=str_replace(result,"<start>",str);
        sprintf(str, "%d", range->end);
        result=str_replace(result,"<end>",str);
    //}
    return result;
}

int getRandomRGBValue(){
    int r = rand();
    return r%256;
}

char *  generateLatexForTask(TaskBeamer*task_beamer){
    char * result;
    char str[10];
    char * mainLine="\n\\definecolor{aclv}{RGB}{<red>,<green>,<blue>} \n \\ganttbar[ bar/.append style={fill=<color>}]{<task_name>}{<start>}{<end>}";
    char * extraLine="\n\\ganttbar[inline, bar/.append style={fill=<color>}]{}{<start>}{<end>}";

    if(task_beamer==NULL){
        printf("ERROR: Expected task but got NULL ptr");
        return NULL;
    }else if(task_beamer->listOfRanges==NULL){
        printf("ERROR: Range queue is empty for task %s",task_beamer->title);
        return NULL;
    }

    result=str_replace(mainLine,"<color>","aclv");

    sprintf(str, "%d", getRandomRGBValue());
    result=str_replace(result,"<red>",str);

    sprintf(str, "%d", getRandomRGBValue());
    result=str_replace(result,"<green>",str);


    sprintf(str, "%d", getRandomRGBValue());
    result=str_replace(result,"<blue>",str);

    result=str_replace(result,"<task_name>",task_beamer->title);

    sprintf(str, "%d", task_beamer->listOfRanges->start);

    result=str_replace(result,"<start>",str);
    sprintf(str, "%d", task_beamer->listOfRanges->end);
    result=str_replace(result,"<end>",str);
    Range*cursor=task_beamer->listOfRanges->next;

printf("\n----------------------------------IN: generateLatexForTask\n");
    while(cursor!=NULL){
        result=strcat(result,generateLatexForRange(cursor));
        cursor=cursor->next;
    }
    result=strcat(result,"\\\\\n");

printf("\n----------------------------------IN: generateLatexForTask\n%s",result);
    return result;
}




char* generateLatexForAlgorithm(Algorithm* algorithm){
    char str[10];
    char* result;
    char* taskString;
    char cwd[2000];
    int limitPerFrame=5;
    
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        result=getFullTextFromFile(strcat(cwd,AlgorithmTemplate));
    } else {
        return NULL;
    }

    if(algorithm==NULL){
        printf("ERROR: Expected algortim but git NULL ptr");
        return NULL;
    }else if(algorithm->listOfTasks==NULL){
        printf("ERROR: Task queue is empty for algorithm %s",algorithm->name);
        return NULL;
    }

    result=str_replace(result,"<algorithm_name>",algorithm->name);
    sprintf(str, "%d", algorithm->totalTime);
    result=str_replace(result,"<total_time_units>",str);

    TaskBeamer* cursor=algorithm->listOfTasks;

printf("\n----------------------------------IN: generateLatexForAlgorithm line 344\n");
    taskString=generateLatexForTask(cursor);
printf("\n----------------------------------IN: generateLatexForAlgorithm line 346\n");
    
    cursor=cursor->next;
    while(cursor!=NULL){
printf("\n----------------------------------LOOP: generateLatexForAlgorithm line 342 %s %s\n",algorithm->name,cursor->title);
        taskString=strcat(taskString,generateLatexForTask(cursor));
        cursor=cursor->next;
        if(limitPerFrame--<=0){
            break;
        }
    }
    printf("\n----------------------------------IN: generateLatexForAlgorithm line 364\n");
    result=str_replace(result,"<ganttbars>",taskString);
printf("\n----------------------------------IN: generateLatexForAlgorithm line 366\n");
    if(limitPerFrame+1<=0&&cursor!=NULL){

printf("\n----------------------------------IN:RecursiveStep\n");
        result=strcat(result,generateLatexForAlgorithm(algorithmBuilder(
            algorithm->name,algorithm->totalTime,cursor
        )));
    }
    return result;
}

void calculateLeisureRanges(Algorithm* algorithm_){

    if(algorithm_==NULL){
        printf("ERROR: Algoritm is null in calculateLeisureTanges.");
    }else if(algorithm_->listOfTasks==NULL){
        printf("ERROR: There is no task for algoritmhm: %s",algorithm_->name);
    }

    int local_Flags[100];
    int i,j;
    TaskBeamer* cursorTask=algorithm_->listOfTasks;
    Range* cursorRange;

    for(i=0;i<algorithm_->totalTime+1;i++){
        local_Flags[i]=0;
    }


    while (cursorTask!=NULL)
    {
        cursorRange=cursorTask->listOfRanges;
        while (cursorRange!=NULL)
        {
            for (i = cursorRange->start; i <= cursorRange->end; i++)
            {
                printf("i=%d\n",i);
                local_Flags[i]=1661;
            }
            cursorRange=cursorRange->next;
        }
        cursorTask=cursorTask->next;
    }

    for(i=1;i<algorithm_->totalTime+1;i++){
        if(local_Flags[i]!=1661){
            cursorTask=algorithm_->listOfTasks;
            while (cursorTask!=NULL){
                addRangeToList(-i,-i,cursorTask->listOfRanges);
                cursorTask=cursorTask->next;
            }
        }
    }

/*
    int * flags=(int*)malloc(sizeof(int) * algorithm_->totalTime+1);
    int i,j,k=1000;

    TaskBeamer* cursorTask=algorithm_->listOfTasks;
    Range* cursorRange;


    printf("\n------------------------------------------------aa");
    for(i=0;i<algorithm_->totalTime+1;i++){
        flags[i]=0;
        printf("Flag[%d]=%d\n",i,flags[i]);
    }

    while (cursorTask!=NULL)
    {
        cursorRange=cursorTask->listOfRanges;
        while (cursorRange!=NULL)
        {
            for (i = cursorRange->start; i <= cursorRange->end; i++)
            {
                if(flags[i]==k){
                    flags[i]=999;
                }else if(flags[i]!=999){
                    flags[i]=k;
                } 
            }
            cursorRange=cursorRange->next;
        }
        
        for(i=1;i<=algorithm_->totalTime;i++){
            if(flags[i]==999){
               flags[i]=k;
               if(cursorTask->listOfRanges==NULL){
                   cursorTask->listOfRanges=rangeBuilder(-i-1000,-i-1000);
               }
               else{
                  addRangeToList(-i-1000,-i-1000,cursorTask->listOfRanges);
                }
            }
        }

        k+=1000;
        cursorTask=cursorTask->next;
    }
    
    printf("\n------------------------------------------------bb");
    for(i=0;i<algorithm_->totalTime+1;i++){
        printf("Flag[%d]=%d\n",i,flags[i]);
    }

    for(i=1;i<=algorithm_->totalTime;i++){
        if(flags[i]<999){
            cursorTask=algorithm_->listOfTasks;
            while (cursorTask!=NULL){

    printf("\n------------------------------------------------ADDING %d %d",-i,-i);
               if(cursorTask->listOfRanges==NULL){
                   cursorTask->listOfRanges=rangeBuilder(-i,-i);
               }
               else
                {
                    addRangeToList(-i,-i,cursorTask->listOfRanges);
                }
               cursorTask=cursorTask->next;
            }
        }
    }

    printf("\n------------------------------------------------cc");
    for(i=0;i<algorithm_->totalTime+1;i++){
        printf("Flag[%d]=%d\n",i,flags[i]);
    }
    free(flags);
    flags=NULL;
    */
}


void generateLatexForBeamer(BeamerPresentation * beamerBuilder ){
    char* algorithmsFile;
    char* outPutPath;
    char cwd[2000];

    Algorithm* cursor=beamerBuilder->listOfAlgorithms;
    if(cursor==NULL){
        printf("ERROR!! THERE IS NO ALGORITM IN BEAMER STRUCT");
        return;
    }
    if(detectLeisureTime==1){
        calculateLeisureRanges(cursor);
    }
    
printf("\n----------------------------------GenerateLatexForAlgoritm\n");
    algorithmsFile=generateLatexForAlgorithm(cursor);
    cursor=cursor->next;
    while(cursor!=NULL){
printf("\n----------------------------------LOOP:Calculate leisure time\n");
    
        if(detectLeisureTime==1){
            calculateLeisureRanges(cursor);
        }
        
printf("\n----------------------------------LOOP: GenerateLatexForAlgoritm\n");
        algorithmsFile=strcat(algorithmsFile,generateLatexForAlgorithm(cursor));
        cursor=cursor->next;
    }

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        outPutPath=strcat(cwd,OUTPUT);
    } else {
        printf("Error traying to read local path");
    }
    printf("%s\n%s\n",outPutPath,algorithmsFile);

    writeFile(outPutPath,algorithmsFile);

}
