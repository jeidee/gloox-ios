//
//  GlooxInterfaceIOS.h
//  testGlooxIOS
//
//  Created by 신창헌 on 2016. 8. 29..
//  Copyright © 2016년 jeidee. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "gloox_interface.h"
#include <thread>

class GlooxInterface : public IGlooxCallback {
public:
    GlooxInterface(void* self);
    ~GlooxInterface();
    
#pragma mark IGlooxCallback
    virtual void onConnect(void);
    virtual void onDisconnect(int e);
    virtual void onLog(const char* tag, const char* log);
    virtual void onRoster(const char* jid, const char* nickname, int subscription, const char* groups);
    virtual void onVCard(const char* jid, const char* nickname, const char* photo);
    virtual void onPresence(const char* rosterJid, const char* rosterNickname, int rosterSubscription, const char* resource, int presence, const char* msg);
    virtual void onSubscriptionRequest(const char* jid, const char* msg);
    virtual void onUnsubscriptionRequest(const char* jid, const char* msg);
    virtual void onItemSubscribed(const char* jid);
    virtual void onItemAdded(const char* jid);
    virtual void onItemUnsubscribed(const char* jid);
    virtual void onItemRemoved(const char* jid);
    virtual void onItemUpdated(const char* jid);
    virtual void onMessage(const char* jid, int msgType, const char* subject, const char* body, const char* thread, void* session);
    
#pragma mark Receiver worker thread
    bool isStop() { return m_isStop; }
    static void recvLoop(void* gloox, GlooxInterface* glooxInterface);
    
#pragma mark Commands for XMPP

    // commands for xmpp
    bool connect(const char* jid, const char* pwd, const char* host, int port);
    void disConnect();
    
    void fetchVCard( const char* jid);
    void setPresence(int presenceType, int priority, const char* status);
    void subscribe(const char* jid, const char* name, const char* groups, const char* msg);
    void unsubscribe(const char* jid, const char* msg);
    void getRosterItem(const char* jid);
    void ackSubscriptionRequest(const char* jid, bool ack);
    void removeRoster(const char* jid);
    void sendMessage(const char* jid, const char* msg);
    
private:
    void* m_self;
    void* m_gloox;
    bool m_isStop;
    std::thread* m_recvThread;
};


@protocol GlooxDelegate <NSObject>
@optional
- (void) onConnect;
- (void) onDisconnect:(int) e;
- (void) onLog:(NSString*)tag withLog:(NSString*)log;
- (void) onRoster:(NSString*)jid withNickname:(NSString*) nickname withSubscription:(int) subscription withGroups:(NSString*) groups;
- (void) onVCard:(NSString*)jid withNickname:(NSString*)nickname withPhoto:(NSString*)photo;
- (void) onPresence:(NSString*) rosterJid withNickname:(NSString*) rosterNickname withSubscription:(int) rosterSubscription withResource:(NSString*) resource withPresence:(int)presenceType withMsg:(NSString*) msg;
- (void) onSubscriptionRequest:(NSString*)jid withMsg:(NSString*)msg;
- (void) onUnsubscriptionRequest:(NSString*)jid withMsg:(NSString*)msg;
- (void) onItemSubscribed:(NSString*)jid;
- (void) onItemAdded:(NSString*)jid;
- (void) onItemUnsubscribed:(NSString*)jid;
- (void) onItemRemoved:(NSString*)jid;
- (void) onItemUpdated:(NSString*)jid;
- (void) onMessage:(NSString*)jid withMsgType:(int) msgType withSubject:(NSString*) subject withBody:(NSString*) body withThread:(NSString*) thread withSession:(void*) session;

@end

@interface GlooxInterfaceIOS : NSObject<GlooxDelegate> {
    GlooxInterface* m_glooxInterface;
    NSMutableArray* m_eventHandlerList;
}

+(GlooxInterfaceIOS*)getInstance;

- (id)init;

#pragma mark For EventHandlers
- (void)addEventHandler:(id <GlooxDelegate>)eventHandler;
- (void)removeEventHandler:(id <GlooxDelegate>)eventHandler;

#pragma mark API for XMPP
- (bool)connect:(NSString*)jid withPwd:(NSString*) pwd withHost:(NSString*) host withPort:(int) port;
- (void)disConnect;

- (void)fetchVCard:(NSString*) jid;
- (void)setPresence:(int) presenceType withPriority:(int) priority withStatus:(NSString*) status;
- (void)subscribe:(NSString*) jid withName:(NSString*) name withGroups:(NSString*) groups withMsg:(NSString*) msg;
- (void)unsubscribe:(NSString*) jid withMsg:(NSString*) msg;
- (void)getRosterItem:(NSString*) jid;
- (void)ackSubscriptionRequest:(NSString*) jid withAck:(bool) ack;
- (void)removeRoster:(NSString*) jid;
- (void)sendMessage:(NSString*) jid withMsg:(NSString*) msg;

@end
