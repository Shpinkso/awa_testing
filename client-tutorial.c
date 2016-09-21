#include <stdlib.h>
#include <stdio.h>

#include <awa/common.h>
#include <awa/client.h>

#define OPERATION_PERFORM_TIMEOUT 1000

static void DefineHeaterObject(AwaClientSession * session)
{
    AwaError ret;
    // Create and Object Definition with ID 1000, called Heater, between 0 and 1 instances allowed.
    AwaObjectDefinition * objectDefinition = AwaObjectDefinition_New(1000, "Heater", 0, 1);
    // Create a resource on this Object, ID 101, called Manufacturer, is not mandatory, is read/writable, no default value
    ret = AwaObjectDefinition_AddResourceDefinitionAsString(objectDefinition, 101, "Manufacturer", false, AwaResourceOperations_ReadWrite, NULL);
    if(ret)
        printf("AwaObjectDefinition_AddResourceDefinitionAsString failed with return %x\n\r",ret);
    // Create a resource on this Object, ID 102, called Temperature, is mandatory, is read/writable, default value 0.0
    ret = AwaObjectDefinition_AddResourceDefinitionAsFloat(objectDefinition, 104, "Temperature", false, AwaResourceOperations_ReadWrite, 0.0);
    if (ret)
        printf("AwaObjectDefinition_AddResourceDefinitionAsFloat failed with return %x\n\r",ret);

    // Create a new Define operation on the client session we created
    AwaClientDefineOperation * operation = AwaClientDefineOperation_New(session);
    // Add the Define operation on the "Heater" object
    ret = AwaClientDefineOperation_Add(operation, objectDefinition);
    if (ret)
        printf("AwaClientDefineOperation_Add failed with return %x\n\r",ret);
    // Perform the Define operation to add the object definition? Why do we need to do this?
    ret = AwaClientDefineOperation_Perform(operation, OPERATION_PERFORM_TIMEOUT);
    if (ret)
        printf("AwaClientDefineOperation_Perform failed with return %x\n\r",ret);
    // Finished with the Define operation and it's a local resource, so free it
    ret = AwaClientDefineOperation_Free(&operation);
    if (ret)
        printf("AwaClientDefineOperation_Free failed with return %x\n\r",ret);
}

static void SetInitialValues(AwaClientSession * session)
{
    AwaError ret;
    // Create new Set operation
    AwaClientSetOperation * operation = AwaClientSetOperation_New(session);

    // Create an instance (0) of object 1000
    ret = AwaClientSetOperation_CreateObjectInstance(operation, "/1000/0");
    if (ret)
        printf("AwaClientSetOperation_CreateObjectInstance failed with return %x\n\r",ret);
    // Create a resource on instance 0 of object 1000, resource type 101
    ret = AwaClientSetOperation_CreateOptionalResource(operation, "/1000/0/101");
    if (ret)
        printf("AwaClientSetOperation_CreateOptionalResource 101 failed with return %x\n\r",ret);
    // Set the value of /1000/0/101 to "HotAir Systems Inc"
    ret = AwaClientSetOperation_AddValueAsCString(operation, "/1000/0/101", "HotAir Systems Inc");
    if (ret)
        printf("AwaClientSetOperation_AddValueAsCString 101 failed with return %x\n\r",ret);
    // Set the value of /100/0/102 to "20.02"
    ret = AwaClientSetOperation_AddValueAsFloat(operation, "/1000/0/104", (float)(20.02));
    if (ret)
        printf("AwaClientSetOperation_AddValueAsFloat 104 failed with return %x\n\r",ret);
    // Create a resource on instance 0 of object 1000, resource type 104
    ret = AwaClientSetOperation_CreateOptionalResource(operation, "/1000/0/104");
    if (ret)
        printf("AwaClientSetOperation_CreateOptionalResource 104 failed with return %x\n\r",ret);
    // Perform the above, obivously the core will take care of the order of operation.
    ret = AwaClientSetOperation_Perform(operation, OPERATION_PERFORM_TIMEOUT);
    if (ret)
        printf("AwaClientSetOperation_Perform failed with return %x\n\r",ret);
    // Finished with this Set operation and it's a local resource, so free it
    ret = AwaClientSetOperation_Free(&operation);
    if (ret)
        printf("AwaClientSetOperation_Free failed with return %x\n\r",ret);
}

int main(void)
{
    AwaError ret;
    // Create a new Awa Client Session
    AwaClientSession * session = AwaClientSession_New();

    // Connect this session to the mysterious "core"
    ret = AwaClientSession_Connect(session);
    if (ret)
        printf("AwaClientSession_Connect failed with return %x\n\r",ret);

    // Do all the above
    DefineHeaterObject(session);
    SetInitialValues(session);

    // Disconnect this session from the mysterious "core"
    ret = AwaClientSession_Disconnect(session);
    if (ret)
        printf("AwaClientSession_Disconnect failed with return %x\n\r",ret);
    // Free the session
    ret = AwaClientSession_Free(&session);
    if (ret)
        printf("AwaClientSession_Free failed with return %x\n\r",ret);
    return 0;
}
