//
//  gloox_callback.h
//
//  Created by jeidee@gmail.com on 2016. 8. 26..
//  Copyright © 2016년 jeidee. All rights reserved.
//

#ifndef GLOOX_CALLBACK_H__
#define GLOOX_CALLBACK_H__

extern "C" {
    typedef void (*callbackConnectFunc)(void);
    typedef void (*callbackDisconnectFunc)(int e);
    typedef void (*callbackLogFunc)(const char* tag, const char* log);
    
    typedef void (*callbackRosterFunc)(const char* jid
    , const char* nickname
    , int subscription
    , const char* groups
    , int no
    , int totalCount);
    typedef void (*callbackVCardFunc)(const char* jid
    , const char* nickname
    , const char* photo);
    typedef void (*callbackPresenceFunc)(const char* rosterJid
    , const char* rosterNickname
    , int rosterSubscription
    , const char* resource
    , int presence
    , const char* msg);
    typedef void (*callbackSubscriptionRequestFunc)(const char* jid
    , const char* msg);
    typedef void (*callbackUnsubscriptionRequestFunc)(const char* jid
    , const char* msg);
    typedef void (*callbackItemSubscribedFunc)(const char* jid);
    typedef void (*callbackItemAddedFunc)(const char* jid);
    typedef void (*callbackItemUnsubscribedFunc)(const char* jid);
    typedef void (*callbackItemRemovedFunc)(const char* jid);
    typedef void (*callbackItemUpdatedFunc)(const char* jid);
    typedef void (*callbackMessageFunc)(const char* jid
    , int msgType
    , const char* subject
    , const char* body
    , const char* thread
    , void* session);
    
    class IGlooxCallback {
    public:
        virtual void onConnect(void) {}
        virtual void onDisconnect(int e) {}
        virtual void onLog(const char* tag, const char* log) {}
        virtual void onRoster(const char* jid
                              , const char* nickname
                              , int subscription
                              , const char* groups
                              , int no
                              , int totalCount) {}
        virtual void onVCard(const char* jid
                             , const char* nickname
                             , const char* photo) {}
        virtual void onPresence(const char* rosterJid
                                , const char* rosterNickname
                                , int rosterSubscription
                                , const char* resource
                                , int presence
                                , const char* msg) {}
        virtual void onSubscriptionRequest(const char* jid
                                           , const char* msg) {}
        virtual void onUnsubscriptionRequest(const char* jid
                                             , const char* msg) {}
        virtual void onItemSubscribed(const char* jid) {}
        virtual void onItemAdded(const char* jid) {}
        virtual void onItemUnsubscribed(const char* jid) {}
        virtual void onItemRemoved(const char* jid) {}
        virtual void onItemUpdated(const char* jid) {}
        virtual void onMessage(const char* jid
                               , int msgType
                               , const char* subject
                               , const char* body
                               , const char* thread
                               , void* session) {}
    };
}

#endif /* GLOOX_CALLBACK_H__ */
