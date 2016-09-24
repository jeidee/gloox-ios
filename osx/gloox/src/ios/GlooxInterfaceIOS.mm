//
//  GlooxInterfaceIOS.m
//  testGlooxIOS
//
//  Created by 신창헌 on 2016. 8. 29..
//  Copyright © 2016년 jeidee. All rights reserved.
//

#import "GlooxInterfaceIOS.h"

@implementation GlooxInterfaceIOS

#pragma mark Implementation GlooxInterface

GlooxInterface::GlooxInterface(void* self) :
      m_self(self)
    , m_isStop(true)
    , m_recvThread(NULL) {
    m_gloox = gwCreateInstance();
    gwRegisterCallback(m_gloox, this);
}

GlooxInterface::~GlooxInterface() {
    if (m_gloox != NULL) {
        gwDeleteInstance(m_gloox);
        m_gloox = NULL;
    }
}

void GlooxInterface::onConnect() {
    printf("OnConnect]\n");
    
    [(__bridge GlooxInterfaceIOS*)m_self onConnect];
}

void GlooxInterface::onDisconnect(int e) {
    printf("OnDisconnect] e: %d\n", e);

    [(__bridge GlooxInterfaceIOS*)m_self onDisconnect:e];
}

void GlooxInterface::onLog(const char* tag, const char* log) {
    printf("%s :: %s\n", tag, log);

    [(__bridge GlooxInterfaceIOS*)m_self onLog:[NSString stringWithUTF8String:tag] withLog:[NSString stringWithUTF8String:log]];
}

void GlooxInterface::onRoster(const char* jid, const char* nickname, int subscription, const char* groups) {
    printf("OnRoster] jid:%s, nickname:%s, subscription:%d, groups:%s\n", jid, nickname, subscription, groups);

    [(__bridge GlooxInterfaceIOS*)m_self onRoster:[NSString stringWithUTF8String:jid] withNickname:[NSString stringWithUTF8String:nickname] withSubscription:subscription withGroups:[NSString stringWithUTF8String:groups]];
}

void GlooxInterface::onVCard(const char* jid, const char* nickname, const char* photo) {
    printf("OnVCard] jid:%s, nickname:%s, photo:%s\n", jid, nickname, photo);
    
    [(__bridge GlooxInterfaceIOS*)m_self onVCard:[NSString stringWithUTF8String:jid] withNickname:[NSString stringWithUTF8String:nickname] withPhoto:[NSString stringWithUTF8String:photo]];
}

void GlooxInterface::onPresence(const char* rosterJid, const char* rosterNickname, int rosterSubscription, const char* resource, int presence, const char* msg) {
    printf("OnPresence] rosterJid:%s, rosterNickname:%s, rosterSubscription:%d, resource:%s, presence:%d, msg:%s\n", rosterJid, rosterNickname, rosterSubscription, resource, presence, msg);
    
    [(__bridge GlooxInterfaceIOS*)m_self onPresence:[NSString stringWithUTF8String:rosterJid] withNickname:[NSString stringWithUTF8String:rosterNickname] withSubscription:rosterSubscription withResource:[NSString stringWithUTF8String:resource] withPresence:presence withMsg:[NSString stringWithUTF8String:msg]];
}

void GlooxInterface::onSubscriptionRequest(const char* jid, const char* msg) {
    printf("OnSubscriptionRequest] jid:%s, msg:%s\n", jid, msg);
    
    [(__bridge GlooxInterfaceIOS*)m_self onSubscriptionRequest:[NSString stringWithUTF8String:jid] withMsg:[NSString stringWithUTF8String:msg]];
}

void GlooxInterface::onUnsubscriptionRequest(const char* jid, const char* msg) {
    printf("OnUnsubscriptionRequest] jid:%s, msg:%s\n", jid, msg);
    
    [(__bridge GlooxInterfaceIOS*)m_self onUnsubscriptionRequest:[NSString stringWithUTF8String:jid] withMsg:[NSString stringWithUTF8String:msg]];
}

