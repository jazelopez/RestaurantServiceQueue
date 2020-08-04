
#ifndef _SERVICE_QUEUE_H
#define _SERVICE_QUEUE_H

#include <iostream>
#include <vector>
#include <utility>



class ServiceQueue {

  private:
    //This list will be used to hold the place of the buzzers

    //attributes of a doubly linked list
    struct queue{
        int buzzer;      
        char inQueue;   //extra bookkeeping. States weather in queue or not 
        queue *next;
        queue *prev;
        
        queue(int _buzzer = 0, char _inQueue = 'y' , queue* _next = nullptr, queue* _prev = nullptr){
            buzzer= _buzzer;
            inQueue= _inQueue;
            next= _next;
            prev= _prev;
        }
    };
    
    //this list will hold the place of a buzzer in 
    //the doubly linked list
    
    //attributes of a single linked list
    struct bucket{
        int buzzer;
        bucket *next;
        
        bucket(int _buzzer=0, bucket* _next=nullptr){
            buzzer=_buzzer; next=_next;
        }
    };
    
    queue *queueHd;     //doubly linked list head pointer
    queue *queueBck;    //doubly linked list back pointer
    bucket *bucketHd;   //single linked list head pointer
    
    int queueSize;  //used to keep track size of the queue 
    int currSmall;  //stores the number of smallest used buzzer
    
    std::vector<queue*> buzzerAdd;

  public:

	
	 //Constructor
	 //intializes an empty service queue.
         ServiceQueue() {
             queueHd = nullptr;
             queueBck = nullptr;
             bucketHd = nullptr;
             queueSize = 0;
             currSmall = 0;
             //buzzerAdd.push_back(new queue());
         }

	
	//Destructor
	~ServiceQueue() {

        }


	//populates buzzers vector with a "snapshot"
	 
	void snapshot(std::vector<int> &buzzers) {
             buzzers.clear();   // you don't know the history of the 
                                //   buzzers vector, so we had better
                                //   clear it first.
        queue *tmp;
        tmp = queueHd;
        if(queueHd != nullptr){
            while(tmp->next != nullptr){
                buzzers.push_back(tmp->buzzer);
                tmp = tmp->next;
            }
        }
    }

	//returns number of buzzers in the queue
	int  length() {
        return queueSize;
    }

	
	//The function selects an available buzzer 
	//and places a new entry at the end of the service queue 
	//with the selected buzer-ID. 
	int  give_buzzer() {
        queue * tmpBack;
        bucket * tmpBucket;
        
        //if bucket is not empty
        if(bucketHd != nullptr){
            tmpBack = buzzerAdd[bucketHd->buzzer];      //getting the address from vector
            tmpBack->next = nullptr;                    //setting next to null
            tmpBack->prev = queueBck;                   //setting prev to current back node
            tmpBack->inQueue = 'y';                     //marking that the node is in queue
            queueBck->next = tmpBack;                   //setting last node next to new node
            queueBck = tmpBack;                         //setting back to new node
            tmpBucket = bucketHd;                       //tmp pointer to front of bucket
            
            if(bucketHd->next == nullptr)                //if last node in bucket
            { bucketHd = nullptr;}                       //set buckethd to null
            else
            { bucketHd = bucketHd->next;}                //else move to next node
            
            delete tmpBucket;                            //deleting first node seated person
        }
        
        //if bucket is empty
        else{
            //if queue is empty
            if(queueHd == nullptr){
                tmpBack = new queue(currSmall, 'y',  nullptr, nullptr);     //creeating a new node
                buzzerAdd.push_back(tmpBack);                               //pushing buzzer to end of vector
                queueHd = tmpBack;                                          //updating head of queue
                queueBck = tmpBack;                                         //updating back of queue
                currSmall++;                                                //updating current smallest
            }
            //if the queue is not empty
            else{
                tmpBack = new queue(currSmall, 'y' , nullptr, queueBck);    //creating new node
                buzzerAdd.push_back(tmpBack);                               //pushing address to end of vector
                queueBck->next = tmpBack;                                   //updating current back's next
                queueBck = tmpBack;                                         //updating back of list
                currSmall++;                                                //updating current smalles
            }
        }
 
        queueSize++;              //updating queue size
        return queueBck->buzzer;  // returning buzzer id that was added to end of queue
    }

