Aaron Himelman (himelman), Ankush Gola (agola)

In do_spawn(), we git the pid by scanning for the first open entry in pcb[], call stack_new(),
(which we modified to take in the pid and allocate a stack corresponding to that pid), call
ramdisk_find() on the file name, set the entry point of the pcb to that, and then increment 
total_ready_priority.

In do_wait(), we set a condition_wait on the condition we're waiting for, and add this
field to the pcb.

In kill, we remove the node corresponding to that pcb from whatever queue it is in by 
re-arranging the queue pointers. We then decrement total_ready_priority, condition_broadcast
on its condition, and set its status to EXITED.

Finally, the message box functions all follow the standard bounded buffer problem. We use a 
circular array for our queue of values, and have a usage count for each mesage box
to determine the number of processes using it.
