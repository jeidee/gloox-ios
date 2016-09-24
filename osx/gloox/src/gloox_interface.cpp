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

void gwRegisterCallback(void* glooxWrapper, IGlooxCallback* callback) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;

    instance->registerCallback(callback);
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

void gwRegisterOnRoster(void* glooxWrapper, callbackRosterFunc callback) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->registerOnRoster(callback);
}

void gwRegisterOnVCard(void* glooxWrapper, callbackVCardFunc callback) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->registerOnVCard(callback);
}

void gwRegisterOnPresence(void* glooxWrapper, callbackPresenceFunc callback) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->registerOnPresence(callback);
}

void gwRegisterOnSubscriptionRequest(void* glooxWrapper, callbackSubscriptionRequestFunc callback) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->registerOnSubscriptionRequest(callback);
}

void gwRegisterOnUnsubscriptionRequest(void* glooxWrapper, callbackUnsubscriptionRequestFunc callback) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->registerOnUnsubscriptionRequest(callback);
}

void gwRegisterOnItemSubscribed(void* glooxWrapper, callbackItemSubscribedFunc callback) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->registerOnItemSubscribed(callback);
}

void gwRegisterOnItemAdded(void* glooxWrapper, callbackItemAddedFunc callback) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->registerOnItemAdded(callback);
}

void gwRegisterOnItemUnsubscribed(void* glooxWrapper, callbackItemUnsubscribedFunc callback) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->registerOnItemUnsubscribed(callback);
}

void gwRegisterOnItemRemoved(void* glooxWrapper, callbackItemRemovedFunc callback) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->registerOnItemRemoved(callback);
}

void gwRegisterOnItemUpdated(void* glooxWrapper, callbackItemUpdatedFunc callback) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->registerOnItemUpdated(callback);
}

void gwRegisterOnMessage(void* glooxWrapper, callbackMessageFunc callback) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->registerOnMessage(callback);
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

void gwFetchVCard(void* glooxWrapper, const char* jid) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->fetchVCard(jid);
}

void gwSetPresence(void* glooxWrapper, int presenceType, int priority, const char* status) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->setPresence(presenceType, priority, status);
}

void gwSubscribe(void* glooxWrapper, const char* jid, const char* name, const char* groups, const char* msg) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->subscribe(jid, name, groups, msg);
}

void gwUnsubscribe(void* glooxWrapper, const char* jid, const char* msg) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->unsubscribe(jid, msg);
}

void gwGetRosterItem(void* glooxWrapper, const char* jid) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->getRosterItem(jid);
}

void gwAckSubscriptionRequest(void* glooxWrapper, const char* jid, bool ack) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->ackSubscriptionRequest(jid, ack);
}

void gwRemoveRoster(void* glooxWrapper, const char* jid) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->remove(jid);
}

void gwSendMessage(void* glooxWrapper, const char* jid, const char* msg) {
    GlooxWrapper* instance = (GlooxWrapper*)glooxWrapper;
    
    instance->sendMessage(jid, msg);
}