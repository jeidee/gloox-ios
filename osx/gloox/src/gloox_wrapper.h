//
//  gloox_wrapper.h
//
//  Created by jeidee@gmail.com on 2016. 8. 26..
//  Copyright © 2016년 jeidee. All rights reserved.
//

#ifndef GLOOX_WRAPPER_H__
#define GLOOX_WRAPPER_H__

#include "client.h"
#include "messagesessionhandler.h"
#include "messageeventhandler.h"
#include "messageeventfilter.h"
#include "chatstatehandler.h"
#include "chatstatefilter.h"
#include "connectionlistener.h"
#include "disco.h"
#include "message.h"
#include "gloox.h"
#include "lastactivity.h"
#include "loghandler.h"
#include "logsink.h"
#include "connectiontcpclient.h"
#include "connectionbosh.h"
#include "connectionsocks5proxy.h"
#include "connectionhttpproxy.h"
#include "messagehandler.h"
#include "rostermanager.h"
#include "vcardhandler.h"
#include "vcardmanager.h"
#include "vcard.h"
#include "eventhandler.h"

#include <string>
#include <vector>
#include <map>
#include <list>
#include <sstream>
#include <mutex>
#include <thread>

#include "gloox_callback.h"

#ifdef __ANDROID_NDK__
#include <jni.h>
#endif

using namespace gloox;
using namespace std;

namespace jd {
    typedef struct {
        bool isSet;
        string jid;
        string pwd;
        string host;
        int port;
        
        void clear() {
            isSet = false;
            jid = "";
            pwd = "";
            host = "";
            port = 0;
        }
    } LoginInfo;
    
