//
//  hash_cache.hpp
//  hash-cache-xcode
//
//  Created by Hsu Chaohung on 2017/08/18.
//  Copyright © 2017年 hsu. All rights reserved.
//

#ifndef hash_cache_hpp
#define hash_cache_hpp

#include <cstdio>
#include <cstdlib>

#include <functional>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include <openssl/md5.h>

namespace hsu {

template <typename T>
class hash_cache {
public:
    T insert(void const* data, size_t size, std::function<T()> create_value_handler) {
        auto checksum = calculate_checksum(data, size);
        auto find_result = checksum_table_.find(checksum);
        if (find_result == checksum_table_.end()) {
            auto value = create_value_handler();
            checksum_table_.insert(std::make_pair(checksum, value));
            return value;
        } else {
            return find_result->second;
        }
    }

private:
    std::string calculate_checksum(void const* data, size_t size) {
        MD5_CTX md5_context;
        unsigned char md5_digests[MD5_DIGEST_LENGTH];
        char md5_string[MD5_DIGEST_LENGTH * 2];

        if(MD5_Init(&md5_context) != 1) std::runtime_error("md5_init");
        if(MD5_Update(&md5_context, data, size) != 1) std::runtime_error("md5_update");
        if(MD5_Final(md5_digests, &md5_context) != 1) std::runtime_error("md5_final");

        for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
            sprintf(&md5_string[i * 2], "%02x", md5_digests[i]);
        return md5_string;
    }

private:
    std::unordered_map<std::string, T> checksum_table_;
};

} // end of namespace hsu

#endif /* hash_cache_hpp */
