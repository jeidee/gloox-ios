using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using AOT;
using System.Threading;
using System.Runtime.InteropServices;

public class Main : MonoBehaviour {

	IntPtr m_glooxWrapper;
	static bool m_connected = false;
	float m_tick = 0f;
	Thread m_recvThread;

	[MonoPInvokeCallback(typeof(callbackLogFunc))]
	public static void onLog(string tag, string log) {
		Debug.Log(tag + "::" + log);
	}

	[MonoPInvokeCallback(typeof(callbackConnectFunc))]
	public static void onConnect() {
		Debug.Log("Connected~");
		m_connected = true;
	}

	// Use this for initialization
	void Start () {
		m_glooxWrapper = gwCreateInstance();

		gwRegisterOnLog(m_glooxWrapper, onLog);
		gwRegisterOnConnect(m_glooxWrapper, onConnect);

		if (!gwConnect(m_glooxWrapper, "test1@bypass", "1234", "msg.iam0.com", 5223)) {
			Debug.Log("Connect failed");
		}
		// StartCoroutine(Connect());

		m_recvThread = new Thread(Recv);
		m_recvThread.Start();
	}

	void Recv() {
		while(true) {
			int e = gwRecv(m_glooxWrapper);
			if (e != 0) {
				Debug.Log("Recv failed");
				break;
			}

			Thread.Sleep(1);
		}
	}

	// IEnumerator Connect() {
	// 	if (!connect(mMsgClient, "test1@bypass", "1234", "msg.iam0.com", 5223)) {
	// 		Debug.Log("Connect failed");
	// 	}

	// 	while(true) {
	// 		int e = recv(mMsgClient);
	// 		if (e == 0 && !mConnected) {
	// 			yield return new WaitForSeconds(1);
	// 		}

	// 		if (e != 0) {
	// 			Debug.Log("Recv error..." + e.ToString());
	// 		}
	// 	}
	// }
	
	// Update is called once per frame
	void Update () {
	}

	void OnDestroy() {
		gwDeleteInstance(m_glooxWrapper);
	}
}
