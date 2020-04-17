/**
 * waiter.cpp
 *
 * @author Adam Bates & Steven Cheng
 * @date 04/18/2020
 */
#include <string>
#include "stdlib.h"

#include "../includes/externs.h"
#include "../includes/waiter.h"

using namespace std;

Waiter::Waiter(int id, std::string filename) :
		id(id), myIO(filename) {
}

Waiter::~Waiter() {
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder) {
	return myIO.getNext(anOrder);
}

void Waiter::beWaiter() {
	ORDER nextOr;

	while (getNext(nextOr) == SUCCESS) {
		lock_guard<mutex> lck(mutex_order_inQ);
		order_in_Q.push(nextOr);
		cv_order_inQ.notify_all();
	}

	b_WaiterIsFinished = true;

}
