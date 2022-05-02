# pterocxx
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/cc1571a8ba3d4c2c8feb28a4bcd1a761)](https://www.codacy.com/gh/rgnter/pterocxx/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=rgnter/pterocxx&amp;utm_campaign=Badge_Grade)
[![Build Linux](https://github.com/rgnter/pterocxx/actions/workflows/build_linux.yml/badge.svg)](https://github.com/rgnter/pterocxx/actions/workflows/build_linux.yml)

Lightweight pterodactyl API library written in c++.

## Developers
### Setup CMake
Prerequisites:
- Conan

Clone pterocxx repository,
```bash
git submodule add https://github.com/rgnter/pterocxx vendor/pterocxx
```
add it as a subproject in CMake, and link aganist it.
```cmake
# Add pterocxx subproject
add_subdirectory(vendor/pterocxx)

# Include & Link pterocxx
include_directories(vendor/pterocxx/include)
link_libraries(pterocxx)
```
### Basic API
#### General
To do anything, you will need `pterocxx::application` instance,
```cxx
// constructs application
pterocxx::application app("panel.example.net",
                          "app_key"
                        // port = 443,          Server port
                        // app_name="pterocxx"  Used in user agent when sending requests
);                  
// initialize application
app.init();
/* do some work*/
// synchronize with application thread
app.sync();
```
it contains info such as endpoint *hostname*, *application key*, *endpoint port* & *optional application name*.
Application is then **intialized** with `pterocxx::application::init`, it spawns thread and begins work. You can **sychronize** with the spawned thread by calling `pterocxx::application::sync`. Application is **terminated** by calling `pterocxx::application::term`
#### Errors
Endpoint can return errors when calling any request, you can query them as such:
```cxx
app.any_request([](const pterocxx::any_response& response) {
  if(!response.successful)
      for (const auto &error : response.errors) {
          printf("Error: %s, %s, %s\n", error.detail.c_str(), error.code.c_str(), error.status.c_str());
      }
});
```
you can read more about errors on [Pterodactyl API Reference](https://dashflo.net/docs/api/pterodactyl/v1/).
#### Naming Conventions
Every request has name, such as `get_users`, `get_servers`. Every request has response which name begins with *(request_name)\_response\_(s - struct)*, such as `get_users_response`, `get_servers_response`.

### User API
#### Get users
Retrieve all users.
```cxx
app.get_users([](const pterocxx::get_users_response_s& response) {
    // users container
    const pterocxx::list_object_s<pterocxx::user_s> users = response.users;
    // loop through all the users, and print their names
    for (const auto &user : users.data) {
        printf("Username: %s\n", user.username.c_str());
    }
});
```

#### Get user details
Retrieve user details.
```cxx
// user id
const pterocxx::user_id_t user_id = 1;
app.get_user_details(user_id, [](const pterocxx::get_user_details_response_s& response) {
    const pterocxx::user_s user = response.user;
    // print username
    printf("Username: %s\n", user.username.c_str());
});
```

#### Create user
Create user.
```cxx
// immutable user model
pterocxx::user_s user;
user.set_username("username")
    .set_email("username@example.net")
    .set_first_name("first name")
    .set_last_name("last name");
    
app.create_user(user, [](const pterocxx::create_user_response_s& response) {
    // created user
    const pterocxx::user_s user = response.user;
    // print username and user id
    printf("Username: %s - Id: %d\n", user.username.c_str(), user.id);
});
```
#### Update user
Update user.
```cxx 
pterocxx::user_s user;
user.id = 1;
user.set_password("password");

app.update_user(user.id, user, [](const pterocxx::update_user_response_s& response) {
    // modified user
    const pterocxx::user_s user = response.user;
    // print username and user id
    printf("Username: %s - Id: %d\n", user.username.c_str(), user.id);
});
```
#### Delete user
Delete user.
```cxx 
pterocxx::user_s user;
user.id = 1;
app.delete_user(user.id, [](const pterocxx::delete_user_response_s& response) {
    if(response.successful) {
        printf("Successfully deleted user\n");
    }
});
```
### Building
#### Prerequisites
  - OpenSSL
  - Boost
  - CMake
  - Conan
