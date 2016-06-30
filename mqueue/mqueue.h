/*
 * mqueue.h
 *
 *  Created on: Jun 26, 2016
 *      Author: alexander
 */

#ifndef MQUEUE_H_
#define MQUEUE_H_

struct Message {
  int what;
  int arg1;
  int arg2;
  void* obj;//Old c-style declaration
};

typedef void (*clearFunction)(void*);
struct MQImpl;
enum {INITIAL_CAPACITY=16};

class MessageQueue
{
	MQImpl *impl;
	MessageQueue &operator=(const MessageQueue &);//Old-style restriction
	MessageQueue(const MessageQueue&); //Old-style restriction
public:
	MessageQueue(clearFunction clr=0,unsigned int initial_capacity=INITIAL_CAPACITY);
	~MessageQueue();
	void SendMessage(const Message &);
	void ReceiveMessage(Message &);//Blocking function
	bool TryReceiveMessage(Message &);//Non-blocking function
	void clearSpecificMessages(int arg);
};

#endif /* MQUEUE_H_ */
