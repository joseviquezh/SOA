#include "Writer.h"




void generateLatexForRangeTest(){
    Range* rangeList=rangeBuilder(1,2);
    char* line =generateLatexForRange(rangeList);
    printf("%s",line);
}


void generateLatexForTaskTest(){
    Task* taskList=taskBuilder(
        "TaskTest",rangeBuilder(2,2)
    );
    addRangeToList(5,8,taskList->listOfRanges);
    addRangeToList(10,12,taskList->listOfRanges);
    char* line =generateLatexForTask(taskList);
    printf("%s",line);
}

void generateLatexForAlgorithmTest(){
    Algorithm* algorithm=algorithmBuilder("AlgorithmNameTest",
    20,taskBuilder(
        "TaskTest",rangeBuilder(2,2)
    ));
    
    addTaskToList("TaskTest2",rangeBuilder(3,5),algorithm->listOfTasks);

    addTaskToList("TaskTest3",rangeBuilder(1,2),algorithm->listOfTasks);

    addTaskToList("TaskTest4",rangeBuilder(2,3),algorithm->listOfTasks);
    
    char* line =generateLatexForAlgorithm(algorithm);//(algorithm);
    printf("%s",line);
}



void generateLatexForBeamerTestAll(){
    BeamerPresentation* presentation=beamerBuilder();
    insertNewAlgorithm(presentation,"Todos",40);

    insertNewTask(presentation,"Todos","RM Process 1");
    insertNewSequenceRange(presentation,"Todos","RM Process 1",3,5,40);

    insertNewTask(presentation,"Todos","RM Process 2");
    insertNewSequenceRange(presentation,"Todos","RM Process 2",2,5,40);

    insertNewTask(presentation,"Todos","No RM 1");
    insertNewSequenceRange(presentation,"Todos","No RM 1",7,10,40);

    insertNewTask(presentation,"Todos","No RM 2");
    insertNewSequenceRange(presentation,"Todos","No RM 2",3,5,40);

    insertNewTask(presentation,"Todos","Otro P1");
    insertNewSequenceRange(presentation,"Todos","Otro P1",3,8,40);


    insertNewTask(presentation,"Todos","Otro P2");
    insertNewSequenceRange(presentation,"Todos","Otro P2",3,4,40);



    generateLatexForBeamer(presentation);
}





void generateLatexForBeamerTest(){
    BeamerPresentation* presentation=beamerBuilder();
    insertNewAlgorithm(presentation,"AlgorithmNameTest",20);

    insertNewTask(presentation,"AlgorithmNameTest","TaskTest");
    
    insertNewRange(presentation,"AlgorithmNameTest","TaskTest",15,17);
    insertNewRange(presentation,"AlgorithmNameTest","TaskTest",2,3);
    insertNewRange(presentation,"AlgorithmNameTest","TaskTest",4,7);
    insertNewRange(presentation,"AlgorithmNameTest","TaskTest",9,13);


    insertNewTask(presentation,"AlgorithmNameTest","TaskTest2");
    insertNewRange(presentation,"AlgorithmNameTest","TaskTest2",15,17);
    insertNewRange(presentation,"AlgorithmNameTest","TaskTest2",2,3);
    insertNewRange(presentation,"AlgorithmNameTest","TaskTest2",4,7);
    insertNewRange(presentation,"AlgorithmNameTest","TaskTest2",9,13);




    insertNewAlgorithm(presentation,"AlgorithmNameTest2",25);

    insertNewTask(presentation,"AlgorithmNameTest2","TaskTest");
    
    insertNewRange(presentation,"AlgorithmNameTest2","TaskTest",15,17);
    insertNewRange(presentation,"AlgorithmNameTest2","TaskTest",2,3);
    insertNewRange(presentation,"AlgorithmNameTest2","TaskTest",4,7);
    insertNewRange(presentation,"AlgorithmNameTest2","TaskTest",9,13);


    insertNewTask(presentation,"AlgorithmNameTest2","TaskTest2");
    insertNewRange(presentation,"AlgorithmNameTest2","TaskTest2",15,17);
    insertNewRange(presentation,"AlgorithmNameTest2","TaskTest2",2,3);
    insertNewRange(presentation,"AlgorithmNameTest2","TaskTest2",4,7);
    insertNewRange(presentation,"AlgorithmNameTest2","TaskTest2",9,13);

    insertNewAlgorithm(presentation,"AlgorithmNameTest3",25);

    insertNewTask(presentation,"AlgorithmNameTest3","TaskTest");
    
    insertNewRange(presentation,"AlgorithmNameTest3","TaskTest",15,17);
    insertNewRange(presentation,"AlgorithmNameTest3","TaskTest",2,3);
    insertNewRange(presentation,"AlgorithmNameTest3","TaskTest",4,7);
    insertNewRange(presentation,"AlgorithmNameTest3","TaskTest",9,13);

    insertNewTask(presentation,"AlgorithmNameTest3","TaskTest2");
    insertNewRange(presentation,"AlgorithmNameTest3","TaskTest2",15,17);
    insertNewRange(presentation,"AlgorithmNameTest3","TaskTest2",2,3);
    insertNewRange(presentation,"AlgorithmNameTest3","TaskTest2",4,7);
    insertNewRange(presentation,"AlgorithmNameTest3","TaskTest2",9,13);

    generateLatexForBeamer(presentation);
}

int main (int argc, char *argv[])
{

    srand(0);
    printf("\nTesting latex for range\n");
    generateLatexForRangeTest();

    printf("\nTesting latex for Task\n");
    generateLatexForTaskTest();

    printf("\nTesting latex for Algorithm\n");
    generateLatexForAlgorithmTest();

    printf("\nTesting Saving File\n");
    generateLatexForBeamerTestAll();
}