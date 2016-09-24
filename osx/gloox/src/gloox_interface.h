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
    // handling a instance of gloox wrapper
    void* gwCreateInstance();
    void gwDeleteInstance(void* glooxWrapper);
    
    // registering callback interface for xmpp events
    void gwRegisterCallback(void* glooxWrapper, IGlooxCallback* callback);
    // registering callback functions for xmpp events
    void gwRegisterOnConnect(void* glooxWrapper, callbackConnectFunc callback);
    void gwRegisterOnDisconnect(void* glooxWrapper, callbackDisconnectFunc callback);
    void gwRegisterOnLog(void* glooxWrapper, callbackLogFunc callback);

    void gwRegisterOnRoster(void* glooxWrapper, callbackRosterFunc callback);
    void gwRegisterOnVCard(void* glooxWrapper, callbackVCardFunc callback);
    void gwRegisterOnPresence(void* glooxWrapper, callbackPresenceFunc callback);
    void gwRegisterOnSubscriptionRequest(void* glooxWrapper, callbackSubscriptionRequestFunc callback);
    void gwRegisterOnUnsubscriptionRequest(void* glooxWrapper, callbackUnsubscriptionRequestFunc callback);
    void gwRegisterOnItemSubscribed(void* glooxWrapper, callbackItemSubscribedFunc callback);
    void gwRegisterOnItemAdded(void* glooxWrapper, callbackItemAddedFunc callback);
    void gwRegisterOnItemUnsubscribed(void* glooxWrapper, callbackItemUnsubscribedFunc callback);
    void gwRegisterOnItemRemoved(void* glooxWrapper, callbackItemRemovedFunc callback);
    void gwRegisterOnItemUpdated(void* glooxWrapper, callbackItemUpdatedFunc callback);
    void gwRegisterOnMessage(void* glooxWrapper, callbackMessageFunc callback);
    
    // connection
    bool gwConnect(void* glooxWrapper, const char* jid, const char* pwd, const char* host, int port);
    void gwDisconnect(void* glooxWrapper);

    // receive xmpp packets, this function cause blocking a thread
    void gwRecv(void* glooxWrapper);

    // commands of xmpp
    void gwFetchVCard(void* glooxWrapper, const char* jid);
    void gwSetPresence(void* glooxWrapper, int presenceType, int priority, const char* status);
    void gwSubscribe(void* glooxWrapper, const char* jid, const char* name, const char* groups, const char* msg);
    void gwUnsubscribe(void* glooxWrapper, const char* jid, const char* msg);
    void gwGetRosterItem(void* glooxWrapper, const char* jid);
    void gwAckSubscriptionRequest(void* glooxWrapper, const char* jid, bool ack);
    void gwRemoveRoster(void* glooxWrapper, const char* jid);
    void gwSendMessage(void* glooxWrapper, const char* jid, const char* msg);
}

#endif /* GLOOX_INTERFACE_H__ */