	//this functions seats the first person in the queue 
    //while updating the buzzer bucket and bookkeeping
	int seat() {
        bucket *tmp;
        queue *queueTmp;
        
        //no buzzer
        if(queueSize <= 0 )
        { return -1; }
        
        //one buzzer in queue
        else if(queueSize ==1){
            //if the bucket is empty
            if(bucketHd == nullptr){
                tmp = new bucket(queueHd->buzzer, nullptr); //creating new node for bucket
                bucketHd = tmp;                             //updating bucket head
            }
            //bucket not empty
            else{
                tmp = new bucket(queueHd->buzzer, bucketHd); //creating new node for bucket
                bucketHd = tmp;                              //updating buckt head
            }
            
            queueHd->inQueue = 'n';                         //updating that node is no longer in queue
            queueHd = nullptr;                              //setiting head to null
            queueBck = nullptr;                             //setting back to null
        }
        
        //2 or more buzzers
        //creating new node and updating bucket head
        else{
            //if bucket is empty
            if(bucketHd == nullptr){
                tmp = new bucket(queueHd->buzzer, nullptr);
                bucketHd = tmp;
            }
            //if bucket it not empty
            else{
                tmp = new bucket(queueHd->buzzer, bucketHd);
                bucketHd = tmp;
            }
            
            queueHd->inQueue = 'n';             //updating that node is no longer in queue
            queueTmp = queueHd;                 //temp pointer to front of list
            queueHd = queueHd->next;            //updating head of queue
            queueTmp->next = nullptr;           //upating next to null
            queueHd->prev = nullptr;            //updating prev to null
        }
        
    
        queueSize --;               //updating queue size
        return bucketHd->buzzer;    //returning buzzer id
        }


	//Kick out someone from the queue and place at the end of the line 
	bool kick_out(int buzzer) {
        //pointers that will be used
        queue * tmpQueue;
        queue *tmpPrev;
        queue *tmpNext;
        bucket *tmpBucket;
        bucket *bucketNxtPtr;
        
        //checking that buzzer id exist if not return false
        if(buzzer < currSmall)
        { tmpQueue = buzzerAdd[buzzer]; }
        else
        { return false; }
        
        //checking if node is in queue if not return false
        if(tmpQueue->inQueue == 'n'){
            return false;
        }
        
        //chekcing if bucket is empty or not
        //determining what bucket next is going to be
        if(bucketHd == nullptr){
            bucketNxtPtr = nullptr;
        }
        else{
            bucketNxtPtr = bucketHd;
        }
        
        //creating new node for bucket
        tmpBucket = new bucket(queueHd->buzzer, bucketNxtPtr);
        bucketHd = tmpBucket;
        
        //if buzzer beign kicked out is first in line
        if(tmpQueue == queueHd ){
            //updating queue list
            queueHd = queueHd->next;
            queueHd->prev = nullptr;
            tmpQueue->next = nullptr;
        }
        //if buzzer being kicked out is last in line
        else if(tmpQueue == queueBck){
            //updating queue list
            queueBck = queueBck->prev;
            queueBck->next = nullptr;
            tmpQueue->prev = nullptr;
        }
        //else buzzer in middle of list
        else{
            //updating queue list
            tmpPrev = tmpQueue->prev;
            tmpNext = tmpQueue->next;
            tmpPrev->next = tmpNext;
            tmpNext->prev = tmpPrev;
            
            //updating kicked out node
            tmpQueue->next = nullptr;
            tmpQueue->prev = nullptr;
        }
        
        queueSize --; //updating queue size
        return true;  // placeholder

        }

	//take bribe from a any buzzer in line and move them to
    //the front of the queue
	bool take_bribe(int buzzer) {
        queue *tmp;
        queue *tmpPrev;
        queue *tmpNext;
        
        //checking if buzzer id exist
        if(buzzer < currSmall)
        { tmp = buzzerAdd[buzzer]; }
        else
        { return false; }
        
        //checking if the node is in the queue
        if(tmp->inQueue == 'n'){
            return false;
        }
        
        //if briber is in front of line
        if(tmp == queueHd){
            return true;
        }
        //if briber is in back of line
        else if(tmp == queueBck){
            tmpPrev = tmp->prev;        //temp pointer to previous node
            tmpPrev->next = nullptr;    //setting tempPrev to nullptr
            queueBck = tmpPrev;         //updating back
            tmp->prev = nullptr;        //setting tmep prev to null pointer
            queueHd->prev= tmp;         //setting current head's prev to new front
            queueHd = tmp;              //updating head
        }
        //anywhere else in line
        else{
            tmpPrev = tmp->prev;            //tmp pointer to previous
            tmpNext = tmp->next;            //tmp pointer for next node
            tmpPrev->next = tmpNext;        //pointing pervious to next next node
            tmpNext->prev = tmpPrev;        //pointing next's prev to tmpPrev
            
            tmp->next = queueHd;            //setting the node moved to the front
            tmp->prev = nullptr;            //setting tmp's prev to null
            queueHd->prev = tmp;            //setting currents front to new front
            queueHd = tmp;                  //queue's head is now poiting to new front
        }
    
           return true;  // placeholder

        }



};   // end ServiceQueue class

#endif

