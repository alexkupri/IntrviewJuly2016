/*
 * main.cpp
 *
 *  Created on: Jun 26, 2016
 *      Author: alexander
 */

#include <thread>
#include <iostream>
#include <cassert>

#include "mqueue.h"

enum {EX_COMM=1,HW_COMM=2};

void ThreadFunc(MessageQueue *in,MessageQueue *out)
{
	Message m;
    for (;;) {
        in->ReceiveMessage(m);
        if(m.what==EX_COMM){
        	return;
        }
        std::cout<<"world!\n";
        out->SendMessage(m);
    }
}

void TestQueue()
{
	int j,k;
	Message m1;
	MessageQueue q1(0,2);
	for(j=0;j<40;j++){
		q1.SendMessage(m1);
	}
	for(j=0;j<40;j++){
		q1.ReceiveMessage(m1);
	}
	for(j=0;j<10;j++){
		for(k=0;k<4;k++){
			m1.what=k;
			q1.SendMessage(m1);
		}
	}
	q1.clearSpecificMessages(3);
	for(j=0;j<10;j++){
		for(k=0;k<3;k++){
			q1.ReceiveMessage(m1);
			assert(m1.what==k);
		}
	}
	assert(!q1.TryReceiveMessage(m1));
	std::cout<<"Test passed\n";
}

int main()
{
	//TestQueue();
	Message m;
    MessageQueue in,out;
    std::thread t(ThreadFunc,&in,&out); // takes a copy of t !
    for(int j=0;j<1000;j++){
        std::cout<<"Hello ";
    	m.what=HW_COMM;
    	in.SendMessage(m);
    	out.ReceiveMessage(m);
    }
	m.what=EX_COMM;
	in.SendMessage(m);
    t.join(); // blocks
    std::cout<<"Success\n";
    return 0;
}


