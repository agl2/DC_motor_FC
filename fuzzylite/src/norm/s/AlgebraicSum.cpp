/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * File:   AlgebraicSum.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:15 AM
 */

#include "../../../fl/norm/s/AlgebraicSum.h"

namespace fl {

    std::string AlgebraicSum::className() const {
        return "AlgebraicSum";
    }

    scalar AlgebraicSum::compute(scalar a, scalar b) const {
        return a + b - (a * b);
    }

    SNorm* AlgebraicSum::constructor() {
        return new AlgebraicSum;
    }
}
