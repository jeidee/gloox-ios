//
//  gloox_wrapper.cpp
//
//  Created by jeidee@gmail.com on 2016. 8. 26..
//  Copyright © 2016년 jeidee. All rights reserved.
//

#include "gloox_wrapper.h"

#include <mutex>
#include <cstring>
#include <cstdarg>

#ifdef __ANDROID_NDK__
#include "jni_util.h"
#include <android/log.h>

#define  LOG_TAG    "jd::GlooxWrapper"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

#else

#define  LOG_TAG    "jd::GlooxWrapper"
#define  LOGE(...)  callbackLog(LOG_TAG"::error", GlooxWrapper::format(__VA_ARGS__).c_str())
#define  LOGW(...)  callbackLog(LOG_TAG"::warn", GlooxWrapper::format(__VA_ARGS__).c_str())
#define  LOGD(...)  callbackLog(LOG_TAG"::debug", GlooxWrapper::format(__VA_ARGS__).c_str())
#define  LOGI(...)  callbackLog(LOG_TAG"::info", GlooxWrapper::format(__VA_ARGS__).c_str())


#endif


namespace jd {
    
#ifdef __ANDROID_NDK__
    GlooxWrapper::GlooxWrapper(JavaVM* jvm, JNIEnv* env, jobject obj)
    : m_jvm(jvm)
    , m_env(env)
    , m_obj(obj)
    , m_client(NULL)
    , m_vcardManager(NULL)
    , m_vcard(NULL)
    , m_heartBeat(0) {
#else
        GlooxWrapper::GlooxWrapper()
        : m_vcardManager(NULL)
        , m_onConnect(NULL)
        , m_onLog(NULL)
        , m_onDisconnect(NULL)
        , m_onRoster(NULL)
        , m_onVCard(NULL)
        , m_onPresence(NULL)
        , m_onSubscriptionRequest(NULL)
        , m_onUnsubscriptionRequest(NULL)
        , m_onItemSubscribed(NULL)
        , m_onItemAdded(NULL)
        , m_onItemUnsubscribed(NULL)
        , m_onItemRemoved(NULL)
        , m_onItemUpdated(NULL)
        , m_onMessage(NULL)
        , m_callback(NULL)
        , m_recvThread(NULL)
        , m_isRecvRunning(false) {
#endif
            //         , m_session(NULL)
            //         , m_messageEventFilter(NULL)
            //         , m_chatStateFilter(NULL)
            
            m_loginInfo.clear();
        }
        
        GlooxWrapper::~GlooxWrapper() {
            stopRecv();
            
            if (m_vcardManager) {
                delete m_vcardManager;
                m_vcardManager = NULL;
            }
            
            // if (m_client) {
            // 	delete m_client;
            // 	m_client = NULL;
            // }
        }
        
        //
        // Networking methods
        //
        
        void GlooxWrapper::setLoginInfo(const char* jid
                                        , const char* pwd
                                        , const char* host
                                        , int port) {
            m_loginInfo.isSet = true;
            m_loginInfo.jid = jid;
            m_loginInfo.pwd = pwd;
            m_loginInfo.host = host;
            m_loginInfo.port = port;
        }
        
        bool GlooxWrapper::connect() {
            if (m_loginInfo.isSet == false) {
                LOGE("m_loginInfo.isSet == false.");
                return false;
            }
            
            LOGD("jid = %s, pwd = %s, host = %s, port = %d"
                 , m_loginInfo.jid.c_str()
                 , m_loginInfo.pwd.c_str()
                 , m_loginInfo.host.c_str()
                 , m_loginInfo.port);
            
            JID j(m_loginInfo.jid.c_str());
            m_client = new Client(j, m_loginInfo.pwd.c_str());
            
            ConnectionTCPClient* conn =
            new ConnectionTCPClient( m_client, m_client->logInstance()
                                    , m_loginInfo.host.c_str()
                                    , m_loginInfo.port);
            
            m_client->setConnectionImpl(conn);
            
            m_client->registerConnectionListener(this);
            m_client->registerMessageSessionHandler(this, 0);
            // registerRosterListener의 syncSubscribeReq를 false로 등록할 경우
            // 비동기로 구독/취소 응답을 처리해야 한다.
            m_client->rosterManager()->registerRosterListener(this, false);
            
            m_client->disco()->setVersion("jd", GLOOX_VERSION, "android");
            m_client->disco()->setIdentity("client", "jd");
            m_client->disco()->addFeature(XMLNS_CHAT_STATES);
            
            m_client->logInstance().registerLogHandler(LogLevelDebug, LogAreaAll, this);
            
            m_vcardManager = new VCardManager(m_client);
            
            return m_client->connect(false);
        }
        
        bool GlooxWrapper::disConnect() {
            m_client->disconnect();
            stopRecv();
            return true;
        }
        
        const char* GlooxWrapper::sendMessage(const char* jid, const char* msg) {
            JID j(jid);
            MessageSession* newSession = m_sessions[j.bare()];
            if (newSession == NULL) {
                newSession = new MessageSession(m_client, j);
                newSession->registerMessageHandler( this );
                m_sessions[j.bare()] = newSession;
            }
            
            if (newSession == NULL) {
                LOGD("sendMessage to %s failed. newSession is null.", jid);
                return "";
            }
            newSession->send(msg);
            return msg;
        }
        
        void GlooxWrapper::heartBeat() {
            m_client->xmppPing(m_client->jid(), this);
            if (++m_heartBeat > 3) {
                LOGD("허트비트 무응답 초과로 연결 종료!");
                m_client->disconnect();
            }
        }
        
        void GlooxWrapper::setNickname(const char* nickname) {
            if (m_vcard == NULL || m_vcardManager == NULL) {
                return;
            }
            
            m_vcard->setNickname(nickname);
            m_vcardManager->storeVCard(new VCard(*m_vcard), this);
        }
        
        void GlooxWrapper::setPhoto(const char* photo) {
            if (m_vcard == NULL || m_vcardManager == NULL) {
                return;
            }
            
            m_vcard->setPhoto(photo);
            m_vcardManager->storeVCard(new VCard(*m_vcard), this);
        }
        
        
        //
        // XMPP Methods using gloox
        //
        
        void GlooxWrapper::fetchVCard(const char* jid) {
            JID j(jid);
            m_vcardManager->fetchVCard(string(jid), this);
        }
        
        void GlooxWrapper::setPresence(int presenceType
                                       , int priority
                                       , const char* status) {
            m_client->presence().addStatus(status);
            m_client->presence().setPriority(priority);
            m_client->presence().setPresence((gloox::Presence::PresenceType)presenceType);
            m_client->setPresence();
        }
        
        void GlooxWrapper::subscribe(const char* jid
                                     , const char* name
                                     , const char* groups
                                     , const char* msg) {
            list<string> groupList = split(groups, ',');
            
            m_client->rosterManager()->subscribe(string(jid), name, groupList, msg);
        }
        
        void GlooxWrapper::unsubscribe(const char* jid, const char* msg) {
            m_client->rosterManager()->unsubscribe(string(jid), msg);
        }
        
        RosterItem* GlooxWrapper::getRosterItem(const char* jid) {
            JID j(jid);
            RosterItem* rosterItem = m_client->rosterManager()->getRosterItem(j);
            return rosterItem;
        }
        
        void GlooxWrapper::ackSubscriptionRequest(const char* jid, bool ack) {
            JID j(jid);
            m_client->rosterManager()->ackSubscriptionRequest(j, ack);
        }
        
        void GlooxWrapper::remove(const char* jid) {
            JID j(jid);
            m_client->rosterManager()->remove(j);
        }
        
        //
        // Receive thread
        //
        
        ConnectionError GlooxWrapper::recv() {
            ConnectionError e = m_client->recv();
            return e;
        }
        
        void GlooxWrapper::recvWorker(GlooxWrapper* gw) {
            gw->m_isRecvRunning = true;
            while(true) {
                if (!gw->m_isRecvRunning) break;
                
                gw->recv();
            }
        }
        
        bool GlooxWrapper::startRecv() {
            if (m_recvThread != NULL) return false;
            
            m_recvThread = new std::thread(&GlooxWrapper::recvWorker, this);
            return true;
        }
        
        void GlooxWrapper::stopRecv() {
            
            if (m_recvThread != NULL && m_isRecvRunning) {
                m_isRecvRunning = false;
                disConnect();
                m_recvThread->join();
                
                delete m_recvThread;
                m_recvThread = NULL;
            }
        }
        
        //
        // Gloox ConnectionListeners
        //
        
        void GlooxWrapper::onConnect() {
            LOGD("connected!!!");
            
            m_heartBeat = 0;
            
            callbackConnect();
            
            //    m_env->CallVoidMethod(m_obj, m_cbOnConnect, m_env->NewStringUTF("[C->J] onTest recv called"));
            //
            //
            //    static jmethodID cb = NULL;
            //    jclass cls = m_env->GetObjectClass(m_obj);
            //
            //    if (cb == NULL) {
            //        cb = m_env->GetMethodID(cls, "onConnect", "()V");
            //    }
            //
            //    m_env->CallVoidMethod(m_obj, cb);
            //
#ifdef __ANDROID_NDK__
            m_env = getJNIEnv(m_jvm);
#endif
            //    callbackJavaMethod(m_env, m_obj, "onConnect", "()V");
        }
        
        void GlooxWrapper::onDisconnect(ConnectionError e) {
            LOGD("disconnected: %d", e);
            
            if (e == ConnAuthenticationFailed) {
                LOGE("auth failed. reason: %d", m_client->authError());
            }
            
            // 세션을 모두 초기화
            for (MessageSessionIt it = m_sessions.begin(); it != m_sessions.end(); ++it) {
                MessageSession* session = it->second;
                delete session;
            }
            
            m_sessions.clear();
            
            callbackDisconnect((int)e);
        }
        
        bool GlooxWrapper::onTLSConnect(const CertInfo& info) {
            time_t from(info.date_from);
            time_t to(info.date_to);
            
#if WIN32
            char fromBuff[512] = { 0, }, toBuff[512] = { 0, };
            ctime_s(fromBuff, 512, &from);
            ctime_s(toBuff, 512, &to);
            LOGD("status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n"
                 "from: %s\nto: %s\n",
                 info.status, info.issuer.c_str(), info.server.c_str(),
                 info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str(),
                 info.compression.c_str(), fromBuff, toBuff);
#else
            LOGD( "status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n"
                 "from: %s\nto: %s\n",
                 info.status, info.issuer.c_str(), info.server.c_str(),
                 info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str(),
                 info.compression.c_str(), ctime( &from ), ctime( &to ) );
#endif
            
            return true;
        }
        
        //
        // Gloox handlers
        //
        
        void GlooxWrapper::handleEvent(const Event& event)
        {
            switch (event.eventType())
            {
                case Event::PingPing:
                    LOGD("PingPing");
                    break;
                case Event::PingPong:
                    LOGD("PingPong");
                    --m_heartBeat;
                    break;
                case Event::PingError:
                    LOGE("PingError");
                    break;
                default:
                    break;
            }
            return;
        }
        
        // MessageHandler
        void GlooxWrapper::handleMessage(const Message& msg, MessageSession* session) {
            LOGD( "id: %s, type: %d, subject: %s, message: %s, thread id: %s\n"
                 , msg.id().c_str()
                 , msg.subtype()
                 , msg.subject().c_str()
                 , msg.body().c_str()
                 , msg.thread().c_str() );
            
            callbackMessage(msg, session);
            
            //        string re = "You said:\n> " + msg.body() + "\nI like that statement.";
            //        string sub;
            //        if( !msg.subject().empty() )
            //            sub = "Re: " +  msg.subject();
            //
            //        m_messageEventFilter->raiseMessageEvent( MessageEventDisplayed );
            //        sleep( 1 );
            //        m_messageEventFilter->raiseMessageEvent( MessageEventComposing );
            //        m_chatStateFilter->setChatState( ChatStateComposing );
            //        sleep( 2 );
            //        m_session->send( re, sub );
            //
            //        if( msg.body() == "quit" )
            //            j->disconnect();
        }
        
        // MessageEnvetHandler
        void GlooxWrapper::handleMessageEvent(const JID& from, MessageEventType event) {
            LOGD("received event: %d from: %s\n", event, from.full().c_str());
        }
        
        // ChatStateHandler
        void GlooxWrapper::handleChatState(const JID& from, ChatStateType state) {
            LOGD("received state: %d from: %s\n", state, from.full().c_str() );
        }
        
        // MessageSessionHandler
        void GlooxWrapper::handleMessageSession(MessageSession* session) {
            LOGD("got new session\n");
            session->registerMessageHandler(this);
            m_sessions[session->target().bare()] = session;
            
            // this example can handle only one session. so we get rid of the old session
            //    m_client->disposeMessageSession(m_session);
            //    m_session = session;
            //    m_session->registerMessageHandler(this);
            //    m_messageEventFilter = new MessageEventFilter(m_session);
            //    m_messageEventFilter->registerMessageEventHandler(this);
            //    m_chatStateFilter = new ChatStateFilter(m_session);
            //    m_chatStateFilter->registerChatStateHandler(this);
        }
        
        // LogHandler
        void GlooxWrapper::handleLog(LogLevel level, LogArea area, const string& message) {
            LOGD("log: level: %d, area: %d, %s\n", level, area, message.c_str());
        }
        
        void GlooxWrapper::handleItemSubscribed( const JID& jid )
        {
            LOGD( "subscribed %s\n", jid.bare().c_str() );
            callbackItemSubscribed(jid.bare().c_str());
        }
        
        void GlooxWrapper::handleItemAdded( const JID& jid )
        {
            LOGD( "added %s\n", jid.bare().c_str() );
            callbackItemAdded(jid.bare().c_str());
        }
        
        void GlooxWrapper::handleItemUnsubscribed( const JID& jid )
        {
            LOGD( "unsubscribed %s\n", jid.bare().c_str() );
            callbackItemUnsubscribed(jid.bare().c_str());
        }
        
        void GlooxWrapper::handleItemRemoved( const JID& jid )
        {
            LOGD( "removed %s\n", jid.bare().c_str() );
            callbackItemRemoved(jid.bare().c_str());
        }
        
        void GlooxWrapper::handleItemUpdated( const JID& jid )
        {
            LOGD( "updated %s\n", jid.bare().c_str() );
            callbackItemUpdated(jid.bare().c_str());
        }
        
        void GlooxWrapper::handleRoster( const Roster& roster )
        {
            LOGD( "roster arriving\nitems:\n" );
            //        Roster::const_iterator it = roster.begin();
            //        for( ; it != roster.end(); ++it )
            //        {
            //            LOGD( "jid: %s, name: %s, subscription: %d\n",
            //                 (*it).second->jid().c_str(), (*it).second->name().c_str(),
            //                 (*it).second->subscription() );
            //            StringList g = (*it).second->groups();
            //            StringList::const_iterator it_g = g.begin();
            //            for( ; it_g != g.end(); ++it_g )
            //                LOGD( "\tgroup: %s\n", (*it_g).c_str() );
            //            RosterItem::ResourceMap::const_iterator rit = (*it).second->resources().begin();
            //            for( ; rit != (*it).second->resources().end(); ++rit )
            //                LOGD( "resource: %s\n", (*rit).first.c_str() );
            //        }
            
            callbackRoster(roster);
        }
        
        void GlooxWrapper::handleRosterError( const IQ& /*iq*/ )
        {
            LOGD( "a roster-related error occured\n" );
        }
        
        void GlooxWrapper::handleRosterPresence(const RosterItem& item
                                                , const std::string& resource
                                                , Presence::PresenceType presence
                                                , const std::string& msg)
        {
            LOGD( "presence received: %s/%s -- %d\n"
                 , item.jidJID().full().c_str()
                 , resource.c_str()
                 , presence );
            callbackPresence(item, resource.c_str(), presence, msg.c_str());
        }
        
        void GlooxWrapper::handleSelfPresence(const RosterItem& item
                                              , const std::string& resource
                                              , Presence::PresenceType presence
                                              , const std::string& msg)
        {
            LOGD( "self presence received: %s/%s -- %d\n"
                 , item.jidJID().full().c_str()
                 , resource.c_str()
                 , presence );
            callbackPresence(item, resource.c_str(), presence, msg.c_str());
        }
        
        // syncSubscibeReq == true일 경우,
        // 구독/취소 요청이 오면 즉시 응답을 위해 콜백되는 함수
        // return true(수락), false(거절)
        // 비동기로 처리하고자 한다면 ackSubscriptionRequest()와 remove()함수를 사용할 것
        // 쌍방향 구독이 이뤄지려면, ackSubscriptionRequest()에 true로 응답해도,
        // 나 역시 상대방에게 subscribe()로 요청해야 한다.
        bool GlooxWrapper::handleSubscriptionRequest(const JID& jid
                                                     , const std::string& msg)
        {
            LOGD( "subscription: %s\n", jid.bare().c_str() );
            callbackSubscriptionRequest(jid.bare().c_str(), msg.c_str());
            //  StringList groups;
            //  JID id( jid );
            //  j->rosterManager()->subscribe( id, "", groups, "" );
            return true;
        }
        
        bool GlooxWrapper::handleUnsubscriptionRequest(const JID& jid
                                                       , const std::string& msg)
        {
            LOGD( "unsubscription: %s\n", jid.bare().c_str() );
            callbackUnsubscriptionRequest(jid.bare().c_str(), msg.c_str());
            return true;
        }
        
        void GlooxWrapper::handleNonrosterPresence(const Presence& presence)
        {
            LOGD("received presence from entity not in the roster: %s\n"
                 , presence.from().full().c_str());
        }
        
        // VCardHandler
        
        
        void GlooxWrapper::handleVCard(const JID& jid, const VCard *v)
        {
            if(!v) {
                LOGD("empty vcard!\n");
                return;
            }
            
            if (m_vcard == NULL && jid.bare() == m_client->jid().bare()) {
                m_lock.lock();
                //            if (m_vcard != NULL) {
                //                delete m_vcard;
                //                m_vcard = NULL;
                //            }
                
                m_vcard = new VCard(*v);
                m_lock.unlock();
            }
            
            //    VCard* vcard = new VCard(*v);
            
            LOGD("received vcard for %s: %s\n"
                 , jid.full().c_str()
                 , v->tag()->xml().c_str());
            callbackVCard(jid.full().c_str()
                          , v->nickname().c_str()
                          , v->photo().extval.c_str());
            
            //    for(VCard::AddressList::const_iterator it = vcard->addresses().begin(); it != vcard->addresses().end(); ++it) {
            //        LOGD("address: %s\n", (*it).street.c_str());
            //    }
        }
        
        void GlooxWrapper::handleVCardResult(VCardContext context
                                             , const JID& jid
                                             , StanzaError se) {
            LOGD("vcard result: context: %d, jid: %s, error: %d\n"
                 , context
                 , jid.full().c_str()
                 , se);
            m_vcardManager->fetchVCard(jid, this);
        }
        
        //
        // External callbacks. ex) jni callback functions...
        //
        
        void GlooxWrapper::callbackConnect(){
#ifdef __ANDROID_NDK__
            
            JniMethodInfo methodInfo;
            if (! getStaticMethodInfo(m_jvm, methodInfo
                                      , "com/smilegate/talk/message/GlooxWrapper"
                                      , "callbackConnect"
                                      , "()V"))
            {
                return;
            }
            
            //jstring stringArg = methodInfo.env->NewStringUTF(value);
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID
                                                 , methodInfo.methodID);
            //msg.methodInfo.env->DeleteLocalRef(stringArg);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
            if (m_callback != NULL) {
                m_callback->onConnect();
            }
            
            if (m_onConnect != NULL) {
                m_onConnect();
            }
        }
        
