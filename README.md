# pterocxx
Pterodactyl API in C++.
## Developers
### General
Construct application:
```cxx
// constructs application connecting to panel.example.net
// uses default https port so the rest client is using secured connection
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
Error checking:
```cxx
// if error is thrown by remote, it can be queried.
app.any_request([](const pterocxx::any_response& response) {
  // response.successful is set to false, when remote response code is not appropriate.
  if(!response.sucessful)
      // iterate through errors, print them
      for (const auto &error : response.errors) {
          printf("Error: %s, %s, %s\n", error.detail.c_str(), error.code.c_str(), error.status.c_str());
      }
});
```
## User API
### Get users
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

### Get user details
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

### Create user
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
### Update user
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
### Delete user
Delete user.
```cxx 
pterocxx::user_s user;
user.id = 1;
app.delete_user(user.id, [](const pterocxx::delete_user_response_s& response) {
    if(response.success) {
        printf("Successfully deleted user\n");
    }
});
```
