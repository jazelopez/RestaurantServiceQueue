
#ifndef _SERVICE_QUEUE_H
#define _SERVICE_QUEUE_H

#include <iostream>
#include <vector>
#include <utility>


/**
* Implementation of the ServiceQueue ADT meeting
*  behavioral requirements, but not all runtime
*  requirements.
*/


class ServiceQueue {

  private:
    std::vector<int> the_queue;  // idx-0 is front of queue

    std::vector<int> buzzer_bucket;  // keeps track of buzzer-IDs
                                     //   that can be reused.  Operates
                                     //   as a stack so buzzer that became
                                     //   reusable most recently is on
                                     //   top (i.e., at back of vector).
 
  
   
  public:

	/**
	 * Constructor
	 * Description: intializes an empty service queue.
	 * 
	 * RUNTIME REQUIREMENT: O(1)
	 *
         * TODO
	 */
         ServiceQueue() {

             // Nothing for us to do here since vector constructors
             //   will be automatically applied to data members.
         }

	/**
	 * Destructor
	 * Description:  deallocates all memory assciated
	 *   with service queue 
	 *
	 * RUNTIME REQUIREMENT:  O(N_b) where N_b is the number of buzzer 
	 *	IDs that have been used during the lifetime of the
	 *	service queue; in general, at any particular instant
	 *	the actual queue length may be less than N_b.
	 *
	 *	[See discussion of "re-using buzzers" below]
	 *
         * TODO
	 */
	~ServiceQueue() {

            // again, nothing for us to do here. 
            // vector destructors automatically applied to 
            //    data members
        }

	/**
	 * Function: snapshot()
         * param:  buzzers is an integer vector passed by ref
	 * Description:  populates buzzers vector with a "snapshot"
         *               of the queue as a sequence of buzzer IDs 
         *
	 *
	 * RUNTIME REQUIREMENT:  O(N)  (where N is the current queue
	 *		length).
	 */
	void snapshot(std::vector<int> &buzzers) {
             buzzers.clear();   // you don't know the history of the 
                                //   buzzers vector, so we had better
                                //   clear it first.

             // Note: the vector class over-rides the assignment operator '='
             //   and does an element-by-element copy from the RHS vector (the_vector)
             //   in this case) to the LHS vector (buzzers) in this case).

             buzzers = the_queue;

        }

	/**
	 * Function: length()
	 * Description:  returns the current number of
	 *    entries in the queue.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	int  length() {

             return the_queue.size();   // placeholder

        }

	/**
	 * Function: give_buzzer()
         * Return:   buzzer-ID (integer) assigned to the new customer.
	 * Description:  This is the "enqueue" operation.  For us
	 *    a "buzzer" is represented by an integer (starting
	 *    from zero).  The function selects an available buzzer 
	 *    and places a new entry at the end of the service queue 
	 *    with the selected buzer-ID. 
	 *    This buzzer ID is returned.
	 *    The assigned buzzer-ID is a non-negative integer 
	 *    with the following properties:
	 *
	 *       (1) the buzzer (really it's ID) is not currently 
	 *         taken -- i.e., not in the queue.  (It
	 *         may have been in the queue at some previous
	 *         time -- i.e., buzzer can be re-used).
	 *	  This makes sense:  you can't give the same
	 *	  buzzer to two people!
	 *
	 *       (2) Reusable Buzzers:  A re-usable buzzer is 
	 *	  a buzzer that _was_ in the queue at some previous
	 *	  time, but currently is not.
	 *
         *         REQUIREMENT:  If there is one or more reusable
         *         buzzer, you MUST return one of them; furthermore,
         *         it must be the buzzer that became reusable most 
         *         MOST RECENTLY.
	 *
	 *       (3) if there are no previously-used / reusable buzzers, 
         *         the SMALLEST possible buzzer-ID is used (retrieved from 
         *         inventory).  
	 *	    Properties in this situation (where N is the current
	 *	      queue length):
	 *
	 *		- The largest buzzer-ID used so far is N-1
	 *
	 *		- All buzzer-IDs in {0..N-1} are in the queue
	 *			(in some order).
	 *
	 *		- The next buzzer-ID (from the basement) is N.
	 *
	 *    In other words, you can always get more buzzers (from
	 *    the basement or something), but you don't fetch an
	 *    additional buzzer unless you have to (i.e., no reusable buzzers).
	 *
	 * Comments/Reminders:
	 *
	 *	Rule (3) implies that when we start from an empty queue,
	 *	the first buzzer-ID will be 0 (zero).
	 *
	 *	Rule (2) does NOT require that the _minimum_ reuseable 
	 *	buzzer-ID be used.  If there are multiple reuseable buzzers, 
	 *	any one of them will do.
	 *	
	 *	Note the following property:  if there are no re-useable 
	 *	buzzers, the queue contains all buzzers in {0..N-1} where
	 *       N is the current queue length (of course, the buzzer IDs 
	 *	may be in any order.)
	 *
	 * RUNTIME REQUIREMENT:  O(1)  ON AVERAGE or "AMORTIZED"  
	 *          In other words, if there have been M calls to 
	 *		give_buzzer, the total time taken for those 
	 *		M calls is O(M).
	 *
	 *		An individual call may therefore not be O(1) so long
	 *		as when taken as a whole they average constant time.
	 *
	 */
	int  give_buzzer() {
          int b;
   
             // take top reusable buzzer if possible
             if(buzzer_bucket.size() != 0) {
                 b = buzzer_bucket.back();
                 buzzer_bucket.pop_back();
             }
             // otherwise, queue must contain exactly buzzers
             //    0..the_queue.size()-1
             // and therefore, the next smallest buzzer must be
             //    the_queue.size()
             else {
                 b = the_queue.size();
             }
             the_queue.push_back(b);
             return b;  
        }


