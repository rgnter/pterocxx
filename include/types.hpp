//
// Created by maros on 05/04/2022.
//

#ifndef PTEROCXX_TYPES_HPP
#define PTEROCXX_TYPES_HPP

#include <vector>
#include <nlohmann/json.hpp>

namespace pterocxx {

    /**
     * Base object.
     */
    struct base_object_s {
    public:
        virtual void build_from_attributes(const nlohmann::json &attributes) = 0;
    };

    /**
     * List object.
     */
     template<class T>
    struct list_object_s : base_object_s {
    public:
        std::vector<T> data;
    public:
        void build_from_attributes(const nlohmann::json &attributes) override {
            if(!attributes.contains("data"))
                return;
            for (const auto &entry : attributes["data"]) {
                T obj;
                obj.build_from_attributes(entry["attributes"]);
                this->data.emplace_back(obj);
            }
        }
    };


    /**
     * Error object.
     */
    struct error_s : base_object_s {
    public:
        std::string code;
        std::string detail;
        std::string status;
    public:
        /**
         * Builds error from json attributes.
         * @param attributes Attributes.
         */
        void build_from_attributes(const nlohmann::json &attributes) override;
    };


    typedef uint32_t user_id_t;
    typedef std::string user_external_id_t;

    /**
     * User object.
     */
    struct user_s : base_object_s {
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
        /**
         * Builds error from json attributes.
         * @param attributes Attributes.
         */
        void build_from_attributes(const nlohmann::json &attributes) override;
    };

    /**
     * Allocation object.
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
     * Limits object.
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
     * Feature Limits object.
     */
    struct feature_limits_s {
    public:
        uint32_t databases;
        uint32_t allocations;
        uint32_t backups;
    };

    /**
     * Server object.
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
    };
}

#endif //PTEROCXX_TYPES_HPP
