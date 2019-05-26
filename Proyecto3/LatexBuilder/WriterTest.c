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
    generateLatexForBeamerTest();
}