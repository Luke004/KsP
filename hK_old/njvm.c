#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "njvm.h"

int main(int argc, char* argv[])
{
  /*manage parameter input */
  {
    int i;
    i=1;
    for(; i < argc; i++) 
    {
        if (strcmp(argv[i], ARGUMENT_VERSION) == 0) 
        {
            printf("Current Version: %d\n", NJVM_VERSION);
            exit(EXIT_SUCCESS);
        }
        else if (strcmp(argv[i], ARGUMENT_HELP) == 0) 
        {
            printHelp();
            exit(EXIT_SUCCESS);
        }
        else 
        {
            printf("unknown command line argument '%s' is not available, try %s %s\n", argv[i], argv[0], ARGUMENT_VERSION);
            exit(EXIT_FAILURE);
        }
    }
  }
    
  /* Main Program */
  printf("Ninja Virtual Maschine started\n");
  printf("Ninja Virtual Maschine stopped\n");
  return 0;
}


void printHelp(void)
{
	printf("Usage: njvm [option] [option] ... \n");
	printf("Arguments: \n");
	printf("%s\tShows the current version of njvm.\n", ARGUMENT_VERSION);
	printf("%s\t\tShows this help screen.\n", ARGUMENT_HELP);
        printf("\n");
}