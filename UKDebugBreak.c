/*
 *  UKDebugBreak.c
 *  AngelDatabase
 *
 *  Created by Uli Kusterer on 18.10.08.
 *  Copyright 2008 The Void Software. All rights reserved.
 *
 */

/*
	Parts based on code from Apple Technical Q&A QA1361
*/

#include "UKDebugBreak.h"

#if DEBUG	// kinfo_proc can change between OS releases, don't put in release build!

#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sysctl.h>

// Returns true if the current process is being debugged (either
// running under the debugger or has a debugger attached post facto).
int	UKAmIBeingDebugged(void)
{
	return true;
	
    int                 junk;
    int                 mib[4];
    struct kinfo_proc   info;
    size_t              size;

    // Initialize the flags so that, if sysctl fails for some bizarre
    // reason, we get a predictable result.

    info.kp_proc.p_flag = 0;

    // Initialize mib, which tells sysctl the info we want, in this case
    // we're looking for information about a specific process ID.

    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PID;
    mib[3] = getpid();

    // Call sysctl.

    size = sizeof(info);
    junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);
    assert(junk == 0);

    // We're being debugged if the P_TRACED flag is set.

    return ( (info.kp_proc.p_flag & P_TRACED) != 0 );
}

#endif