#include <stdio.h>
#define __USE_GNU
#include <signal.h>

#include <string.h>

#include <unistd.h>
#include <ucontext.h>
#include <sys/types.h>


long long * find_reasonable_return_address(long long * stack);

static void sighandle(int sig, siginfo_t *siginfo, void *context) {
    pid_t my_pid;  
    ucontext_t *uc = (ucontext_t *)context;
   
    printf("Sending PID: %ld, UID: %ld, Signal: %d\n",
            (long)siginfo->si_pid, (long)siginfo->si_uid, sig);
    
    my_pid = getpid();


    if ( (siginfo->si_signo == SIGTERM) ) {
        printf("Got Sigterm\n");
        if ( (my_pid != siginfo->si_pid)) {
            // wanna fite m8?    
            kill(siginfo->si_pid, SIGKILL);
            return;
        }
    }
    
    if ( (siginfo->si_signo == SIGILL) ) {
        printf("Got Sigill\n");
        uc->uc_mcontext.gregs[REG_RIP] += 1;
    }
    
    if ( (siginfo->si_signo == SIGABRT) ) {
        long long * stack;
        /*
            Lots of libc stuff ends up in SIGABRT if you force things forward enough
            e.g free. Quitting is for losers so we're gonna find a return address and just
            go with it.
        */
       
        // Most of the time returning 0 means stuff is ok.
        uc->uc_mcontext.gregs[REG_RAX] = 0;

        //printf("Got SigAbort\n");
        //printf("RIP at %p\n", uc->uc_mcontext.gregs[REG_RIP]);

        // Swap out rip
        stack = find_reasonable_return_address((long long *)uc->uc_mcontext.gregs[REG_RSP]);
        uc->uc_mcontext.gregs[REG_RIP] = *stack;
        

        // Hope rbp is there
        stack--;
        uc->uc_mcontext.gregs[REG_RSP] = *stack;

        //printf("Set Stack to %p\n", *stack);

        setcontext(uc);
        return;
    }

    if (siginfo->si_signo == SIGSEGV) {
        // its fine we'll just skip that one.
        long long newRIP;
        //printf("Segfault at address: %p\n", siginfo->si_addr);
        //printf("RIP at %p\n", uc->uc_mcontext.gregs[REG_RIP]);
        uc->uc_mcontext.gregs[REG_RIP] += 1;
    }

}

long long * find_reasonable_return_address(long long * stack) {
    long long myaddr;
    int j;
    myaddr = (long long)&sighandle & 0xffffffffffff0000;

    for (stack;;stack++) {
        if ( (*stack & 0xffffffffffff0000) == myaddr ) {
            for(j = 0; j < 0x10; j++) {
                if (memcmp( (void *)(*stack) + j, "\x41\x41\x41\x41", 4) == 0) {
                    // Probably the right one
                    return (long long *)stack;
                }
            }
        }
    }
}



void register_sighandlers() {
    struct sigaction act;

	memset (&act, '\0', sizeof(act));
 
	/* Use the sa_sigaction field because the handles has two additional parameters */
	act.sa_sigaction = &sighandle;
 
	/* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
	act.sa_flags = SA_SIGINFO;
 
	if (sigaction(SIGTERM, &act, NULL) < 0) {
		perror ("sigaction");
		return;
	}
	
	if (sigaction(SIGSEGV, &act, NULL) < 0) {
		perror ("sigaction");
		return;
	}
	
    if (sigaction(SIGILL, &act, NULL) < 0) {
		perror ("sigaction");
		return;
	}
    
    if (sigaction(SIGABRT, &act, NULL) < 0) {
		perror ("sigaction");
		return;
	}

    printf("Fuck it, loaded handlers\n");

}
