/***************************************************************************
 *   Copyright (C) 2006 by Michael Ambrus                                  *
 *   michael.ambrus@maquet.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/*
set the following on command line to compile for Linux. I.e.
gcc -lrt
*/

/*
http://mia.ece.uic.edu/~papers/WWW/books/posix4/DOCU_011.HTM
http://physics.usask.ca/~angie/ep414/labmanual/pdf/posix_users.pdf
*/

#include <stdio.h> 
#include <stdlib.h>
#include <assert.h>  
#include <pthread.h> 
#include <mqueue.h>
#include <errno.h>
#include <string.h>  
#include <time.h>  
#include <fcntl.h>  

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#if !defined(TINKER) && defined(_WIN32) &&  defined(_MSC_VER)
//If this example is compiled under pThread-W32 *Widout TinKer* - 
//i.e. it needs some "help"
#  include <windows.h>
#  define sleep(x) (Sleep(x * 1000))
#  define usleep( x ) (Sleep( (unsigned long)x / 1000ul ) )

#endif



#define MYMSGSIZE 4
#define QNAME "/zingo_forever" 

void *thread2(void *inpar ){ 
   int         loop_cntr            = 0; 
   int         loop_cntr2           = 0; 
   mqd_t       q; 
   int         rc; 
   char        msg_buf[16]; 
   
   q = mq_open( QNAME, /*O_NONBLOCK | */O_WRONLY, 0 ,NULL); 
   if (q==(mqd_t)-1){ 
      int myerrno = errno;
      
      printf("1.Errno = %d\n",myerrno);
      if (myerrno == EACCES)
         printf("EACCES\n");
         
      perror(strerror(myerrno)); 
      assert("Queue opening for writing faliure" == NULL);      
   } 
   
   while (TRUE) {               
      usleep(100000); 
      loop_cntr++; 
      if (loop_cntr>=2){ 
         loop_cntr2++; 
         loop_cntr = 0; 
         
         msg_buf[0] = loop_cntr2;
         printf("sending....\n"); 
         rc = mq_send(q, msg_buf, MYMSGSIZE, 5); 
         if (rc==(mqd_t)-1){
            int myerrno = errno;
      
            printf("2.Errno = %d\n",myerrno);
            if (myerrno == EACCES)
               printf("EACCES\n");
         
            perror(strerror(myerrno));     
            assert("Queue writing faliure" == NULL); 
         } 
         printf("sent!\n"); 
      } 
   } 
   return (void*)1; 
} 

void *thread3(void *inpar){ 
   char        msg_buf[16]; 
   mqd_t       q; 
   int         rc; 
   
   q = mq_open( QNAME, O_RDONLY, 0 ,NULL); 
   if (q==(mqd_t)-1){ 
      int myerrno = errno;
      
      printf("3.Errno = %d\n",myerrno);
      if (myerrno == EACCES)
         printf("EACCES\n");
      perror(strerror(myerrno)); 
      
      assert("Queue opening for reading faliure" == NULL); 
   } 
   
   while (TRUE) {
      printf("receiving....\n");                
      rc = mq_receive(q, msg_buf, MYMSGSIZE, NULL);       
      if (rc==(mqd_t)-1){
         int myerrno = errno;
          
         printf("4.Errno = %d\n",myerrno);
         if (myerrno == EACCES)
            printf("EACCES\n");
            
         perror(strerror(myerrno)); 

         assert("Queue reading faliure\n" == NULL); 
      } 
      printf("Received: %d of length %d\n",msg_buf[0],rc); 
   } 
   return (void*)1; 
} 

