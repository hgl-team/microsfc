/*
 * Step.cpp
 *
 *  Created on: 23/11/2019
 *      Author: leonardo
 */

#include "Step.h"

namespace sfc {

Step::Step() : StatefulObject() {
	this->entry_point = false;
}

Step::Step(bool entry_point) : StatefulObject() {
	this->entry_point = entry_point;
}

bool Step::isEntryPoint() const {
	return entry_point;
}

void Step::setEntryPoint(bool entryPoint) {
	entry_point = entryPoint;
}

Step::~Step() {
}

} /* namespace sfc */
