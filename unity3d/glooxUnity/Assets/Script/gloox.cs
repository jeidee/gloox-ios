using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System;
using AOT;
using System.Threading;
using System.Runtime.InteropServices;

public class gloox : MonoBehaviour {

	private delegate void callbackConnectFunc();
	private delegate void callbackDisconnectFunc(int e);
	private delegate void callbackLogFunc(string tag, string messge);

	private delegate void callbackRosterFunc(string jid
		, string nickname
		, int subscription
		, string groups
		, int no
		, int totalCount);
	private delegate void callbackVCardFunc(string jid
		, string nickname
		, string photo);
	private delegate void callbackPresenceFunc(string rosterJid
		, string rosterNickname
		, int rosterSubscription
		, string resource
		, int presence
		, string msg);
	private delegate void callbackSubscriptionRequestFunc(string jid
		, string msg);
	private delegate void callbackUnsubscriptionRequestFunc(string jid
		, string msg);
	private delegate void callbackItemSubscribedFunc(string jid);
	private delegate void callbackItemAddedFunc(string jid);
	private delegate void callbackItemUnsubscribedFunc(string jid);
	private delegate void callbackItemRemovedFunc(string jid);
	private delegate void callbackItemUpdatedFunc(string jid);
	private delegate void callbackMessageFunc(string jid
		, int msgType
		, string subject
		, string body
		, string Thread
		, IntPtr session);


#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern IntPtr gwCreateInstance();

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern void gwDeleteInstance(IntPtr glooxWrapper);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern void gwRegisterOnConnect(IntPtr glooxWrapper
		, callbackConnectFunc callback);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern void gwRegisterOnDisconnect(IntPtr glooxWrapper
		, callbackDisconnectFunc callback);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern void gwRegisterOnLog(IntPtr glooxWrapper
		, callbackLogFunc callback);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern void gwRegisterOnRoster(IntPtr glooxWrapper
		, callbackRosterFunc callback);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern void gwRegisterOnVCard(IntPtr glooxWrapper
		, callbackVCardFunc callback);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern void gwRegisterOnPresence(IntPtr glooxWrapper
		, callbackPresenceFunc callback);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern void gwRegisterOnSubscriptionRequest(IntPtr glooxWrapper
		, callbackSubscriptionRequestFunc callback);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern void gwRegisterOnUnsubscriptionRequest(IntPtr glooxWrapper
		, callbackUnsubscriptionRequestFunc callback);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern void gwRegisterOnItemSubscribed(IntPtr glooxWrapper
		, callbackItemSubscribedFunc callback);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern void gwRegisterOnItemAdded(IntPtr glooxWrapper
		, callbackItemAddedFunc callback);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern void gwRegisterOnItemUnsubscribed(IntPtr glooxWrapper
		, callbackItemUnsubscribedFunc callback);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern void gwRegisterOnItemRemoved(IntPtr glooxWrapper
		, callbackItemRemovedFunc callback);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern void gwRegisterOnItemUpdated(IntPtr glooxWrapper
		, callbackItemUpdatedFunc callback);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern void gwRegisterOnMessage(IntPtr glooxWrapper
		, callbackMessageFunc callback);		

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern bool gwConnect(IntPtr glooxWrapper
		, string jid
		, string pwd
		, string host
		, int port);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern bool gwDisconnect(IntPtr glooxWrapper);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern int gwRecv(IntPtr glooxWrapper);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern int gwFetchVCard(IntPtr glooxWrapper, string jid);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern int gwSetPresence(IntPtr glooxWrapper
		, int presenceType
		, int priority
		, string status);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern int gwSubscribe(IntPtr glooxWrapper
		, string jid
		, string name
		, string groups
		, string msg);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern int gwUnsubscribe(IntPtr glooxWrapper
		, string jid
		, string msg);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern int gwGetRosterItem(IntPtr glooxWrapper
		, string jid);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern int gwAckSubscriptionRequest(IntPtr glooxWrapper
		, string jid
		, bool ack);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern int gwRemoveRoster(IntPtr glooxWrapper
		, string jid);

#if UNITY_IOS && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("gloox")]
#endif
	private static extern int gwSendMessage(IntPtr glooxWrapper
		, string jid
		, string msg);

	IntPtr m_glooxWrapper;
	static bool m_connected = false;
	Thread m_recvThread;

	[MonoPInvokeCallback(typeof(callbackConnectFunc))]
	public static void onConnect() {
		Debug.Log("Connected!");
		m_connected = true;
	}