        void GlooxWrapper::callbackDisconnect(int e){
#ifdef __ANDROID_NDK__
            
            JniMethodInfo methodInfo;
            if (! getStaticMethodInfo(m_jvm, methodInfo
                                      , "com/smilegate/talk/message/GlooxWrapper"
                                      , "callbackDisconnect"
                                      , "(I)V"))
            {
                return;
            }
            
            //jstring stringArg = methodInfo.env->NewStringUTF(value);
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID
                                                 , methodInfo.methodID
                                                 , e);
            //msg.methodInfo.env->DeleteLocalRef(stringArg);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
            if (m_callback != NULL) {
                m_callback->onDisconnect(e);
            }
            
            if (m_onDisconnect != NULL) {
                m_onDisconnect(e);
            }
        }
        
        void GlooxWrapper::callbackRoster(const Roster& roster) {
#ifdef __ANDROID_NDK__
            
            JNIEnv *env = getJNIEnv(m_jvm);
            if (!env) {
                LOGE("env is null!");
                return;
            }
            
            jclass rosterCls = getClassID(env
                                          , "com/smilegate/talk/message/RosterArray");
            jmethodID rosterConstructor = env->GetMethodID(rosterCls, "<init>", "()V");
            jmethodID rosterAdd = env->GetMethodID(rosterCls
                                                   , "add"
                                                   , "(Ljava/lang/String;Ljava/lang/String;I)V");
            jobject rosterObj = env->NewObject(rosterCls, rosterConstructor);
            
            for (Roster::const_iterator it = roster.begin(); it != roster.end(); ++it) {
                jstring jid = env->NewStringUTF((*it).second->jid().c_str());
                jstring name = env->NewStringUTF((*it).second->name().c_str());
                int subscription = (int)(*it).second->subscription();
                
                env->CallVoidMethod(rosterObj, rosterAdd, jid, name, subscription);
                
                env->DeleteLocalRef(jid);
                env->DeleteLocalRef(name);
            }
            
            JniMethodInfo methodInfo;
            if (! getStaticMethodInfo(m_jvm, methodInfo
                                      , "com/smilegate/talk/message/GlooxWrapper"
                                      , "callbackRoster"
                                      , "(Lcom/smilegate/talk/message/RosterArray;)V"))
            {
                return;
            }
            
            //jstring stringArg = methodInfo.env->NewStringUTF(value);
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID
                                                 , methodInfo.methodID
                                                 , rosterObj);
            //msg.methodInfo.env->DeleteLocalRef(stringArg);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
            
            
            env->DeleteLocalRef(rosterCls);
            env->DeleteLocalRef(rosterObj);
