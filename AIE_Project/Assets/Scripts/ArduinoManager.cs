using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using ArduinoBluetoothAPI;
using System;
using System.Text;

/*
아두이노 블루투스 연결 예제

게임 내 미사용, 통신 관리 기능은 GameManager.cs로 이전
*/

public class ArduinoManager : MonoBehaviour {

	#region Propertices & Variables

	// Use this for initialization
	BluetoothHelper bluetoothHelper;
	string deviceName;

	string received_message;

	#endregion


	#region Methods

	void ConnectBluetooth(){
		if(bluetoothHelper.isDevicePaired())
			bluetoothHelper.Connect (); // tries to connect
	}

	void DisconnectBluetooth(){
		bluetoothHelper.Disconnect ();
	}


	#endregion


	#region Monobehavior Callbacks

	void Start () {
		deviceName = "M"; //bluetooth should be turned ON;
		try
		{	
			bluetoothHelper = BluetoothHelper.GetInstance(deviceName);
			bluetoothHelper.OnConnected += OnConnected;
			bluetoothHelper.OnConnectionFailed += OnConnectionFailed;
			bluetoothHelper.OnDataReceived += OnMessageReceived; //read the data
			bluetoothHelper.setTerminatorBasedStream(";"); //delimits received messages based on ; char
			
            LinkedList<BluetoothDevice> ds = bluetoothHelper.getPairedDevicesList();

            foreach(BluetoothDevice d in ds)
            {
                Debug.Log($"{d.DeviceName} {d.DeviceAddress}");
            }
		}
		catch (Exception ex) 
		{
			Debug.Log (ex.Message);
		}
	}
	
	// Update is called once per frame
	void Update () {

	}

	//Asynchronous method to receive messages
	void OnMessageReceived(BluetoothHelper helper)
	{
        received_message = helper.Read();
        Debug.Log(received_message);
	}

	void OnConnected(BluetoothHelper helper)
	{
		try{
			helper.StartListening();
		}catch(Exception ex){
			Debug.Log(ex.Message);
		}
	}

	void OnConnectionFailed(BluetoothHelper helper)
	{
		Debug.Log("Connection Failed");
	}

	void OnDestroy()
	{
		if(bluetoothHelper!=null)
		bluetoothHelper.Disconnect();
	}

	#endregion
}
