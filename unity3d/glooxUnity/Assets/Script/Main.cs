using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Main : MonoBehaviour {

	private Gloox m_gloox;

	// Use this for initialization
	void Start () {
		m_gloox = Gloox.Instance;

		m_gloox.ConnectHandler += new Gloox.callbackConnectFunc(onConnect);
		m_gloox.LogHandler += new Gloox.callbackLogFunc(onLog);
		m_gloox.DisconnectHandler += new Gloox.callbackDisconnectFunc(onDisconnect);
	}

	// Update is called once per frame
	void Update () {
	}

	void OnDestroy() {

	}

	void onConnect() {
		Debug.Log("connect......>");
	}

	void onDisconnect(int e) {
		Debug.Log("disconnect....");
	}

	void onLog(string tag, string log) {
		Debug.Log("log......>");
		Debug.Log(tag);
		Debug.Log(log);
	}

	void OnGUI() {
		if (GUI.Button(new Rect(10, 10, 100, 100), "Connect")) {
			if (!m_gloox.Connect("test1@bypass", "1234", "msg.iam0.com", 5223)) {
				Debug.Log("Connect failed");
			}
		}
	}
}
