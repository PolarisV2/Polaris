/*
* Copyright (c) 2021, Polaris All rights reserved.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef TABLE_H
#define TABLE_H

#include "plate.h"

#include <list>

namespace polaris
{
    namespace tables
    {
        //!  A stacked State Machine class.
        /*!
        * Stacks Plates on top of each other, the Plate on top of the stack is considered Primary and will execute.
        */
        class Table
        {
        public:
            std::list<plates::Plate*> m_lpPlates; //!< List of Plates. Plate at position 0 in list is considered Primary.

            void Update(); //!< Update has to be called to keep the Plates functioning properly, preferrably at a (1000 / 60) interval.

            void PushPlate(plates::Plate* plate); //!< PushPlate adds a new Plate on top of the stack.
            void PopPlate(); //!< PopPlate pops the primary Plate, making the plate below primary.
            void Pop(); //!< Pop pops all the Plates and throws the Table into the garbage collector.
        };
    }
}

#endif // !TABLE_H