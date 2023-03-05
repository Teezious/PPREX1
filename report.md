# Subtask 1: A naive implementation
- [X]
# Subtask 2: Deadlock prevention
## Why does the deadlock occur? Answer the following questions:
- What are the necessary conditions for deadlocks (discussed in the lecture) [0.5 points]?
  - There are four conditions, which allow a deadlock to happen:
    1. **Mutual Exclusion:** limited number of threads may utilize a resource concurrently 
    2. **Hold and Wait:** A thread holding a resource may request access to other resources and wait until it gets them
    3. **No preemption:** Resources are released only voluntarily by the thread holding the resource
    4. **Circular Wait:** here is a set of {T 1, ... T n} threads, where T 1 is waiting for a resource held by T 2, T 2 is waiting for a resource held by T 3, and so forth, up to T n waiting for a resource held by T 1
- Why does the initial solution lead to a deadlock (by looking at the deadlock conditions) [0.5 points]?
    1. [X] Each fork can only be held by one philosopher at a time
    2. [X] After acquiring the left fork to his left a philosopher will wait until he is able to acquire the fork to his right.
    3. [X] The philosopher will only release the forks he held on to when he finished eating
    4. [X] It is possible for philosopher p1 to wait for the fork f1, while philosopher p0 holds on to fork f1 and waits for the fork f0, which is held on to by philosopher p3 ... 
## Prevent the deadlock by removing Circular Wait condition:
- Switch the order in which philosophers take the fork by using the following scheme: Odd philosophers start with the left fork, while even philosophers start with the right hand [6 points]. Make sure to use concurrency primitives correctly!
  - [X]
- Does this strategy resolve the deadlock and why [1 point]?
  - Yes because the asymmetry removes the circular wait condition. 
- Measure the total time spent in waiting for forks and compare it to the total runtime. Interpret the measurement - Was the result expected? [3 points].
  - The total time spent waiting is even slightly higher than the total runtime, as the time spent waiting is counted in several threads simultaneously. On average a thread uses a little over 1/n of the total time waiting. This is hardly surprising as the application doesn't do a lot of heavy calculations and thus most of the runtime consists of waiting for the resources(forks) to be available. The extra time can be explained by different philosophers waiting for a resource at the same time. 
- Can you think of other techniques for deadlock prevention?
  - For example one could define the minimum amount of available forks to be at least one. A fork could then only be acquired if grabbing it would not reduce the total amount of available forks to zero. 
- Make sure to always shut down the program cooperatively and to always clean up all allocated resources [2 points]
  - [X]