	[MonoPInvokeCallback(typeof(callbackDisconnectFunc))]
	public static void onDisconnect(int e) {
		Debug.Log("Disconnected. e = " + e.ToString());
		m_connected = false;
	}

	[MonoPInvokeCallback(typeof(callbackLogFunc))]
	public static void onLog(string tag, string log) {
		Debug.Log("onLog >> " +  tag + "::" + log);
	}

	[MonoPInvokeCallback(typeof(callbackRosterFunc))]
	public static void onRoster(string jid
		, string nickname
		, int subscription
		, string groups
		, int no
		, int totalCount) {
		Debug.Log("onRoster >> " + jid
			+ ", " + nickname
			+ ", subscription=" + subscription.ToString());
	}

	[MonoPInvokeCallback(typeof(callbackVCardFunc))]
	public static void onVCard(string jid
		, string nickname
		, string photo) {
		Debug.Log("onVCard >> " + jid
			+ ", " + nickname
			+ ", " + photo);
	}

	[MonoPInvokeCallback(typeof(callbackPresenceFunc))]
	public static void onPresence(string rosterJid
		, string rosterNickname
		, int rosterSubscription
		, string resource
		, int presence
		, string msg) {
		Debug.Log("onPresence >> " + rosterJid
			+ ", nick = " + rosterNickname
			+ ", subscription = " + rosterSubscription.ToString()
			+ ", resource = " + resource
			+ ", presence = " + presence.ToString()
			+ ", msg = " + msg);
	}
	
	[MonoPInvokeCallback(typeof(callbackSubscriptionRequestFunc))]
	public static void onSubscriptionRequest(string jid
		, string msg) {
		Debug.Log("onSubscriptionRequest >> " + jid
			+ ", msg = " + msg);
	}

	[MonoPInvokeCallback(typeof(callbackUnsubscriptionRequestFunc))]
	public static void onUnsubscriptionRequest(string jid
		, string msg) {
		Debug.Log("onUnsubscriptionRequest >> " + jid
			+ ", msg = " + msg);
	}

	[MonoPInvokeCallback(typeof(callbackItemSubscribedFunc))]
	public static void onItemSubscribed(string jid) {
		Debug.Log("onItemSubscribed >> " + jid);
	}
	
	[MonoPInvokeCallback(typeof(callbackItemUnsubscribedFunc))]
	public static void onItemUnsubscribed(string jid) {
		Debug.Log("onItemUnsubscribed >> " + jid);
	}

	[MonoPInvokeCallback(typeof(callbackItemAddedFunc))]
	public static void onItemAdded(string jid) {
		Debug.Log("onItemAdded >> " + jid);
	}

	[MonoPInvokeCallback(typeof(callbackItemRemovedFunc))]
	public static void onItemRemoved(string jid) {
		Debug.Log("onItemRemoved >> " + jid);
	}
	
	[MonoPInvokeCallback(typeof(callbackItemUpdatedFunc))]
	public static void onItemUpdated(string jid) {
		Debug.Log("onItemUpdated >> " + jid);
	}

	[MonoPInvokeCallback(typeof(callbackMessageFunc))]
	public static void onMessage(string jid
		, int msgType
		, string subject
		, string body
		, string thread
		, IntPtr session) {
		Debug.Log("onMessage >> " + jid
			+ ", msgType = " + msgType.ToString()
			+ ", subject = " + subject
			+ ", body = " + body
			+ ", thread = " + thread
			+ ", session = " + session.ToString());
	}
	
여기작업중
	// Use this for initialization
	void Start () {
		m_glooxWrapper = gwCreateInstance();

		gwRegisterOnLog(m_glooxWrapper, onLog);
		gwRegisterOnConnect(m_glooxWrapper, onConnect);
	}

	// Update is called once per frame
	void Update () {
	}

	void OnDestroy() {
		gwDeleteInstance(m_glooxWrapper);
	}


	void Recv() {
		while(true) {
			// gwRecv() block until receive a packet
			int e = gwRecv(m_glooxWrapper);
			if (e != 0) {
				Debug.Log("Recv failed");
				break;
			}
		}
	}

	public bool Connect(string jid, string pwd, string server, int port) {
		if (m_connected) {
			Debug.Log("Already connected.");
			return false;
		}

		if (!gwConnect(m_glooxWrapper, jid, pwd, server, port)) {
			Debug.Log("Connection failed");
			return false;
		}

		m_recvThread = new Thread(Recv);
		m_recvThread.Start();

		return true;
	}

	public void Disconnect() {
		gwDis
	}
}