#endif
            int totalCount = roster.size();
            int no = 0;
            for (Roster::const_iterator it = roster.begin(); it != roster.end(); ++it) {
                no += 1;
                
                const char* jid = (*it).second->jidJID().bare().c_str();
                const char* nickname = (*it).second->name().c_str();
                int subscription = (int)(*it).second->subscription();
                StringList groups = (*it).second->groups();
                std::string groupsTmp("");
                for(StringList::const_iterator itg = groups.begin()
                    ;itg != groups.end()
                    ; ++itg ) {
                    groupsTmp = (*itg) + ",";
                }
                
                if (m_callback != NULL) {
                    m_callback->onRoster(jid
                                         , nickname
                                         , subscription
                                         , groupsTmp.c_str()
                                         , no
                                         , totalCount);
                }
                
                if (m_onRoster != NULL) {
                    m_onRoster(jid, nickname
                               , subscription
                               , groupsTmp.c_str()
                               , no
                               , totalCount);
                }
            }
        }
        
        void GlooxWrapper::callbackVCard(const char* jid
                                         , const char* nickname
                                         , const char* photo) {
#ifdef __ANDROID_NDK__
            
            JniMethodInfo methodInfo;
            if (! getStaticMethodInfo(m_jvm
                                      , methodInfo
                                      , "com/smilegate/talk/message/GlooxWrapper"
                                      , "callbackVCard"
                                      , "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
            {
                return;
            }
            
            jstring jidStr = methodInfo.env->NewStringUTF(jid);
            jstring nicknameStr = methodInfo.env->NewStringUTF(nickname);
            jstring photoStr = methodInfo.env->NewStringUTF(photo);
            
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID
                                                 , methodInfo.methodID
                                                 , jidStr
                                                 , nicknameStr
                                                 , photoStr);
            
            methodInfo.env->DeleteLocalRef(jidStr);
            methodInfo.env->DeleteLocalRef(nicknameStr);
            methodInfo.env->DeleteLocalRef(photoStr);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
            if (m_callback != NULL) {
                m_callback->onVCard(jid, nickname, photo);
            }
            
            if (m_onVCard != NULL) {
                m_onVCard(jid, nickname, photo);
            }
        }
        
        void GlooxWrapper::callbackItemSubscribed(const char* jid) {
#ifdef __ANDROID_NDK__
            
            JniMethodInfo methodInfo;
            if (! getStaticMethodInfo(m_jvm, methodInfo
                                      , "com/smilegate/talk/message/GlooxWrapper"
                                      , "callbackRosterItemSubscribed"
                                      , "(Ljava/lang/String;)V"))
            {
                return;
            }
            
            jstring jidStr = methodInfo.env->NewStringUTF(jid);
            
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID
                                                 , methodInfo.methodID
                                                 , jidStr);
            
            methodInfo.env->DeleteLocalRef(jidStr);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
            if (m_callback != NULL) {
                m_callback->onItemSubscribed(jid);
            }
            
            if (m_onItemSubscribed != NULL) {
                m_onItemSubscribed(jid);
            }
        }
        
        void GlooxWrapper::callbackItemAdded(const char* jid) {
#ifdef __ANDROID_NDK__
            
            JniMethodInfo methodInfo;
            if (! getStaticMethodInfo(m_jvm, methodInfo
                                      , "com/smilegate/talk/message/GlooxWrapper"
                                      , "callbackRosterItemAdded"
                                      , "(Ljava/lang/String;)V"))
            {
                return;
            }
            
            jstring jidStr = methodInfo.env->NewStringUTF(jid);
            
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID
                                                 , methodInfo.methodID
                                                 , jidStr);
            
            methodInfo.env->DeleteLocalRef(jidStr);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
            if (m_callback != NULL) {
                m_callback->onItemAdded(jid);
            }
            
            if (m_onItemAdded != NULL) {
                m_onItemAdded(jid);
            }
        }
        
        void GlooxWrapper::callbackItemUnsubscribed(const char* jid) {
#ifdef __ANDROID_NDK__
            
            JniMethodInfo methodInfo;
            if (! getStaticMethodInfo(m_jvm
                                      , methodInfo
                                      , "com/smilegate/talk/message/GlooxWrapper"
                                      , "callbackRosterItemUnsubscribed"
                                      , "(Ljava/lang/String;)V"))
            {
                return;
            }
            
            jstring jidStr = methodInfo.env->NewStringUTF(jid);
            
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID
                                                 , methodInfo.methodID
                                                 , jidStr);
            
            methodInfo.env->DeleteLocalRef(jidStr);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
            if (m_callback != NULL) {
                m_callback->onItemUnsubscribed(jid);
            }
            
            if (m_onItemUnsubscribed != NULL) {
                m_onItemUnsubscribed(jid);
            }
        }
        
        void GlooxWrapper::callbackItemRemoved(const char* jid) {
#ifdef __ANDROID_NDK__
            
            JniMethodInfo methodInfo;
            if (! getStaticMethodInfo(m_jvm, methodInfo
                                      , "com/smilegate/talk/message/GlooxWrapper"
                                      , "callbackRosterItemRemoved"
                                      , "(Ljava/lang/String;)V"))
            {
                return;
            }
            
            jstring jidStr = methodInfo.env->NewStringUTF(jid);
            
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID
                                                 , methodInfo.methodID
                                                 , jidStr);
            
            methodInfo.env->DeleteLocalRef(jidStr);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
            if (m_callback != NULL) {
                m_callback->onItemRemoved(jid);
            }
            
            if (m_onItemRemoved != NULL) {
                m_onItemRemoved(jid);
            }
        }
        
        void GlooxWrapper::callbackItemUpdated(const char* jid) {
#ifdef __ANDROID_NDK__
            
            JniMethodInfo methodInfo;
            if (! getStaticMethodInfo(m_jvm, methodInfo
                                      , "com/smilegate/talk/message/GlooxWrapper"
                                      , "callbackRosterItemUpdated"
                                      , "(Ljava/lang/String;)V"))
            {
                return;
            }
            
            jstring jidStr = methodInfo.env->NewStringUTF(jid);
            
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID
                                                 , methodInfo.methodID
                                                 , jidStr);
            
            methodInfo.env->DeleteLocalRef(jidStr);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
            if (m_callback != NULL) {
                m_callback->onItemUpdated(jid);
            }
            
            if (m_onItemUpdated != NULL) {
                m_onItemUpdated(jid);
            }
        }
        
        void GlooxWrapper::callbackSubscriptionRequest(const char* jid
                                                       , const char* msg) {
#ifdef __ANDROID_NDK__
            
            JniMethodInfo methodInfo;
            if (! getStaticMethodInfo(m_jvm, methodInfo
                                      , "com/smilegate/talk/message/GlooxWrapper"
                                      , "callbackSubscriptionRequest"
                                      , "(Ljava/lang/String;Ljava/lang/String;)V"))
            {
                return;
            }
            
            jstring jidStr = methodInfo.env->NewStringUTF(jid);
            jstring msgStr = methodInfo.env->NewStringUTF(msg);
            
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID
                                                 , methodInfo.methodID
                                                 , jidStr
                                                 , msgStr);
            
            methodInfo.env->DeleteLocalRef(jidStr);
            methodInfo.env->DeleteLocalRef(msgStr);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
            if (m_callback != NULL) {
                m_callback->onSubscriptionRequest(jid, msg);
            }
            
            if (m_onSubscriptionRequest != NULL) {
                m_onSubscriptionRequest(jid, msg);
            }
        }
        
        void GlooxWrapper::callbackUnsubscriptionRequest(const char* jid, const char* msg) {
#ifdef __ANDROID_NDK__
            JniMethodInfo methodInfo;
            if (! getStaticMethodInfo(m_jvm, methodInfo
                                      , "com/smilegate/talk/message/GlooxWrapper"
                                      , "callbackUnsubscriptionRequest"
                                      , "(Ljava/lang/String;Ljava/lang/String;)V"))
            {
                return;
            }
            
            jstring jidStr = methodInfo.env->NewStringUTF(jid);
            jstring msgStr = methodInfo.env->NewStringUTF(msg);
            
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID
                                                 , methodInfo.methodID
                                                 , jidStr
                                                 , msgStr);
            
            methodInfo.env->DeleteLocalRef(jidStr);
            methodInfo.env->DeleteLocalRef(msgStr);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
            if (m_callback != NULL) {
                m_callback->onUnsubscriptionRequest(jid, msg);
            }
            
            if (m_onUnsubscriptionRequest != NULL) {
                m_onUnsubscriptionRequest(jid, msg);
            }
        }
        
        void GlooxWrapper::callbackPresence(const RosterItem& roster
                                            , const char* resource
                                            , Presence::PresenceType presence
                                            , const char* msg) {
#ifdef __ANDROID_NDK__
            
            JNIEnv *env = getJNIEnv(m_jvm);
            if (!env) {
                LOGE("env is null!");
                return;
            }
            
            // roster object
            jclass rosterCls = getClassID(env, "com/smilegate/talk/message/Roster");
            jmethodID rosterConstructor = env->GetMethodID(rosterCls
                                                           , "<init>"
                                                           , "(Ljava/lang/String;Ljava/lang/String;I)V");
            
            jstring jid = env->NewStringUTF(roster.jid().c_str());
            jstring name = env->NewStringUTF(roster.name().c_str());
            
            jobject rosterObj = env->NewObject(rosterCls
                                               , rosterConstructor
                                               , jid
                                               , name
                                               , roster.subscription());
            
            env->DeleteLocalRef(jid);
            env->DeleteLocalRef(name);
            
            JniMethodInfo methodInfo;
            if (! getStaticMethodInfo(m_jvm, methodInfo
                                      , "com/smilegate/talk/message/GlooxWrapper"
                                      , "callbackPresence"
                                      , "(Lcom/smilegate/talk/message/Roster;Ljava/lang/String;ILjava/lang/String;)V"))
            {
                return;
            }
            
            jstring resourceArg = env->NewStringUTF(resource);
            jstring msgArg = env->NewStringUTF(msg);
            
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID
                                                 , methodInfo.methodID
                                                 , rosterObj
                                                 , resourceArg
                                                 , (int)presence
                                                 , msgArg);
            
            methodInfo.env->DeleteLocalRef(resourceArg);
            methodInfo.env->DeleteLocalRef(msgArg);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
            
            env->DeleteLocalRef(rosterCls);
            env->DeleteLocalRef(rosterObj);
            