    class GlooxWrapper : public MessageSessionHandler
    , ConnectionListener
    , LogHandler
    , MessageEventHandler
    , MessageHandler
    , ChatStateHandler
    , RosterListener
    , VCardHandler
    , EventHandler
    {
        typedef map<string, MessageSession*> MessageSessionT;
        typedef MessageSessionT::iterator MessageSessionIt;
        
    public:
#ifdef __ANDROID_NDK__
        GlooxWrapper(JavaVM* jvm, JNIEnv* env, jobject obj);
#else
        GlooxWrapper();
#endif
        virtual ~GlooxWrapper();
        
        void setLoginInfo(const char* jid
                          , const char* pwd
                          , const char* host
                          , int port);
        const char* jid() { return m_loginInfo.jid.c_str(); }
        
        // for connection
        virtual bool connect();
        virtual bool disConnect();
        // for vcard
        virtual void fetchVCard(const char* jid);
        // for presence
        virtual void setPresence(int presenceType
                                 , int priority
                                 , const char* status);
        // for roster
        virtual void subscribe(const char* jid
                               , const char* name
                               , const char* groups
                               , const char* msg);
        virtual void unsubscribe(const char* jid, const char* msg);
        virtual RosterItem* getRosterItem(const char* jid);
        virtual void ackSubscriptionRequest(const char* jid, bool ack);
        virtual void remove(const char* jid);
        
        // for message
        virtual const char* sendMessage(const char* jid, const char* msg);
        
        virtual ConnectionError recv();
        virtual void heartBeat();
        void setNickname(const char* nickname);
        void setPhoto(const char* photo);
        
        // for recv thread
        static void recvWorker(GlooxWrapper* gw);
        bool startRecv();
        void stopRecv();
        
        // for callback interface
        void registerCallback(IGlooxCallback* callback) {
            m_callback = callback;
        }
        // for callback functions
        void registerOnConnect(callbackConnectFunc callback) {
            m_onConnect = callback;
        }
        void registerOnDisconnect(callbackDisconnectFunc callback) {
            m_onDisconnect = callback;
        }
        void registerOnLog(callbackLogFunc callback) {
            m_onLog = callback;
        }
        void registerOnRoster(callbackRosterFunc callback) {
            m_onRoster = callback;
        }
        void registerOnVCard(callbackVCardFunc callback) {
            m_onVCard = callback;
        }
        void registerOnPresence(callbackPresenceFunc callback) {
            m_onPresence = callback;
        }
        void registerOnSubscriptionRequest(callbackSubscriptionRequestFunc callback) {
            m_onSubscriptionRequest = callback;
        }
        void registerOnUnsubscriptionRequest(callbackUnsubscriptionRequestFunc callback) {
            m_onUnsubscriptionRequest = callback;
        }
        void registerOnItemSubscribed(callbackItemSubscribedFunc callback) {
            m_onItemSubscribed = callback;
        }
        void registerOnItemUnsubscribed(callbackItemUnsubscribedFunc callback) {
            m_onItemUnsubscribed = callback;
        }
        void registerOnItemAdded(callbackItemAddedFunc callback) {
            m_onItemAdded = callback;
        }
        void registerOnItemRemoved(callbackItemRemovedFunc callback) {
            m_onItemRemoved = callback;
        }
        void registerOnItemUpdated(callbackItemUpdatedFunc callback) {
            m_onItemUpdated = callback;
        }
        void registerOnMessage(callbackMessageFunc callback) {
            m_onMessage = callback;
        }
        
    protected:
        
        // ConnectionListener
        virtual void onConnect();
        virtual void onDisconnect(ConnectionError e);
        virtual bool onTLSConnect(const CertInfo& info);
        
        // EventHandler
        virtual void handleEvent(const Event& event);
        
        // MessageHandler
        virtual void handleMessage(const Message& msg, MessageSession* /*session*/);
        
        // MessageEnvetHandler
        virtual void handleMessageEvent(const JID& from, MessageEventType event);
        
        // ChatStateHandler
        virtual void handleChatState(const JID& from, ChatStateType state);
        
        // MessageSessionHandler
        virtual void handleMessageSession(MessageSession* session);
        
        // LogHandler
        virtual void handleLog(LogLevel level, LogArea area, const string& message);
        
        // RosterListener
        virtual void handleItemSubscribed(const JID& jid);
        virtual void handleItemAdded(const JID& jid);
        virtual void handleItemUnsubscribed(const JID& jid);
        virtual void handleItemRemoved(const JID& jid);
        virtual void handleItemUpdated(const JID& jid);
        virtual void handleRoster(const Roster& roster);
        virtual void handleRosterError(const IQ& /*iq*/);
        virtual void handleRosterPresence(const RosterItem& item
                                          , const std::string& resource
                                          , Presence::PresenceType presence
                                          , const std::string& /*msg*/);
        virtual void handleSelfPresence(const RosterItem& item
                                        , const std::string& resource
                                        , Presence::PresenceType presence
                                        , const std::string& /*msg*/);
        virtual bool handleSubscriptionRequest(const JID& jid
                                               , const std::string& /*msg*/);
        virtual bool handleUnsubscriptionRequest(const JID& jid
                                                 , const std::string& /*msg*/);
        virtual void handleNonrosterPresence(const Presence& presence);
        
        // VCardHandler
        virtual void handleVCard(const JID& jid, const VCard *v);
        virtual void handleVCardResult(VCardContext context, const JID& jid,
                                       StanzaError se = StanzaErrorUndefined);
        
        /*
         * callback functions
         */
        virtual void callbackConnect();
        virtual void callbackDisconnect(int e);
        virtual void callbackRoster(const Roster& roster);
        virtual void callbackVCard(const char* jid
                                   , const char* nickname
                                   , const char* photo);
        virtual void callbackPresence(const RosterItem& roster
                                      , const char* resource
                                      , Presence::PresenceType presence
                                      , const char* msg);
        virtual void callbackSubscriptionRequest(const char* jid
                                                 , const char* msg);
        virtual void callbackUnsubscriptionRequest(const char* jid
                                                   , const char* msg);
        virtual void callbackItemSubscribed(const char* jid);
        virtual void callbackItemAdded(const char* jid);
        virtual void callbackItemUnsubscribed(const char* jid);
        virtual void callbackItemRemoved(const char* jid);
        virtual void callbackItemUpdated(const char* jid);
        virtual void callbackMessage(const Message& msg, MessageSession* session);
    public:
        virtual void callbackLog(const char* tag, const char* log);
        
    private:
        
        /*
         * string utilities
         */
        static list<string> split(const char* source, char delimeter);
        static std::string format(const char* fmt, ...);
        static std::string format(const char* fmt, va_list args);
        
    private:
        
        
        //    MessageSession*     m_session;
        //    MessageEventFilter* m_messageEventFilter;
        //    ChatStateFilter*    m_chatStateFilter;
        MessageSessionT m_sessions;
        
        Client*             m_client;
        LoginInfo           m_loginInfo;
        VCardManager*       m_vcardManager;
        VCard*              m_vcard;
        std::mutex          m_lock;
        std::thread*		m_recvThread;
        bool				m_isRecvRunning;
        int                 m_heartBeat;
        
        // callback functions
        callbackConnectFunc     			m_onConnect;
        callbackDisconnectFunc  			m_onDisconnect;
        callbackLogFunc         			m_onLog;
        callbackRosterFunc      			m_onRoster;
        callbackVCardFunc       			m_onVCard;
        callbackPresenceFunc    			m_onPresence;
        callbackSubscriptionRequestFunc     m_onSubscriptionRequest;
        callbackUnsubscriptionRequestFunc   m_onUnsubscriptionRequest;
        callbackItemSubscribedFunc      	m_onItemSubscribed;
        callbackItemAddedFunc           	m_onItemAdded;
        callbackItemUnsubscribedFunc    	m_onItemUnsubscribed;
        callbackItemRemovedFunc 			m_onItemRemoved;
        callbackItemUpdatedFunc 			m_onItemUpdated;
        callbackMessageFunc     			m_onMessage;
        // callback interface
        IGlooxCallback* m_callback;
        
#ifdef __ANDROID_NDK__
        JavaVM*     m_jvm;
        JNIEnv*     m_env;
        jobject     m_obj;
        jmethodID   m_cbOnConnect;
#endif
    };  // class GlooxWrapper
};  // namespace jd

#endif  // GLOOX_WRAPPER_H__
