//
//  gloox_interface.cpp
//
//  Created by jeidee@gmail.com on 2016. 8. 26..
//  Copyright © 2016년 jeidee. All rights reserved.
//


#include "gloox_interface.h"
#include "gloox_wrapper.h"

using namespace jd;

void* gwCreateInstance() {
    return new GlooxWrapper();
}

void gwDeleteInstance(void* glooxWrapper) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;

    if (instance != NULL) {
        delete instance;
    }
}

void gwRegisterOnConnect(void* glooxWrapper, callbackConnectFunc callback) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->registerOnConnect(callback);
}

void gwRegisterOnDisconnect(void* glooxWrapper, callbackDisconnectFunc callback) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->registerOnDisconnect(callback);
}

void gwRegisterOnLog(void* glooxWrapper, callbackLogFunc callback) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->registerOnLog(callback);
}

bool gwConnect(void* glooxWrapper, const char* jid, const char* pwd, const char* host, int port) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;

    instance->setLoginInfo(jid, pwd, host, port);

    return instance->connect();
}

void gwDisconnect(void* glooxWrapper) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->disConnect();
}

void gwRecv(void* glooxWrapper) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->recv();
}