void GlooxInterface::onItemSubscribed(const char* jid) {
    printf("OnItemSubscribed] jid:%s\n", jid);
    
    [(__bridge GlooxInterfaceIOS*)m_self onItemSubscribed:[NSString stringWithUTF8String:jid]];
}

void GlooxInterface::onItemAdded(const char* jid) {
    printf("OnItemAdded] jid:%s\n", jid);
    
    [(__bridge GlooxInterfaceIOS*)m_self onItemAdded:[NSString stringWithUTF8String:jid]];
}

void GlooxInterface::onItemUnsubscribed(const char* jid) {
    printf("OnItemUnsubscribed] jid:%s\n", jid);
    
    [(__bridge GlooxInterfaceIOS*)m_self onItemUnsubscribed:[NSString stringWithUTF8String:jid]];

}

void GlooxInterface::onItemRemoved(const char* jid) {
    printf("OnItemRemoved] jid:%s\n", jid);
    
    [(__bridge GlooxInterfaceIOS*)m_self onItemRemoved:[NSString stringWithUTF8String:jid]];
}

void GlooxInterface::onItemUpdated(const char* jid) {
    printf("OnItemUpdated] jid:%s\n", jid);
    
    [(__bridge GlooxInterfaceIOS*)m_self onItemUpdated:[NSString stringWithUTF8String:jid]];
}

void GlooxInterface::onMessage(const char* jid, int msgType, const char* subject, const char* body, const char* thread, void* session) {
    printf("OnMessage] jid:%s, msgType:%d, subject:%s, body:%s, thread:%s\n", jid, msgType, subject, body, thread);
    
    [(__bridge GlooxInterfaceIOS*)m_self onMessage:[NSString stringWithUTF8String:jid] withMsgType:msgType withSubject:[NSString stringWithUTF8String:subject] withBody:[NSString stringWithUTF8String:body] withThread:[NSString stringWithUTF8String:thread] withSession:session];
}


void GlooxInterface::recvLoop(void* gloox, GlooxInterface* glooxInterface) {
    while(true) {
        if (glooxInterface->isStop()) {
            break;
        }
        gwRecv(gloox);
    }
}

bool GlooxInterface::connect(const char* jid, const char* pwd, const char* host, int port) {
    if (gwConnect(m_gloox, jid, pwd, host, port)) {
        m_isStop = false;
        m_recvThread = new std::thread(recvLoop, m_gloox, this);
        return true;
    } else {
        return false;
    }
}

void GlooxInterface::disConnect() {
    gwDisconnect(m_gloox);

    m_isStop = true;
    m_recvThread->join();
    
    delete m_recvThread;
    m_recvThread = NULL;
    
}

void GlooxInterface::fetchVCard(const char* jid) {
    gwFetchVCard(m_gloox, jid);
}

void GlooxInterface::setPresence(int presenceType, int priority, const char* status) {
    gwSetPresence(m_gloox, presenceType, priority, status);
}

void GlooxInterface::subscribe(const char* jid, const char* name, const char* groups, const char* msg) {
    gwSubscribe(m_gloox, jid, name, groups, msg);
}

void GlooxInterface::unsubscribe(const char* jid, const char* msg) {
    gwUnsubscribe(m_gloox, jid, msg);
}

void GlooxInterface::getRosterItem(const char* jid) {
    gwGetRosterItem(m_gloox, jid);
}

void GlooxInterface::ackSubscriptionRequest(const char* jid, bool ack) {
    gwAckSubscriptionRequest(m_gloox, jid, ack);
}

void GlooxInterface::removeRoster(const char* jid) {
    gwRemoveRoster(m_gloox, jid);
}

void GlooxInterface::sendMessage(const char* jid, const char* msg) {
    gwSendMessage(m_gloox, jid, msg);
}


#pragma mark GlooxInterfaceIOS

+(GlooxInterfaceIOS*)getInstance {
    static dispatch_once_t pred;
    static GlooxInterfaceIOS* instance = nil;
    
    dispatch_once(&pred, ^{
        instance = [[GlooxInterfaceIOS alloc] init];
    });
    
    return instance;
}

