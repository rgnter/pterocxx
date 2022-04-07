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
        virtual void read_object(const nlohmann::json &object) = 0;
        virtual nlohmann::json write_object() const = 0;
    };

    /**
     * List object.
     */
    template<class T>
    struct list_object_s : base_object_s {
    public:
        std::vector<T> data;
    public:
        void read_object(const nlohmann::json &attributes) override {
            if(!attributes.contains("data"))
                return;
            for (const auto &entry : attributes["data"]) {
                T obj;
                obj.read_object(entry["attributes"]);
                this->data.emplace_back(obj);
            }
        }

        nlohmann::json write_object() const override {
            return nlohmann::json();
        };
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
        void read_object(const nlohmann::json &attributes) override;
        nlohmann::json write_object() const override;
    };

    /**
     * Limits object.
     */
    struct limits_s : base_object_s {
    public:
        int32_t memory;
        int32_t swap;
        int32_t disk;
        int32_t io;
        int32_t cpu;

        bool oom_disabled;
    public:
        /**
         * Builds limits from json attributes.
         * @param attributes Attributes.
         */
        void read_object(const nlohmann::json &attributes) override;

        /**
         * Builds limits attributes
         * @return JSON attributes.
         */
        nlohmann::json write_object() const override;
    };

    /**
     * Feature limits object.
     */
    struct feature_limits_s : base_object_s {
    public:
        uint32_t databases;
        uint32_t allocations;
        uint32_t backups;
    public:
        /**
         * Builds feature limits from json attributes.
         * @param attributes Attributes.
         */
        void read_object(const nlohmann::json &attributes) override;

        /**
         * Builds limits attributes
         * @return JSON attributes.
         */
        nlohmann::json write_object() const override;
    };


    typedef uint32_t location_id_t;
    /**
     * Location object
     */
    struct location_s : base_object_s {
    public:
        location_id_t id;

        std::string short_description;
        std::string long_description;
        std::string created_at;
        std::string updated_at;
    public:
        /**
         * Builds node from json attributes.
         * @param attributes Attributes.
         */
        void read_object(const nlohmann::json &attributes) override;
        /**
         * Builds location attributes
         * @return JSON attributes.
         */
        nlohmann::json write_object() const override;
    };

    typedef uint32_t node_id_t;
    /**
     * Node object
     */
    struct node_s : base_object_s {
    public:
        node_id_t id;
        std::string uuid;
        bool is_public;

        std::string name;
        std::string description;
        location_id_t location_id;

        std::string fqdn;
        std::string scheme;

        bool is_behind_proxy;
        bool is_maintenance_mode;

        uint32_t memory;
        uint32_t memory_overallocate;
        uint32_t cpu;
        uint32_t cpu_overallocate;
        uint32_t disk;
        uint32_t disk_overallocate;

        uint32_t upload_size;
        uint32_t daemon_listen;
        uint32_t daemon_sftp;
        std::string daemon_base;

        std::string created_at;
        std::string updated_at;
    public:
        /**
         * Builds node from json attributes.
         * @param attributes Attributes.
         */
        void read_object(const nlohmann::json &attributes) override;
        /**
         * Builds node attributes
         * @return JSON attributes.
         */
        nlohmann::json write_object() const override;
    };

    typedef uint32_t egg_id_t;
    typedef uint32_t nest_id_t;

    typedef uint32_t allocation_id_t;

    /**
     * Allocation object.
     */
    struct allocation_s : base_object_s {
    public:
        allocation_id_t id;
        std::string ip;
        std::string ip_alias;
        uint16_t port;

        std::string notes;
        bool is_default;
    public:
        /**
         * Builds allocation from json attributes.
         * @param attributes Attributes.
         */
        void read_object(const nlohmann::json &attributes) override;

        /**
         * Builds allocation attributes
         * @return JSON attributes.
         */
        nlohmann::json write_object()  const override;
    };

    typedef uint32_t server_id_t;
    typedef std::string server_external_id_t;

    /**
     * Server object.
     */
    struct server_s : base_object_s {
    public:
        std::string server_owner;
        std::string identifier;
        std::string description;

        egg_id_t egg;
        nest_id_t nest;
        node_id_t node;

        allocation_id_t allocation;

        server_id_t id;
        server_external_id_t external_id;

        std::string uuid;
        std::string name;

        std::string created_at;
        std::string updated_at;

        pterocxx::allocation_s sftp_details;
        pterocxx::limits_s limits;
        pterocxx::feature_limits_s feature_limits;

        bool is_suspended;
        bool is_installing;

        std::vector<pterocxx::allocation_s> allocations;
    public:
        /**
         * Builds server from json attributes.
         * @param attributes Attributes.
         */
        void read_object(const nlohmann::json &attributes) override;
        /**
         * Builds server attributes
         * @return JSON attributes.
         */
        nlohmann::json write_object()  const override;
    };

    typedef uint32_t user_id_t;
    typedef std::string user_external_id_t;

    /**
     * User object.
     */
    struct user_s : public base_object_s {
    public:
        user_id_t id;
        user_external_id_t external_id;
        std::string uuid;

        std::string username;
        std::string email;
        std::string first_name;
        std::string last_name;
        std::string language;
        std::string password;

        bool root_admin;
        bool two_factor_auth;

        std::string created_at;
        std::string updated_at;

        std::vector<server_s> servers;
    public:
        user_s& set_username(const std::string& username);
        user_s& set_email(const std::string& email);
        user_s& set_first_name(const std::string& first_name);
        user_s& set_last_name(const std::string& last_name);
        user_s& set_password(const std::string& password);

    public:
        /**
         * Builds error from json attributes.
         * @param attributes Attributes.
         */
        void read_object(const nlohmann::json &attributes) override;
        /**
         * Builds user attributes
         * @return JSON attributes.
         */
        nlohmann::json write_object() const override;
    };




}

#endif //PTEROCXX_TYPES_HPP
