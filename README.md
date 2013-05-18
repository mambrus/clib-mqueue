Chapter 0.0: LIBMQUEUE
======================

POSIX 1003.1b rt-mqueue for systems that don't have it but implement
1003.1b semaphores.

Chapter 1.0: PURPOSE & DEFINITION
=================================

Note that all files at this level should be considered as test for the real
project, which is in the subdirectory libmqueue. I.e. when deployed, either
link files from there, link the directory, or build the directory and
install as library.

This project implement POSIX 1003.1B rt-queues for systems that don't have
it's ultra light as it was originally intended for deeply embedded systems.
It uses no other API's than whats available in all libc's (even the
minimalistic ones) and semaphores. I.e. it can be used almost anywhere as
either a drop-in replacement or as en extension.

Note that files at this level should be considered as test for the real

Chapter 1.0: PURPOSE & DEFINITION
=================================

Note that all files at this level should be considered as test for the real
project, which is in the subdirectory libmqueue. I.e. when deployed, either
link files from there, link the directory, or build the directory and
install as library.

This project implement POSIX 1003.1B rt-queues for systems that don't have
it's ultra light as it was originally intended for deeply embedded systems.
It uses no other API's than whats available in all libc's (even the
minimalistic ones) and semaphores. I.e. it can be used almost anywhere as
either a drop-in replacement or as en extension.

Note that files at this level should be considered as test for the real

Chapter 1.1: /*
===============


Early age
---------
Work-effort for this code was originally donated courtesy the ZOI project
(year 2000, SIEMENS Elema AB, SWEDEN). It was written by Michael Ambrus,
employee at the time, as a wrapper for pThreads-win32 to give Windows
pthreads POSIX RT queues capabilities.  Code is  however completely based on
standard API:s and as such portable to most systems that implement those
API:s.  I.e.  functions that all libc's implement, even deeply embedded
ones. The only requirement is that the system has support for POSIX 1003.1B
semaphores.

Life as a kernel
----------------
Early 2006, the library was ported to the RT kernel TinKer, which lacked
POSIX 1003.1B semaphores at the time.  I.e. the "port" basically involved
implementing those semaphores which in turn mapped almost 1:1 to existing
semaphores who implemented the same API as (the at the time) very popular
pSos. There are only so many ways one can implement semaphores, and the
synch mechanism could had been different. Semaphores were ideal to us as
they have the great advantage of:

1) Allowing more than one thread in a critical section, which is usually not
   what you want to use semaphores for anyway.

2) I.e. the number of *positive* tokens that semaphores carry as carry-over
   are ideal for representing the number of messages in queue.

Very briefly as part of project HEMUL
-------------------------------------
For a very brief time the library found it's way into the code of HEMUL,
Where it was noticed to be needed because Android lacked support for
RT-queues (first log entries in this git are from that project). It turns
out that Chrbuntu lacks the same, which the author happened to want to use
(Chrubuntu = Linux/Ubuntu on a Google Chrome book).

One almost suspects that there is a common denominator (Google), but if so
that would be the kernel which doesn't make sense. There is no real need to
involve a kernel for queue implementation. Not even when communicating
between processes, which this library at the moment of writing does not
support.  This libmqueue is all user-land based using shared memory to
transfer data.  Synchronisation is kernel, but there is no need for the data
to go down through the kernel and up. For IPC, this can much more
efficiently be done using shmem, and for cases local threads are used it's
even more efficient to stay on the same heap.

License & future
----------------
During all phases, there has been only one Copyright holder, the author.
This explains why the code has been able to move around in parts without
breaking license. Even though licence trough all stages has been GPL, it's
always the copyright holder's right to change the license (if several legal
entities hold Copyright, the only difference is that all have to agree),
which these transfers can be considered to be. I.e. a rewrite, and a move at
the same time.