-(id)init {
    self = [super init];
    if (self) {
        m_eventHandlerList = [[NSMutableArray alloc] init];
        m_glooxInterface = new GlooxInterface((__bridge void*)self);
    }
    return self;
}

#pragma mark Event Handler

-(void)addEventHandler:(id <GlooxDelegate>)eventHandler {
    [m_eventHandlerList addObject:eventHandler];
}

-(void)removeEventHandler:(id <GlooxDelegate>)eventHandler {
    for (id eh in m_eventHandlerList) {
        if ([eh isEqual:eventHandler]) {
            [m_eventHandlerList removeObject:eventHandler];
            return;
        }
    }
}

#pragma mark API for XMPP


#pragma mark Implementation GlooxDelegate

- (void) onConnect {
    dispatch_async(dispatch_get_main_queue(), ^ {
        for (id eventHandler in m_eventHandlerList) {
            if ([eventHandler respondsToSelector:@selector(onConnect)])
                [eventHandler onConnect];
        }
    });
}

- (void) onDisconnect:(int) e {
    dispatch_async(dispatch_get_main_queue(), ^ {
        for (id eventHandler in m_eventHandlerList) {
            if ([eventHandler respondsToSelector:@selector(onDisconnect:)])
                [eventHandler onDisconnect:e];
        }
    });
}

- (void) onLog:(NSString*) tag withLog:(NSString*) log {
    dispatch_async(dispatch_get_main_queue(), ^ {
        for (id eventHandler in m_eventHandlerList) {
            if ([eventHandler respondsToSelector:@selector(onLog:withLog:)])
                [eventHandler onLog:tag withLog:log];
        }
    });
}

- (void) onRoster:(NSString*)jid withNickname:(NSString*) nickname withSubscription:(int) subscription withGroups:(NSString*) groups {
    dispatch_async(dispatch_get_main_queue(), ^ {
        for (id eventHandler in m_eventHandlerList) {
            if ([eventHandler respondsToSelector:@selector(onRoster:withNickname:withSubscription:withGroups:)])
                [eventHandler onRoster:jid withNickname:nickname withSubscription:subscription withGroups:groups];
        }
    });
}

- (void) onVCard:(NSString*)jid withNickname:(NSString*)nickname withPhoto:(NSString*)photo {
    dispatch_async(dispatch_get_main_queue(), ^ {
        for (id eventHandler in m_eventHandlerList) {
            if ([eventHandler respondsToSelector:@selector(onVCard:withNickname:withPhoto:)])
                [eventHandler onVCard:jid withNickname:nickname withPhoto:photo];
        }
    });
}

- (void) onPresence:(NSString*) rosterJid withNickname:(NSString*) rosterNickname withSubscription:(int) rosterSubscription withResource:(NSString*) resource withPresence:(int)presenceType withMsg:(NSString*) msg {
    dispatch_async(dispatch_get_main_queue(), ^ {
        for (id eventHandler in m_eventHandlerList) {
            if ([eventHandler respondsToSelector:@selector(onPresence:withNickname:withSubscription:withResource:withPresence:withMsg:)])
                [eventHandler onPresence:rosterJid withNickname:rosterNickname withSubscription:rosterSubscription withResource:resource withPresence:presenceType withMsg:msg];
        }
    });
    
}

- (void) onSubscriptionRequest:(NSString*)jid withMsg:(NSString*)msg {
    dispatch_async(dispatch_get_main_queue(), ^ {
        for (id eventHandler in m_eventHandlerList) {
            if ([eventHandler respondsToSelector:@selector(onSubscriptionRequest:withMsg:)])
                [eventHandler onSubscriptionRequest:jid withMsg:msg];
        }
    });
}

- (void) onUnsubscriptionRequest:(NSString*)jid withMsg:(NSString*)msg {
    dispatch_async(dispatch_get_main_queue(), ^ {
        for (id eventHandler in m_eventHandlerList) {
            if ([eventHandler respondsToSelector:@selector(onUnsubscriptionRequest:withMsg:)])
                [eventHandler onUnsubscriptionRequest:jid withMsg:msg];
        }
    });
}

