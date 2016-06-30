/*
 * mqueue.cpp
 *
 *  Created on: Jun 26, 2016
 *      Author: alexander
 */
#include <pthread.h>
#include "mqueue.h"

void defClear(void *)
{}

struct MQImpl
{
	pthread_cond_t cv;
	pthread_mutex_t mutex;
	//
	clearFunction cf;
	//
	Message *queue;
	int capacity,start,present;
	//
	inline Message &at(int idx){return queue[(start+idx)%capacity];}
};

MessageQueue::MessageQueue(clearFunction clr,unsigned int initial_capacity)
{
	if(clr==0){
		clr=&defClear;
	}
	impl=new MQImpl;
	impl->capacity=initial_capacity;
	impl->queue=new Message[impl->capacity];
	impl->start=impl->present=0;
	impl->cf=clr;
	pthread_mutex_init(&impl->mutex,NULL);
	pthread_cond_init(&impl->cv,NULL);
}

MessageQueue::~MessageQueue()
{
	while(impl->present>0){
		impl->cf(impl->at(0).obj);
		impl->start=(impl->start+1)%impl->capacity;
		impl->present--;
	}
	pthread_mutex_destroy(&impl->mutex);
	pthread_cond_destroy(&impl->cv);
	delete impl;
}

void MessageQueue::SendMessage(const Message &msg)
{
	pthread_mutex_lock(&impl->mutex);
	if(impl->capacity==impl->present){
		int new_capacity=impl->capacity*2;
		Message *n_queue=new Message[new_capacity];
		for(int j=0;j<impl->capacity;j++){
			n_queue[j]=impl->at(j);
		}
		impl->capacity=new_capacity;
		impl->start=0;
		delete[] impl->queue;
		impl->queue=n_queue;
	}
	impl->at(impl->present)=msg;
	impl->present++;
	pthread_mutex_unlock(&impl->mutex);
	pthread_cond_signal(&impl->cv);
}

void MessageQueue::ReceiveMessage(Message &msg)
{
	pthread_mutex_lock(&impl->mutex);
	while(impl->present==0){
		pthread_cond_wait(&impl->cv,&impl->mutex);
	}
	msg=impl->at(0);
	impl->start=(impl->start+1)%impl->capacity;
	impl->present--;
	pthread_mutex_unlock(&impl->mutex);
}

bool MessageQueue::TryReceiveMessage(Message &msg)
{
	bool retval=false;
	pthread_mutex_lock(&impl->mutex);
	if(impl->present>0){
		msg=impl->at(0);
		impl->start=(impl->start+1)%impl->capacity;
		impl->present--;
		retval=true;
	}
	pthread_mutex_unlock(&impl->mutex);
	return retval;
}

void MessageQueue::clearSpecificMessages(int arg)
{
	int j,k=0;
	pthread_mutex_lock(&impl->mutex);
	for(j=0;j<impl->present;j++){
		if(impl->queue[(j+impl->start)%impl->capacity].what==arg){
			impl->cf(impl->at(j).obj);
		}else{
			impl->at(k)=impl->at(j);
			k++;
		}
	}
	impl->present=k;
	pthread_mutex_unlock(&impl->mutex);
}