int main(char argc, char **argv)
{ 
   pthread_t T2_Thid,T3_Thid; 
   mqd_t q2;
   int loop =0;   
   struct mq_attr qattr;       

   printf("\n\n\n\n\n\n\nHello world or TinKer targets\n"); 

   printf("Unlinking old queue name (if used). \n");  
   mq_unlink(QNAME);  //Don't assert - "failiure" is normal here
   sleep(1);
   
   qattr.mq_maxmsg = 3;
   qattr.mq_msgsize = MYMSGSIZE;
   q2 = mq_open( QNAME,O_CREAT|O_RDWR,0666,&qattr);  

   if (q2==(mqd_t)-1){      
      int myerrno = errno;
      
      printf("Errno = %d\n",myerrno);
      if (myerrno == EACCES)
         printf("EACCES\n");
      perror(strerror(myerrno)); 
      assert("Queue opening for creation faliure" == NULL); 
   }
   sleep(3);
   
   printf("Queues created\n"); 
   printf("Creating thread2\n"); 
   assert (pthread_create(&T2_Thid, NULL, thread2, 0) == 0); 
   printf("thread2 started\n"); 
   
   printf("Creating thread3\n"); 
   assert (pthread_create(&T3_Thid, NULL, thread3, 0) == 0); 
   printf("thread3 started\n"); 

   printf("Root started\n"); 
   for (loop=0;loop<10;loop++) {
      printf("Root 2s bling!!!!!!!!!!!!!!!!!\n");      
      sleep(2); 
   } 

   printf("Test finished\n");
   
   printf("Closing queue handle. Both threads should still work <------------\n");  
   assert (mq_close(q2) == 0);
   sleep(10);
       
   printf("Unlinking queue name. Threads should block now <------------------\n");    
   assert(mq_unlink(QNAME) == 0);
   sleep(5);
   printf("Hmm, does Linux really following standard? <----------------------\n");  
   sleep(5);
   
   
   printf("Killing thread 3\n");  
   assert (pthread_cancel(T3_Thid) == 0);
   
   printf("Killing thread 2\n");  
   assert (pthread_cancel(T2_Thid) == 0);
   return 0;
} 

/*!
 * @defgroup CVSLOG_test_posix_c test_posix_c
 * @ingroup CVSLOG
 *  $Log: test-posix.c,v $
 *  Revision 1.8  2006-04-08 10:15:47  ambrmi09
 *  Merged with branch newThreadstarter (as of 060408)
 *
 *  Revision 1.7.2.1  2006/04/03 20:07:18  ambrmi09
 *  Minor cosmetic change
 *
 *  Revision 1.7  2006/03/19 12:44:33  ambrmi09
 *  Got rid of many compilation warnings. MSVC amd GCC actually gompiles
 *  without one single warning (yay!). Be aware that ther was a lot of
 *  comparisons between signed/unsigned in ITC. Fetts a bit shaky...
 *
 *  Revision 1.6  2006/03/17 12:19:56  ambrmi09
 *  Major uppdate (5 days hard work)
 *
 *  - Finally tied up all loose ends in the concept. Threads are now
 *  joinable
 *
 *  - Corrected one error: compacting scheduele while cancelling a
 *  threads
 *
 *  - Several new API, mainly concerned with cancelation (corrsp pThread
 *  also)
 *
 *  - Found a nasty bug while creating threads in threads for XC167. TOS is
 *  really a patchy solution ;( This one had to do with the compiler
 *  being fooled by the inline assembly and optimized something that was not
 *  optimizable (saving stack segment got wacked).
 *
 *  - Designed a concurrent qsort test-app. This is good for showing
 *  boss-worker model. Number of threads recoed on XC167 was 50 and on MSVS
 *  more than 150! Interesting to notice was that TinKer creation and
 *  cancelation for threads was much faster than Windows own (20-30 times
 *  faster).
 *
 *  - A MSVC workspace for pThreads-Win32. Good for testing apps
 *  transparency.
 *
 *  - Increased memory on XC167 (phyCore HW). now 32k for stacks and 16k for
 *  malloc. We still lack RAM that is not deployed (pHycore has
 *  128k + 256k physical RAM memory i think). Maximum for
 *  stack is 64k however (type of pointers determine this). If memory is
 *  increased further, we get a TRAP_B saying bad memory interface. Typical
 *  error for config memory issues in DaVe.
 *
 *  Revision 1.5  2006/03/05 11:11:22  ambrmi09
 *  License added (GPL).
 *
 *  Revision 1.4  2006/02/28 18:16:52  ambrmi09
 *  - Mainly a ci for the new Workspace structure
 *  - Houwever, found and corrected a bug in mqueue.c (a NULL pointer
 *    assignement)
 *
 *  Revision 1.3  2006/02/23 11:34:58  ambrmi09
 *  - Improved post mortem
 *   - Fixed bug in i2hex2.
 *   - Added uptime output
 *   - mark running among RDY threads in schedule dump
 *
 *  - \ref putchar now supports easy switching between serial0 and serial1
 *
 *  mqueue.h and mqueue.c should be untouched. But trying to identify if
 *  qsort is the reason for TinKer sometimes to hang (recent possible bug).
 *
 *  Revision 1.2  2006/02/22 13:05:45  ambrmi09
 *  Major doxygen structure modification. No chancge in actual sourcecode.
 *
 *  
 */



