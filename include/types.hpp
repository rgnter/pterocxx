//
// Created by maros on 05/04/2022.
//

#ifndef PTEROCXX_TYPES_HPP
#define PTEROCXX_TYPES_HPP

#include <vector>
#include <cstdint>
#include <string_view>
#include <nlohmann/json.hpp>

namespace pterocxx {


    /**
     * Structure holding error information.
     */
    struct error_s {
    public:
        std::string code;
        std::string detail;
        std::string status;
    };

    typedef uint32_t user_id_t;
    typedef std::string user_external_id_t;

    /**
     * Structure holding user information.
     */
    struct user_s {
    public:
        user_id_t id;
        user_external_id_t external_id;
        std::string uuid;

        std::string username;
        std::string email;
        std::string first_name;
        std::string last_name;
        std::string language;

        bool root_admin;
        bool two_factor_auth;

        std::string created_at;
        std::string updated_at;
    public:
        void build_from_attributes(const nlohmann::json& attributes);
    };

    /**
     * Structure holding allocation information of server.
     */
    struct allocation_s {
    public:
        uint32_t id;
        std::string_view ip;
        std::string_view ip_alias;
        uint16_t port;

        std::string_view notes;
        bool is_default;
    };

    /**
     * Structure holding limits information of server.
     */
    struct limits_s {
    public:
        int32_t memory;
        int32_t swap;
        int32_t disk;
        int32_t io;
        int32_t cpu;
    };

    /**
     * Structure holding feature limits information of server.
     */
    struct feature_limits_s {
    public:
        uint32_t databases;
        uint32_t allocations;
        uint32_t backups;
    };

    /**
     * Structure holding pagination information.
     */
    struct pagination_s {
    public:
        uint32_t total;
        uint32_t count;
        uint32_t per_page;
        uint32_t current_page;
        uint32_t total_pages;

        std::vector<std::string_view> links;
    };

    struct meta_s {
    public:
        pterocxx::pagination_s pagination;
    };

    /**
     * Structure holding server information.
     */
    struct server_s {
    public:
        std::string_view server_owner;
        std::string_view identifier;
        std::string_view description;

        std::string_view uuid;
        std::string_view name;
        std::string_view node;

        pterocxx::limits_s limits;
        pterocxx::allocation_s sftp_details;
        pterocxx::feature_limits_s feature_limits;

        bool is_suspended;
        bool is_installing;

        std::vector<pterocxx::allocation_s> allocations;

        pterocxx::meta_s meta;
    };
}

#endif //PTEROCXX_TYPES_HPP
