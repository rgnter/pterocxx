//
// Created by maros on 07/04/2022.
//

#ifndef PTEROCXX_CACHE_HPP
#define PTEROCXX_CACHE_HPP

#include "types.hpp"

#include <unordered_map>
#include <memory>

namespace pterocxx {

    /**
     * Cache
     * @tparam T Stored type
     */
    template<typename T>
    class cache {
    private:
        std::unordered_map<uint32_t, std::shared_ptr<T>> cache_map;
    public:
        /**
         * Retrieves value.
         * @param id  ID
         * @return Value ptr
         */
        inline std::shared_ptr<T> retrieve(uint32_t id) {
            if(this->cache_map.contains(id))
                return this->cache_map[id];
            return std::shared_ptr<T>();
        }
        /**
         * Stores value.
         * @param id    ID
         * @param value Value
         * @return Value ptr
         */
        inline std::shared_ptr<T> store(uint32_t id, T value) {
            this->cache_map[id] = std::make_shared<T>(value);
            return this->cache_map[id];
        }
    };

    typedef cache<user_s> user_cache_t;
    typedef cache<server_s> server_cache_t;

    static user_cache_t user_cache;
    static server_cache_t server_cache;
}

#endif //PTEROCXX_CACHE_HPP
