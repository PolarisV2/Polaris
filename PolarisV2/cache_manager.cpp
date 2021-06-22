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

#include "cache_manager.h"

namespace polaris::cache
{
    auto CacheManager::GetFromCache(ECache cache, std::string query)
    {
        switch (cache)
        {
        default:
            return nullptr;

        case ECache::Cache_BlueprintGeneratedClass:
            for (auto& entry : v_pBlueprintGeneratedClassCache)
            {
                if (entry != nullptr && entry->GetFullName() == query)
                    return entry;
            }
            return nullptr;
            break;
        }
    }
    void CacheManager::AddToCache(ECache cache, auto newEntry)
    {
        switch (cache)
        {
        case ECache::Cache_BlueprintGeneratedClass:
            v_pBlueprintGeneratedClassCache.push_back(newEntry);
            break;
        }
    }
}