// System includes
#include <stdio.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/reg.h>
#include <signal.h>
#include <string.h>

// Custom includes
#include "utilities/syscall.h"
#include "utilities/table.h"

// Main
int main(int argc, char *argv[])
{
    // Properties
    long orig_rax;

    // Flags
    int status;
    int isNotInCall = 1;
    int vOptionActive = 0; // flag for option v (V causes this to be active as well)
    int VOptionActive = 0; // flag for option V
    int programFileNameFound = 0;

    // Parsing command line parameters.
    char programName[1000] = "";
    for (int i = 1; i < argc; i++) {
        if (programFileNameFound == 0) {
            if (strcmp("-v", argv[i]) == 0) {
                vOptionActive = 1;
            } else if (strcmp("-V", argv[i]) == 0) {
                vOptionActive = 1;
                VOptionActive = 1;
            } else {
                strcat(programName, argv[i]);
                programFileNameFound = 1;
            }
        } else {
            strcat(programName, " ");
            strcat(programName, argv[i]);
        }
    }
    if (programFileNameFound == 1) {
        pid_t child = fork();

        // Main Process
        if(child == 0) {
            ptrace(PTRACE_TRACEME, child, NULL, NULL);
	          execl("/bin/sh","sh", "-c", programName,NULL); // Replace current process with new program.
        }
        else {
            while(1) {
                wait(&status);
                if(WIFEXITED(status)) break;

                orig_rax = ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL);
                if (isNotInCall) {
                    isNotInCall = 0;
                    if (vOptionActive) printf("System call made: %s\n\n", callname(orig_rax));
                    addRecord(orig_rax);

                    if (VOptionActive) {
                        getchar();
                    }
                } else {
                    isNotInCall = 1;
                }

                ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        }
            printTable(); // Always have to run at the end of the process execution to show the summay of the syscalls called.
        }

        return 0;
    } else {
        printf ("Could not found 'Program' name to run\n");
        return 0;
    }
}
