#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 5
#define MAX_RESOURCES 5
#define MAX_NAME_LEN 20

typedef enum{
    READ = 1,
    WRITE = 2,
    EXECUTE = 4
}Permission;

//User and Resource Definitions
typedef struct{
    char name[MAX_NAME_LEN];
}User;

typedef struct{
    char name[MAX_NAME_LEN];
}Resource;

//ACL Entry
typedef struct{
    char username[MAX_NAME_LEN];
    int permissions;
}ACLEntry;

typedef struct{
    Resource resource;
    ACLEntry acl[MAX_USERS];
    int aclCount;
}ACLControlledResource;

//Capability Entry
typedef struct{
    char resourceName[MAX_NAME_LEN];
    int permissions;
}Capability;

typedef struct{
    User user;
    Capability caps[MAX_RESOURCES];
    int capCount;
}CapabilityUser;

//Utility Functions
void printPermissions(int perm){
    if(perm == READ) printf("READ");
    else if(perm == WRITE) printf("WRITE");
    else if(perm == EXECUTE) printf("EXECUTE");
    else if(perm == (READ | WRITE)) printf("READ WRITE");
    else if(perm == (READ | EXECUTE)) printf("READ EXECUTE");
    else if(perm == (WRITE | EXECUTE)) printf("WRITE EXECUTE");
    else if(perm == (READ | WRITE | EXECUTE)) printf("READ WRITE EXECUTE");
}

int hasPermission(int userPerm, int requiredPerm){
    return (userPerm & requiredPerm) == requiredPerm;
}

// Optional enhancement functions
void addACLEntry(ACLControlledResource *res, const char *username, int permissions) {
    if(res->aclCount < MAX_USERS) {
        strcpy(res->acl[res->aclCount].username, username);
        res->acl[res->aclCount].permissions = permissions;
        res->aclCount++;
    }
}

void addCapability(CapabilityUser *user, const char *resourceName, int permissions) {
    if(user->capCount < MAX_RESOURCES) {
        strcpy(user->caps[user->capCount].resourceName, resourceName);
        user->caps[user->capCount].permissions = permissions;
        user->capCount++;
    }
}

//ACL System
void checkACLAccess(ACLControlledResource *res, const char *userName, int perm){
    int i;
    for(i = 0; i < res->aclCount; i++){
        if(strcmp(res->acl[i].username, userName) == 0){
            if(hasPermission(res->acl[i].permissions, perm)){
                printf("ACL Check: User %s requests ", userName);
                printPermissions(perm);
                printf(" on %s: Access GRANTED\n", res->resource.name);
                return;
            }
            else{
                printf("ACL Check: User %s requests ", userName);
                printPermissions(perm);
                printf(" on %s: Access DENIED\n", res->resource.name);
                return;
            }
        }
    }
    printf("ACL Check: User %s has NO entry for resource %s: Access DENIED\n", userName, res->resource.name);
}

//Capability System
void checkCapabilityAccess(CapabilityUser *user, const char *resourceName, int perm){
    int j;
    for(j = 0; j < user->capCount; j++){
        if(strcmp(user->caps[j].resourceName, resourceName) == 0){
            if(hasPermission(user->caps[j].permissions, perm)){
                printf("Capability Check: User %s requests ", user->user.name);
                printPermissions(perm);
                printf(" on %s: Access GRANTED\n", resourceName);
                return;
            }
            else{
                printf("Capability Check: User %s requests ", user->user.name);
                printPermissions(perm);
                printf(" on %s: Access DENIED\n", resourceName);
                return;
            }
        }
    }
    printf("Capability Check: User %s has NO capability for %s: Access DENIED\n", user->user.name, resourceName);
}

int main(){
    //Sample users and resources
    User users[MAX_USERS] = {{"Alice"}, {"Bob"}, {"Charlie"}, {"David"}, {"Eve"}};
    Resource resources[MAX_RESOURCES] = {{"File1"}, {"File2"}, {"File3"}, {"Database1"}, {"Script1"}};

    //ACL Setup
    ACLControlledResource aclResources[MAX_RESOURCES];
    
    // Initialize all ACL resources
    int k;
    for(k = 0; k < MAX_RESOURCES; k++){
        aclResources[k].resource = resources[k];
        aclResources[k].aclCount = 0;
    }
    
    // Setup ACL for File1
    addACLEntry(&aclResources[0], "Alice", READ | WRITE);
    addACLEntry(&aclResources[0], "Bob", READ);
    
    // Setup ACL for File2
    addACLEntry(&aclResources[1], "Charlie", READ);
    addACLEntry(&aclResources[1], "David", WRITE);
    
    // Setup ACL for File3
    addACLEntry(&aclResources[2], "Alice", EXECUTE);
    addACLEntry(&aclResources[2], "Eve", READ | EXECUTE);
    
    // Setup ACL for Database1
    addACLEntry(&aclResources[3], "David", READ | WRITE);
    addACLEntry(&aclResources[3], "Eve", READ);
    
    // Setup ACL for Script1
    addACLEntry(&aclResources[4], "Alice", EXECUTE);
    addACLEntry(&aclResources[4], "Charlie", READ | EXECUTE);

    //Capability Setup
    CapabilityUser capUsers[MAX_USERS];
    
    // Initialize all capability users
    for(k = 0; k < MAX_USERS; k++){
        capUsers[k].user = users[k];
        capUsers[k].capCount = 0;
    }
    
    // Alice capabilities
    addCapability(&capUsers[0], "File1", READ | WRITE);
    addCapability(&capUsers[0], "File3", EXECUTE);
    addCapability(&capUsers[0], "Script1", EXECUTE);
    
    // Bob capabilities
    addCapability(&capUsers[1], "File1", READ);
    
    // Charlie capabilities
    addCapability(&capUsers[2], "File2", READ);
    addCapability(&capUsers[2], "Script1", READ | EXECUTE);
    
    // David capabilities
    addCapability(&capUsers[3], "File2", WRITE);
    addCapability(&capUsers[3], "Database1", READ | WRITE);
    
    // Eve capabilities
    addCapability(&capUsers[4], "File3", READ | EXECUTE);
    addCapability(&capUsers[4], "Database1", READ);

    //Test ACL
    checkACLAccess(&aclResources[0], "Alice", READ);
    checkACLAccess(&aclResources[0], "Bob", WRITE);
    checkACLAccess(&aclResources[1], "Charlie", READ);
    checkACLAccess(&aclResources[1], "David", WRITE);
    checkACLAccess(&aclResources[2], "Alice", EXECUTE);
    checkACLAccess(&aclResources[3], "David", READ);
    checkACLAccess(&aclResources[3], "Eve", WRITE);
    checkACLAccess(&aclResources[4], "Charlie", EXECUTE);

    //Test Capability
    checkCapabilityAccess(&capUsers[0], "File1", WRITE);
    checkCapabilityAccess(&capUsers[1], "File1", WRITE);
    checkCapabilityAccess(&capUsers[2], "File2", READ);
    checkCapabilityAccess(&capUsers[4], "File3", EXECUTE);
    checkCapabilityAccess(&capUsers[3], "Database1", WRITE);
    checkCapabilityAccess(&capUsers[4], "File3", READ);
    checkCapabilityAccess(&capUsers[0], "Script1", EXECUTE);
    checkCapabilityAccess(&capUsers[4], "Database1", READ);
    checkCapabilityAccess(&capUsers[2], "File2", READ);

    return 0;
}