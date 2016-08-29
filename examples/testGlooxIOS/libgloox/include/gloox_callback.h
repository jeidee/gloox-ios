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

    typedef void (*callbackRosterFunc)(const char* jid, const char* nickname, int subscription, const char* groups);
    typedef void (*callbackVCardFunc)(const char* jid, const char* nickname, const char* photo);
    typedef void (*callbackPresenceFunc)(const char* rosterJid, const char* rosterNickname, int rosterSubscription, const char* resource, int presence, const char* msg);
    typedef void (*callbackSubscriptionRequestFunc)(const char* jid, const char* msg);
    typedef void (*callbackUnsubscriptionRequestFunc)(const char* jid, const char* msg);
    typedef void (*callbackItemSubscribedFunc)(const char* jid);
    typedef void (*callbackItemAddedFunc)(const char* jid);
    typedef void (*callbackItemUnsubscribedFunc)(const char* jid);
    typedef void (*callbackItemRemovedFunc)(const char* jid);
    typedef void (*callbackItemUpdatedFunc)(const char* jid);
    typedef void (*callbackMessageFunc)(const char* jid, int msgType, const char* subject, const char* body, const char* thread, void* session);

    class IGlooxCallback {
    public:
        virtual void onConnect(void) = 0;
        virtual void onDisconnect(int e) = 0;
        virtual void onLog(const char* tag, const char* log) = 0;
        virtual void onRoster(const char* jid, const char* nickname, int subscription, const char* groups) = 0;
        virtual void onVCard(const char* jid, const char* nickname, const char* photo) = 0;
        virtual void onPresence(const char* rosterJid, const char* rosterNickname, int rosterSubscription, const char* resource, int presence, const char* msg) = 0;
        virtual void onSubscriptionRequest(const char* jid, const char* msg) = 0;
        virtual void onUnsubscriptionRequest(const char* jid, const char* msg) = 0;
        virtual void onItemSubscribed(const char* jid) = 0;
        virtual void onItemAdded(const char* jid) = 0;
        virtual void onItemUnsubscribed(const char* jid) = 0;
        virtual void onItemRemoved(const char* jid) = 0;
        virtual void onItemUpdated(const char* jid) = 0;
        virtual void onMessage(const char* jid, int msgType, const char* subject, const char* body, const char* thread, void* session) = 0;
    };
}

#endif /* GLOOX_CALLBACK_H__ */