#endif
            if (m_callback != NULL) {
                const char* rosterJid = roster.jidJID().bare().c_str();
                const char* rosterNickname = roster.name().c_str();
                int rosterSubscription = (int)roster.subscription();
                m_callback->onPresence(rosterJid
                                       , rosterNickname
                                       , rosterSubscription
                                       , resource
                                       , (int)presence
                                       , msg);
            }
            
            if (m_onPresence != NULL) {
                const char* rosterJid = roster.jidJID().bare().c_str();
                const char* rosterNickname = roster.name().c_str();
                int rosterSubscription = (int)roster.subscription();
                m_onPresence(rosterJid
                             , rosterNickname
                             , rosterSubscription
                             , resource
                             , (int)presence
                             , msg);
            }
        }
        
        void GlooxWrapper::callbackMessage(const Message& msg, MessageSession* session) {
#ifdef __ANDROID_NDK__
            
            JniMethodInfo methodInfo;
            if (! getStaticMethodInfo(m_jvm
                                      , methodInfo
                                      , "com/smilegate/talk/message/GlooxWrapper"
                                      , "callbackMessage"
                                      , "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
            {
                return;
            }
            
            jstring jidStr = methodInfo.env->NewStringUTF(session->target().bare().c_str());
            jstring idStr = methodInfo.env->NewStringUTF(msg.id().c_str());
            jstring subStr = methodInfo.env->NewStringUTF(msg.subject().c_str());
            jstring bodyStr = methodInfo.env->NewStringUTF(msg.body().c_str());
            jstring threadStr = methodInfo.env->NewStringUTF(msg.thread().c_str());
            
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID
                                                 , methodInfo.methodID
                                                 , jidStr
                                                 , idStr
                                                 , msg.subtype()
                                                 , subStr
                                                 , bodyStr
                                                 , threadStr);
            
            methodInfo.env->DeleteLocalRef(jidStr);
            methodInfo.env->DeleteLocalRef(idStr);
            methodInfo.env->DeleteLocalRef(subStr);
            methodInfo.env->DeleteLocalRef(bodyStr);
            methodInfo.env->DeleteLocalRef(threadStr);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
#endif
            if (m_callback != NULL) {
                m_callback->onMessage(session->target().bare().c_str()
                                      , (int)msg.subtype()
                                      , msg.subject().c_str()
                                      , msg.body().c_str()
                                      , msg.thread().c_str()
                                      , session);
            }
            
            if (m_onMessage != NULL) {
                m_onMessage(session->target().bare().c_str()
                            , (int)msg.subtype()
                            , msg.subject().c_str()
                            , msg.body().c_str()
                            , msg.thread().c_str()
                            , session);
            }
        }
        
        void GlooxWrapper::callbackLog(const char *tag, const char *log) {
            if (m_callback != NULL) {
                m_callback->onLog(tag, log);
            }
            
            if (m_onLog != NULL) {
                m_onLog(tag, log);
            }
        }
        
        //
        // String utilities
        //
        
        list<string> GlooxWrapper::split(const char* source, char delimeter) {
            list<string> tokenList;
            stringstream ss(source);
            
            string token;
            while (getline(ss, token, delimeter)) {
                tokenList.push_back(token);
            }
            
            return tokenList;
        }
        
        std::string GlooxWrapper::format(const char *fmt, ...)
        {
            va_list args;
            
            va_start( args, fmt );
            
            std::string log = format(fmt, args);
            
            va_end( args );
            
            return log;
        }
        
        std::string GlooxWrapper::format(const char *fmt, va_list args)
        {
            if (!fmt) return "";
            int   result = -1, length = 1024;
            char *buffer = 0;
            while (result == -1)    {
                if (buffer)
                {
                    delete[] buffer;
                }
                
                buffer = new char[length + 1];
                memset(buffer, 0, length + 1);
                
                result = vsnprintf(buffer, length, fmt, args);
                
                length *= 2;
            }
            std::string s(buffer);
            delete [] buffer;
            return s;
        }
        
    };  // namespace jd
