CSE.30341.FA17: Project 01
==========================

This is the documentation for [Project 01] of [CSE.30341.FA17].

[Project 01]:       https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/project01.html
[CSE.30341.FA17]:   https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/

Members
-------

1. Herman Tong (ktong1@nd.edu)
2. John McGuinness (jmcguin1@nd.edu)

References
----------

> The bare structure of this project was inspired by Project 1 from Systems Programming
> last spring.

Design
------

> Explain how you would periodically scan the `root` the directory and detect
> if a file has been created, modified, or removed.
>
>   1. What system calls would you need to use?
>
>       opendir
>           - to open the directory
>       readdir
>           - read files inside the directory
>       stat
>           - use to check the modified time of the file
        closedir
            - to close the directory when finished
>
>   2. What information would you need to store and what data structures would
>      you need?
>
>       We will need to track existing filer, their modified time for
>       detecting created and modified. For detecting removed, we will need to
>       keep track if we have visited all file at each scan. If not, the files
>       not visited are deleted
>
>       To do that, we will use std::unorderd_map with the key being the full
>       path to the file and the value will keep track of the modified time and
>       whether the file has been visited. This allow us
>       to have O(1) access time when checking the stored information. Then
>       after checking each file, we will scan through the entire unordered_map
>       and the entry that hasn't been visited is deleted.
>


.

> Explain how you would check if a rule matched an event and execute the
> corresponding action.
>
>   1. What system calls would you need to use?
>
>       To keep check if something has been deleted, simply check if the total number
>       of files is the same as the number from the previous scan.
>
>       To check for created, try to access the std::unordered_map with the filename
>       from the current scan. If there's no element with the filename, it means
>       the file.
>
>       To check for modified, simply look up the filename in the
>       std::unorderd_map. If the name differs from the st_mtime, then it has
>       been modified.
>       
>       To execute the corresponding action, we will use fork() to create a new
>       process and use exec() in the child process to execute the action.
>
>   2. How would you pass the environment variables to the command?
>
>       We will simply pass the environment variables as a function variable.
>       Then in the execute function that will handle forking and actual
>       executing of the action, we will use setenv() to set the environment
>       variable. Since the child process get the same environment variables, the
>       child process should be able to access them.

.

> Explain how you would handle a [SIGINT] signal and what you would need to
> cleanup?
>
>   1. What system calls would you need to use?
>
>       signal()
>           - the signal function takes 2 arguments, first is the signal number
>             and second is a function that would handle the signal
>
>   2. How would you know what resources to cleanup?
>       
>       We will need to free up any dynamically allocated structs such as the
>       structs we'll use to keep the rules.  We will also need to free any
        dynamicaly alocated strings.
Testing
-------

> Describe how you tested and verified that `rorschach` met the project
> requirements.

As we finished each part, we made sure it worked before moving on to the next part.
We first got scan working, then we got detect working, and finally we got execute to
work.  We originally tested pattern matching using our own rules file before we used
the test script.  We had an unfortunate error with using a valid method for removing
from an STL unordered_map that is not supported with the student machine compilers
but is supported by both of our newer compilers, so we had to try to fix the segfault
this caused for a while.  Finally, when we overcame this bug, we had to reconfigure
our output text to match what the test script was looking for.

Analysis
--------

> How is what `rorschach` does similar to how a [system call] operates?

> Just like system calls interrupt the normal flow of a program to trigger events,
> `rorschach` detects changes (which can be considered analagous to the system calls)
> to a filesystem. The program then acts on these changes based on its list of rules,
> executing some sort of shell command if the modification type and file pattern
> matches those specified in the rules before returning ot the normal execution of
> the file-monitoring program.  This is similar to how a system call switches the
> program from user mode to kernel mode, waits for something from the OS kernel, and
> then returns the program back to user mode.


> As described in the project requirements, `rorschach` periodically scans the
> `root` directory to detect changes in files.  This process can be described
> as a form of [busy waiting].
>
>   1. In this specific context, why could this be considered a possible
>      negative design flaw?
>       Busy Waiting is not real time so it is not detecting changes the moment
>       it happens and it may miss some changes. In addition, it is also expensive 
>       to keep checking if there's changes as opposed to event driven program that 
>       triggers a function call when there's a change
>
>   2. Consider [inotify(7)] provided by [Linux].  How would these system calls
>      help address the problem of [busy waiting]?
>       Inotify provides a more real time notification of changes made in a
>       directory. It would prevent missing some changes which is something that
>       occurs with file polling which is essentially what rorschach does
>
>   3. Why might we still consider using [busy waiting] as specified in the
>      original design rather than use something like [inotify(7)]?
>       For starter, busy waiting is probably easier to implement than something
>       like inotify. In addition, something like inotify must be implemented
>       within the file system in order for events to trigger some sort of
>       function.

[Linux]:        https://kernel.org
[busy waiting]: https://en.wikipedia.org/wiki/Busy_waiting
[system call]:  https://en.wikipedia.org/wiki/System_call
[inotify(7)]:   http://man7.org/linux/man-pages/man7/inotify.7.html

Errata
------

> Describe any known errors, bugs, or deviations from the requirements.

> We are currently not passing the memory test, but we believe this is due to the way
> that valgrind works with strings and unordered_maps on the older student machines.
> On student04, there are no memory errors reported by valgrind.

Extra Credit
------------

> Once we finished, we went on to add concurrency to our program by using a
> pipeline.  This works by creating the pipe in main, then forking to create
> a child process.  The parent process scans the files while the child executes
> necessary actions depending on events.  We pass a string through the pipe
> containing one character specifying the modification type, the filename
> changed, and a newline character to specify that we are done processing.
> The execution function parses this and then behaves exactly as it did before.

Grader
------

- Kyle Gifaldi (kgifaldi@nd.edu)





Reminders
- cite project01 from system for macros
