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

#include "table.h"

namespace polaris
{
    namespace tables
    {
        void Table::Update()
        {
            if (m_lpPlates.size() > 0)
                m_lpPlates.front()->Update();
        }

        void Table::PushPlate(plates::Plate* plate)
        {
            // NOTE: If there's no plates yet, not having this check will cause a crash.
            if (m_lpPlates.size() > 0)
                m_lpPlates.front()->OnDisabled();

            m_lpPlates.push_front(plate);
            plate->OnEnabled();
        }
        void Table::PopPlate()
        {
            if (m_lpPlates.size() > 0)
            {
                m_lpPlates.front()->OnDisabled();
                m_lpPlates.pop_front();

                if (m_lpPlates.size() > 0)
                    m_lpPlates.front()->OnEnabled();
            }
        }
        void Table::Pop()
        {
            m_lpPlates.clear();
            delete this;
        }
    }
}