     private:
        // "slides" elements from index idx+1 to end of vector
        //    one position to the left (over-writing element at
        //    position idx).
        // final pop_back is to remove duplicate element (or elem
        //    at position idx in the event that idx==vec.size()-1)
        static void slide_left(std::vector<int> &vec, int idx){
            int n=vec.size();
            for(int i=idx; i<n-1; i++) 
                vec[i]=vec[i+1];
            vec.pop_back();
        }

        // returns index of first vector entry containing x
        // if x does not appear, -1 is returned
        static int find(std::vector<int> &vec, int x){
         
           for(int i=0; i<vec.size(); i++) {
               if(vec[i]==x)
                   return i;
           }
           return -1;
        }

        static void push_front(std::vector<int> &vec, int val){
          
           if(vec.size()==0) 
               vec.push_back(val);
           else {
               vec.push_back(vec.back());  // make new entry at end
               // now slide everything one slot to right
               for(int i=vec.size()-1; i>0; i--) {
                   vec[i]=vec[i-1];
               }
               // now we can overwrite slot zero with value to push
               vec[0] = val;
            }
        }

     public:

	/**
	 * function: seat()
	 * description:  if the queue is non-empty, it removes the first 
	 *	 entry from (front of queue) and returns the 
	 *	 buzzer ID.
	 *	 Note that the returned buzzer can now be re-used.
	 *
	 *	 If the queue is empty (nobody to seat), -1 is returned to
	 *	 indicate this fact.
         *
         * Returns:  buzzer ID of dequeued customer, or -1 if queue is empty.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	int seat() {
           int buzzer;

           if(the_queue.size()==0) 
              return -1;
           else {
              buzzer = the_queue[0];
           
              slide_left(the_queue, 0);
              buzzer_bucket.push_back(buzzer);
              return buzzer;
           }
        }
     


	/**
	 * function: kick_out()
	 *
	 * description:  Some times buzzer holders cause trouble and
	 *		a bouncer needs to take back their buzzer and
	 *		tell them to get lost.
	 *
	 *		Specifially:
	 *
	 *		If the buzzer given by the 2nd parameter is 
	 *		in the queue, the buzzer is removed (and the
	 *		buzzer can now be re-used) and 1 (one) is
	 *		returned (indicating success).
	 *
	 * Return:  If the buzzer isn't actually currently in the
	 *		queue, the queue is unchanged and false is returned
	 *		(indicating failure).  Otherwise, true is returned.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	bool kick_out(int buzzer) {
           int idx;

             idx = find(the_queue, buzzer);
             if(idx == -1)
                 return false;
             else {
                 slide_left(the_queue,idx);
                 buzzer_bucket.push_back(buzzer);
                 return true;
             }
        }

	/**
	 * function:  take_bribe()
	 * description:  some people just don't think the rules of everyday
	 *		life apply to them!  They always want to be at
	 *		the front of the line and don't mind bribing
	 *		a bouncer to get there.
	 *
	 *	        In terms of the function:
	 *
	 *		  - if the given buzzer is in the queue, it is 
	 *		    moved from its current position to the front
	 *		    of the queue.  1 is returned indicating success
	 *		    of the operation.
	 *		  - if the buzzer is not in the queue, the queue 
	 *		    is unchanged and 0 is returned (operation failed).
	 *
	 * Return:  If the buzzer isn't actually currently in the
	 *		queue, the queue is unchanged and false is returned
	 *		(indicating failure).  Otherwise, true is returned.
	 *
	 * RUNTIME REQUIREMENT:  O(1)
	 */
	bool take_bribe(int buzzer) {
          int idx;

             idx = find(the_queue, buzzer);
             if(idx == -1)
                 return false;
             else {
                 slide_left(the_queue,idx);
                 push_front(the_queue, buzzer);
                 return true;
             }
         }

};   // end ServiceQueue class

#endif

