//
//  gloox_interface.h
//
//  Created by jeidee@gmail.com on 2016. 8. 26..
//  Copyright © 2016년 jeidee. All rights reserved.
//

#ifndef GLOOX_INTERFACE_H__
#define GLOOX_INTERFACE_H__

#include <stdio.h>
#include "gloox_callback.h"

extern "C" {
    void* gwCreateInstance();
    void gwDeleteInstance(void* glooxWrapper);
    
    void gwRegisterOnConnect(void* glooxWrapper, callbackConnectFunc callback);
    void gwRegisterOnDisconnect(void* glooxWrapper, callbackDisconnectFunc callback);
    void gwRegisterOnLog(void* glooxWrapper, callbackLogFunc callback);
    
    bool gwConnect(void* glooxWrapper, const char* jid, const char* pwd, const char* host, int port);
    void gwDisconnect(void* glooxWrapper);

    void gwRecv(void* glooxWrapper);
}

#endif /* GLOOX_INTERFACE_H__ */
