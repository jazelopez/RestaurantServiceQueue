
#include <stdio.h>
#include <stdlib.h>

/* FILE:  Driver.cpp
 * desc:  simple menu-based interactive program which lets user exercise
 *         the ServiceQueue class functions, inspect the queue, etc.
 */
 

// The #ifdef directives below enables compilation using different
//   implementations of the ServiceQueue class.
//
// Default compilation uses the ServiceQueue.h implementation:
//
//     g++ -std=c++11 Driver.cpp -o Driver
//
// Compilation which sets the _SLOW preprocessor variable uses the 
//           ServiceQueueSlow.h implementation:
//
//     g++ -std=c++11 -D_SLOW Driver.cpp -o DriverSlow
//
// Compilation which sets the _SLOW2 preprocessor variable uses the 
//           ServiceQueueSlow2.h implementation:
//
//     g++ -std=c++11 -D_SLOW2 Driver.cpp -o DriverSlow2

#ifdef _SLOW
#include "ServiceQueueSlow.h"
std::string SourceFile = "ServiceQueueSlow.h";
#define __IMPL_INCL
#endif

#ifdef _SLOW2
#ifndef __IMPL_INCL
#include "ServiceQueueSlow2.h"
std::string SourceFile = "ServiceQueueSlow2.h";
#define __IMPL_INCL
#endif
#endif

#ifndef __IMPL_INCL
#include "ServiceQueue.h"
std::string SourceFile = "ServiceQueue.h";
#define __IMPL_INCL
#endif


void display(ServiceQueue &q) {
std::vector<int> snap;

  q.snapshot(snap);

  std::cout << "  [ ";
  for(int b : snap) {
      std::cout << b << " ";
  }
  std::cout << "] " << std::endl;
}
  

/**
* takes a full line of user input and 
*   parses it.
*
* Executes specified command and prints appropriate
*   message if syntactially correct.
*   otherwise prints an error message.
*
* Returns 1 only to indicate the the quite command 'q' was 
*   entered.
* Otherwise, 0 is returned.
*/
int execute_cmd(ServiceQueue &q, char *buf) {
int tok;
char cmd;
int buzzer;
char junk[128];

  // tok stores number of tokens parsed
  // Note that all commands have either one or two 'tokens':
  //   example:  b 8
  //     'b' is the first token and '8' is the 2nd
  // junk array is used to detect if an extraneous 3rd argument
  //     has been entered (parse error).
  tok = sscanf(buf, " %c %i %s", &cmd, &buzzer, junk);

  if(tok==0) return 0;
  if(tok > 2){
	printf("    bad command. try again\n");
	return 0;
  }

  switch (cmd) {
	case 'q':
		if(tok != 1){
		  printf("    bad command. try again\n");
		  return 0;
		}
		else{
		  printf("  goodbye...\n");
		  return 1;
		}
	case 'd' : 
		if(tok != 1)
		  printf("    bad command. try again\n");
		else
		  display(q);
		return 0;
	case 'l':
		if(tok != 1)
		  printf("    bad command. try again\n");
		else
		  printf("  len: %i\n", q.length());
		return 0;
	case 'g':
		if(tok != 1)
		  printf("    bad command. try again\n");
		else
		  printf("  buzzer: %i\n", q.give_buzzer());
		return 0;
	case 's':
		if(tok != 1)
		  printf("    bad command. try again\n");
		else{
                  buzzer = q.seat();
                  if(buzzer != -1) 
		    printf("  seating buzzer: %i\n", buzzer);
                  else
		    printf("  sorry, queue is empty\n");
                }
		return 0;
	case 'k':
		if(tok != 2)
		  printf("    bad command. try again\n");
		else {
		  if(q.kick_out(buzzer)) 
			printf("  %i is outta here!\n", buzzer);
		  else 
			printf("  could not remove tkt %i!\n", buzzer);
		}
		return 0;
	case 'b':
		if(tok != 2)
		  printf("    bad command. try again\n");
		else {
		  if(q.take_bribe( buzzer)) 
			printf("  VIP coming through!\n");
		  else 
			printf("  Get in line, then bribe me!\n");

		}
		return 0;
	default:
	    printf("    bad command. try again\n");
	    return 0;
  }
}
	




int main() {
ServiceQueue q;
char buf[128];
int done = 0;


  printf("\nWelcome to the simple service-queue interactive program\n");
  printf("\n      (SOURCE FILE USED FOR ServiceQueue CLASS: %s)\n\n", SourceFile.c_str());
  printf("   An empty service queue has been created for you\n");
  printf("   Commands:\n");
  printf("    d          : display queue\n");
  printf("    l          : report length of queue\n");
  printf("    g          : give out a buzzer\n");
  printf("    s          : serve the first buzzer in line\n");
  printf("    k <buzzer> : kick specified buzzer out!\n");
  printf("    b <buzzer> : take a bribe to move specified buzzer to front!\n");
  printf("    q          : quit\n");
  printf("-----------------------------------\n\n");
  

  while(!done) {
	printf("cmd > ");
	if(fgets(buf, 127, stdin) != NULL) {
	    if(execute_cmd(q, buf))
		done = 1;
	}
  }
  return 0;
}