- (void) onItemSubscribed:(NSString*)jid {
    dispatch_async(dispatch_get_main_queue(), ^ {
        for (id eventHandler in m_eventHandlerList) {
            if ([eventHandler respondsToSelector:@selector(onItemSubscribed:)])
                [eventHandler onItemSubscribed:jid];
        }
    });
}

- (void) onItemAdded:(NSString*)jid {
    dispatch_async(dispatch_get_main_queue(), ^ {
        for (id eventHandler in m_eventHandlerList) {
            if ([eventHandler respondsToSelector:@selector(onItemAdded:)])
                [eventHandler onItemAdded:jid];
        }
    });
}

- (void) onItemUnsubscribed:(NSString*)jid {
    dispatch_async(dispatch_get_main_queue(), ^ {
        for (id eventHandler in m_eventHandlerList) {
            if ([eventHandler respondsToSelector:@selector(onItemUnsubscribed:)])
                [eventHandler onItemUnsubscribed:jid];
        }
    });
    
}

- (void) onItemRemoved:(NSString*)jid {
    dispatch_async(dispatch_get_main_queue(), ^ {
        for (id eventHandler in m_eventHandlerList) {
            if ([eventHandler respondsToSelector:@selector(onItemRemoved:)])
                [eventHandler onItemRemoved:jid];
        }
    });
    
}

- (void) onItemUpdated:(NSString*)jid {
    dispatch_async(dispatch_get_main_queue(), ^ {
        for (id eventHandler in m_eventHandlerList) {
            if ([eventHandler respondsToSelector:@selector(onItemUpdated:)])
                [eventHandler onItemUpdated:jid];
        }
    });
    
}

- (void) onMessage:(NSString*)jid withMsgType:(int) msgType withSubject:(NSString*) subject withBody:(NSString*) body withThread:(NSString*) thread withSession:(void*) session {
    dispatch_async(dispatch_get_main_queue(), ^ {
        for (id eventHandler in m_eventHandlerList) {
            if ([eventHandler respondsToSelector:@selector(onMessage:withMsgType:withSubject:withBody:withThread:withSession:)])
                [eventHandler onMessage:jid withMsgType:msgType withSubject:subject withBody:body withThread:thread withSession:session];
        }
    });
}

#pragma mark API for XMPP
- (bool)connect:(NSString*)jid withPwd:(NSString*) pwd withHost:(NSString*) host withPort:(int) port {
    
    return m_glooxInterface->connect([jid UTF8String], [pwd UTF8String], [host UTF8String], port);
}

- (void)disConnect {
    m_glooxInterface->disConnect();
}

- (void)fetchVCard:(NSString*) jid {
    m_glooxInterface->fetchVCard([jid UTF8String]);
    
}

- (void)setPresence:(int) presenceType withPriority:(int) priority withStatus:(NSString*) status {
    m_glooxInterface->setPresence(presenceType, priority, [status UTF8String]);
}

- (void)subscribe:(NSString*) jid withName:(NSString*) name withGroups:(NSString*) groups withMsg:(NSString*) msg {
    m_glooxInterface->subscribe([jid UTF8String], [name UTF8String], [groups UTF8String], [msg UTF8String]);
}

- (void)unsubscribe:(NSString*) jid withMsg:(NSString*) msg {
    m_glooxInterface->unsubscribe([jid UTF8String], [msg UTF8String]);
}

- (void)getRosterItem:(NSString*) jid {
    m_glooxInterface->getRosterItem([jid UTF8String]);
}

- (void)ackSubscriptionRequest:(NSString*) jid withAck:(bool) ack {
    m_glooxInterface->ackSubscriptionRequest([jid UTF8String], ack);
}

- (void)removeRoster:(NSString*) jid {
    m_glooxInterface->removeRoster([jid UTF8String]);
}

- (void)sendMessage:(NSString*) jid withMsg:(NSString*) msg {
    m_glooxInterface->sendMessage([jid UTF8String], [msg UTF8String]);
}


@end
