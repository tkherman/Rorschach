CSE.30341.FA17: Project 01
==========================

This is the documentation for [Project 01] of [CSE.30341.FA17].

[Project 01]:       https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/project01.html
[CSE.30341.FA17]:   https://www3.nd.edu/~pbui/teaching/cse.30341.fa17/

Members
-------

1. Domer McDomerson (dmcdomer@nd.edu)
2. Belle Fleur (bfleur@nd.edu)

Design
------

> Explain how you would periodically scan the `root` the directory and detect
> if a file has been created, modified, or removed.
>
>   1. What system calls would you need to use?
>
>   2. What information would you need to store and what data structures would
>      you need?

.

> Explain how you would execute a task for each event while limiting to total
> number of concurrent processes to the specified limit?
>
>   1. What system calls would you need to use?
>
>   2. What information would you need to store and what data structures would
>      you need?

Testing
-------

> Describe how you tested and verified that `rorschach` met the project
> requirements.

Analysis
--------

> How is what `rorschach` does similar to how a [system call] operates?

.

> What happens when the number actions require is much larger than the process
> limit?  That is, how does your program behave when many tasks need to be
> executed?  How does the program scale?

.

> As described in the project requirements, `rorschach` periodically scans the
> `root` directory to detect changes in files.  This process can be described
> as a form of [busy waiting].
>
>   1. In this specific context, why could this be considered a possible
>      negative design flaw?
>
>   2. Consider [inotify(7)] provided by [Linux].  How would these system calls
>      help address the problem of [busy waiting]?
>
>   3. Why might we still consider using [busy waiting] as specified in the
>      original design rather than use something like [inotify(7)]?

[Linux]:        https://kernel.org
[busy waiting]: https://en.wikipedia.org/wiki/Busy_waiting
[system call]:  https://en.wikipedia.org/wiki/System_call
[inotify(7)]:   http://man7.org/linux/man-pages/man7/inotify.7.html

Errata
------

> Describe any known errors, bugs, or deviations from the requirements.

Extra Credit
------------

> Describe what extra credit (if any) that you implemented.
