using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System;
using AOT;
using System.Threading;
using System.Runtime.InteropServices;

public class Gloox : MonoBehaviour {

	// for singleton
	private static Gloox _instance;  
	public static Gloox Instance  
	{  
		get  
		{  
			if (!_instance)  
			{  
				_instance = GameObject.FindObjectOfType<Gloox>();  
				if (!_instance)  
				{  
						GameObject container = new GameObject();  
						container.name = "GlooxContainer";  
						_instance = container.AddComponent<Gloox>();  
				}  
			}  

			return _instance;  
		}  
	}

	public delegate void callbackConnectFunc();
	public delegate void callbackDisconnectFunc(int e);
	public delegate void callbackLogFunc(string tag, string messge);

	public delegate void callbackRosterFunc(string jid
		, string nickname
		, int subscription
		, string groups
		, int no
		, int totalCount);
	public delegate void callbackVCardFunc(string jid
		, string nickname
		, string photo);
	public delegate void callbackPresenceFunc(string rosterJid
		, string rosterNickname
		, int rosterSubscription
		, string resource
		, int presence
		, string msg);
	public delegate void callbackSubscriptionRequestFunc(string jid
		, string msg);
	public delegate void callbackUnsubscriptionRequestFunc(string jid
		, string msg);
	public delegate void callbackItemSubscribedFunc(string jid);
	public delegate void callbackItemAddedFunc(string jid);
	public delegate void callbackItemUnsubscribedFunc(string jid);
	public delegate void callbackItemRemovedFunc(string jid);
	public delegate void callbackItemUpdatedFunc(string jid);
	public delegate void callbackMessageFunc(string jid
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

	[MonoPInvokeCallback(typeof(callbackConnectFunc))]
	public void onConnect() {
		//Debug.Log("Connected!");
		Gloox.Instance.m_connected = true;

		if (Gloox.Instance.ConnectHandler != null) {
			Gloox.Instance.ConnectHandler();
		}
	}

	[MonoPInvokeCallback(typeof(callbackDisconnectFunc))]
	public void onDisconnect(int e) {
		//Debug.Log("Disconnected. e = " + e.ToString());
		m_connected = false;

		if (Gloox.Instance.DisconnectHandler != null) {
			Gloox.Instance.DisconnectHandler(e);
		}
	}

	[MonoPInvokeCallback(typeof(callbackLogFunc))]
	public void onLog(string tag, string log) {
		//Debug.Log("onLog >> " +  tag + "::" + log);

		if (Gloox.Instance.LogHandler != null) {
			Gloox.Instance.LogHandler(tag, log);
		}
	}

	[MonoPInvokeCallback(typeof(callbackRosterFunc))]
	public void onRoster(string jid
		, string nickname
		, int subscription
		, string groups
		, int no
		, int totalCount) {
		// Debug.Log("onRoster >> " + jid
		// 	+ ", " + nickname
		// 	+ ", subscription=" + subscription.ToString());
		if (Gloox.Instance.RosterHandler != null) {
			Gloox.Instance.RosterHandler(jid
				, nickname
				, subscription
				, groups
				, no
				, totalCount);
		}
	}

	[MonoPInvokeCallback(typeof(callbackVCardFunc))]
	public void onVCard(string jid
		, string nickname
		, string photo) {
		// Debug.Log("onVCard >> " + jid
		// 	+ ", " + nickname
		// 	+ ", " + photo);
		if (Gloox.Instance.VCardHandler != null) {
			Gloox.Instance.VCardHandler(jid, nickname, photo);
		}
	}

	[MonoPInvokeCallback(typeof(callbackPresenceFunc))]
	public void onPresence(string rosterJid
		, string rosterNickname
		, int rosterSubscription
		, string resource
		, int presence
		, string msg) {
		// Debug.Log("onPresence >> " + rosterJid
		// 	+ ", nick = " + rosterNickname
		// 	+ ", subscription = " + rosterSubscription.ToString()
		// 	+ ", resource = " + resource
		// 	+ ", presence = " + presence.ToString()
		// 	+ ", msg = " + msg);
		if (Gloox.Instance.PresenceHandler != null) {
			Gloox.Instance.PresenceHandler(rosterJid
				, rosterNickname
				, rosterSubscription
				, resource
				, presence
				, msg);
		}
	}
	
	[MonoPInvokeCallback(typeof(callbackSubscriptionRequestFunc))]
	public void onSubscriptionRequest(string jid
		, string msg) {
		// Debug.Log("onSubscriptionRequest >> " + jid
		// 	+ ", msg = " + msg);
		if (Gloox.Instance.SubscriptionRequestHandler != null) {
			Gloox.Instance.SubscriptionRequestHandler(jid, msg);
		}
	}

	[MonoPInvokeCallback(typeof(callbackUnsubscriptionRequestFunc))]
	public void onUnsubscriptionRequest(string jid
		, string msg) {
		// Debug.Log("onUnsubscriptionRequest >> " + jid
		// 	+ ", msg = " + msg);
		if (Gloox.Instance.UnsubscriptionRequestHandler != null) {
			Gloox.Instance.UnsubscriptionRequestHandler(jid, msg);
		}
	}

	[MonoPInvokeCallback(typeof(callbackItemSubscribedFunc))]
	public void onItemSubscribed(string jid) {
		// Debug.Log("onItemSubscribed >> " + jid);
		if (Gloox.Instance.ItemSubscribedHandler != null) {
			Gloox.Instance.ItemSubscribedHandler(jid);
		}
	}
	
	[MonoPInvokeCallback(typeof(callbackItemUnsubscribedFunc))]
	public void onItemUnsubscribed(string jid) {
		// Debug.Log("onItemUnsubscribed >> " + jid);
		if (Gloox.Instance.ItemUnsubscribedHandler != null) {
			Gloox.Instance.ItemUnsubscribedHandler(jid);
		}
	}

	[MonoPInvokeCallback(typeof(callbackItemAddedFunc))]
	public void onItemAdded(string jid) {
		// Debug.Log("onItemAdded >> " + jid);
		if (Gloox.Instance.ItemAddedHandler != null) {
			Gloox.Instance.ItemAddedHandler(jid);
		}
	}

	[MonoPInvokeCallback(typeof(callbackItemRemovedFunc))]
	public void onItemRemoved(string jid) {
		// Debug.Log("onItemRemoved >> " + jid);
		if (Gloox.Instance.ItemRemovedHandler != null) {
			Gloox.Instance.ItemRemovedHandler(jid);
		}
	}
	
	[MonoPInvokeCallback(typeof(callbackItemUpdatedFunc))]
	public void onItemUpdated(string jid) {
		// Debug.Log("onItemUpdated >> " + jid);
		if (Gloox.Instance.ItemUpdatedHandler != null) {
			Gloox.Instance.ItemUpdatedHandler(jid);
		}
	}

	[MonoPInvokeCallback(typeof(callbackMessageFunc))]
	public void onMessage(string jid
		, int msgType
		, string subject
		, string body
		, string thread
		, IntPtr session) {
		// Debug.Log("onMessage >> " + jid
		// 	+ ", msgType = " + msgType.ToString()
		// 	+ ", subject = " + subject
		// 	+ ", body = " + body
		// 	+ ", thread = " + thread
		// 	+ ", session = " + session.ToString());
		if (Gloox.Instance.MessageHandler != null) {
			Gloox.Instance.MessageHandler(jid
				, msgType
				, subject
				, body
				, thread
				, session);
		}
	}

	public event callbackConnectFunc ConnectHandler;
	public event callbackDisconnectFunc DisconnectHandler;
	public event callbackLogFunc LogHandler;
	public event callbackRosterFunc RosterHandler;
	public event callbackVCardFunc VCardHandler;
	public event callbackPresenceFunc PresenceHandler;
	public event callbackSubscriptionRequestFunc SubscriptionRequestHandler;
	public event callbackUnsubscriptionRequestFunc UnsubscriptionRequestHandler;
	public event callbackItemSubscribedFunc ItemSubscribedHandler;
	public event callbackItemAddedFunc ItemAddedHandler;
	public event callbackItemUnsubscribedFunc ItemUnsubscribedHandler;
	public event callbackItemRemovedFunc ItemRemovedHandler;
	public event callbackItemUpdatedFunc ItemUpdatedHandler;
	public event callbackMessageFunc MessageHandler;

	IntPtr m_glooxWrapper;
	Thread m_recvThread;
	public bool m_connected = false;

	// Use this for initialization
	void Start () {
		Debug.Log("Gloox Started!");

		m_glooxWrapper = gwCreateInstance();

		gwRegisterOnLog(m_glooxWrapper, onLog);
		gwRegisterOnConnect(m_glooxWrapper, onConnect);
	}

	// Update is called once per frame
	void Update () {
	}

	void OnDestroy() {
		Disconnect();
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
		if (!m_connected) {
			Debug.Log("Already disconnected.");
			return;
		}

		if (!gwDisconnect(m_glooxWrapper)) {
			Debug.Log("Disconnection failed");
			return;
		}
	}

	public void fetchVCard(string jid) {
		if (!m_connected) {
			Debug.Log("Connection closed");
			return;
		}

		gwFetchVCard(m_glooxWrapper, jid);
	}

	public void setPresence(int presenceType, int priority, string status) {
		if (!m_connected) {
			Debug.Log("Connection closed");
			return;
		}

		gwSetPresence(m_glooxWrapper, presenceType, priority, status);
	}

	public void subscribe(string jid, string name, string groups, string msg) {
		if (!m_connected) {
			Debug.Log("Connection closed");
			return;
		}

		gwSubscribe(m_glooxWrapper, jid, name, groups, msg);
	}

	public void unsubscribe(string jid, string msg) {
		if (!m_connected) {
			Debug.Log("Connection closed");
			return;
		}

		gwUnsubscribe(m_glooxWrapper, jid, msg);
	}

	public void getRosterItem(string jid) {
		if (!m_connected) {
			Debug.Log("Connection closed");
			return;
		}

		gwGetRosterItem(m_glooxWrapper, jid);
	}
	
	public void ackSubscriptionRequest(string jid, bool ack) {
		if (!m_connected) {
			Debug.Log("Connection closed");
			return;
		}

		gwAckSubscriptionRequest(m_glooxWrapper, jid, ack);
	}
	
	public void removeRoster(string jid) {
		if (!m_connected) {
			Debug.Log("Connection closed");
			return;
		}

		gwRemoveRoster(m_glooxWrapper, jid);
	}
	
	public void sendMessage(string jid, string msg) {
		if (!m_connected) {
			Debug.Log("Connection closed");
			return;
		}

		gwSendMessage(m_glooxWrapper, jid, msg);
	}
}
