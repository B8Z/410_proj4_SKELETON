/**
 * Baker.cpp
 *
 * @author Adam Bates & Steven Cheng
 * @date 04/18/2020
 */
#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"

using namespace std;

Baker::Baker(int id) :
		id(id) {
}

Baker::~Baker() {
}

void Baker::bake_and_box(ORDER &anOrder) {
	//total num of boxes needed
	int totalBox = (anOrder.number_donuts / 12);
	int excess = (anOrder.number_donuts % 12);

	//fill'em up
	//works for orders 1-12
	for (int i = 0; i < totalBox; i++) {
		Box box;
		for (int j = 0; j < anOrder.number_donuts; j++) {
			DONUT donut;
			box.addDonut(donut);
		}
		anOrder.boxes.push_back(box);
	}

	//works for orders 13-15+
	if (excess > 0) {
		Box box;
		for (int i = 0; i < excess; i++) {
			DONUT donut;
			box.addDonut(donut);
		}
		anOrder.boxes.push_back(box);
	}
}

void Baker::beBaker() {
	while (true) {
		unique_lock<mutex> lck(mutex_order_inQ);

		//if there is no order and waiter working, waiting...
		while (order_in_Q.empty() && !b_WaiterIsFinished) {
			cv_order_inQ.wait(lck);
		}

		//order incoming!
		if (!order_in_Q.empty()) {
			//vector changes
			bake_and_box(order_in_Q.front());

			unique_lock<mutex> lock(mutex_order_outQ);
			order_out_Vector.push_back(order_in_Q.front());
			lock.unlock();

			order_in_Q.pop();
		}

		//if there is no orders in the queue and the waiter is finished...
		if (order_in_Q.empty() && b_WaiterIsFinished) {
			break;
		}
	}
}
