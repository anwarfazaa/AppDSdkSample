// AppDSdkSample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>


#include "appdynamics.h"
#include "appdynamics_advanced.h"




int start_bt(const char* name)
{
    static unsigned int bt_count = 15;
    appd_bt_handle bt_handle = appd_bt_begin(name, NULL);
    if (bt_handle == NULL)
    {
        printf("Failed to begin transaction\n");
        return -1;
    }
    if (++bt_count % 20 == 0)
    {
        if (appd_bt_enable_snapshot(bt_handle) == 0)
        {
            printf("Failed to snapshot transaction\n");
        }
    }
    appd_frame_handle frame_handle = appd_frame_begin(bt_handle, APPD_FRAME_TYPE_CPP, "STARTBT","start_bt", __FILE__, __LINE__);
    if (frame_handle != NULL)
        printf("\nBT Handle %p, Frame Handle %p\n", bt_handle, frame_handle);
    else
        printf("\nBT Handle %p\n", bt_handle);

   // step1_http(bt_handle);
    appd_bt_add_user_data(bt_handle, "Company", "UBS");
    appd_bt_end(bt_handle);

    return 0;
}

int main()
{
    std::cout << "This is to test AppDynamics C++ SDK\n";
    int valueCheck = 0;
    const char* controller_host = "xxx";
    short controller_port = 8090;
    const char* acct_name = "customer1";
    const char* access_key = "xxx";
    bool use_SSL = false;
    const char*   app_name = "SDKTestApp";
    const char*   tier_name = "SDKTestTier1";
    const char*   node_name = "SDKTestNode1";

    

    struct appd_config* cfg = NULL;

    cfg = appd_config_init();

    if (cfg == NULL) {
        printf("appD_config_init() failed!\n");
        exit(1);
    }

    appd_config_set_controller_host(cfg, controller_host);
    appd_config_set_controller_port(cfg, controller_port);
    appd_config_set_controller_account(cfg, acct_name);
    appd_config_set_controller_access_key(cfg, access_key);
    appd_config_set_controller_use_ssl(cfg, use_SSL);
    appd_config_set_init_timeout_ms(cfg, 10000);
    appd_config_set_app_name(cfg, app_name);
    appd_config_set_tier_name(cfg, tier_name);
    appd_config_set_node_name(cfg, node_name);


    //Preparing to call BTs

    printf("About to call appd_sdk_init() , waiting for Controller response..\n");

    valueCheck = appd_sdk_init(cfg);

    if (valueCheck != 0) {
        printf("appd_sdk_init() failed!\n");
        exit(1);
    }

    //Loop volume
    int loop = 0;
    int limit = 1000;
    int callCount = 1;
    //Producing BT traffic 
    for (loop = 0; loop < limit; loop++)
    {
        Sleep(5000);
        if (start_bt("TestBt") != 0)
        {
            printf("BT failed\n");
            break;
        }    
        printf("BT Count: %p", callCount);
        callCount++;
    }


    //disposing sdk after being done
    appd_sdk_term();
}

