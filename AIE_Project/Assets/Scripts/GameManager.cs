using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using ArduinoBluetoothAPI;
using System;
using System.Text;

/*
게임 매니저 스크립트

싱글톤 패턴으로 전체 게임을 관리하고
블루투스를 통해 아두이노와의 데이터 입출력을 관리
*/

public class GameManager : MonoBehaviour
{
    #region Propertices & Variables

    public static GameManager instance = null;

    // Use this for initialization
	public BluetoothHelper bluetoothHelper;
	string deviceName;

	string received_message;

    public Vector3 gyro;
    public int[] enc;

    #endregion


    #region Methods

	public void ConnectBluetooth(){
		if(bluetoothHelper.isDevicePaired())
			bluetoothHelper.Connect (); // tries to connect
	}

	public void DisconnectBluetooth(){
		bluetoothHelper.Disconnect ();
	}


	#endregion


    private void Awake(){
        if (instance == null)
        {
            instance = this;
            DontDestroyOnLoad(gameObject);
        }
        else
        {
            Destroy(this.gameObject);
            return;
        }

        enc = new int[6];
    }


    #region Callbacks

    // Start is called before the first frame update
    void Start()
    {
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
    void Update()
    {
        
    }

    //Asynchronous method to receive messages
	void OnMessageReceived(BluetoothHelper helper)
	{
        received_message = helper.Read();
        // Debug.Log(received_message);

        
        try{
            string[] temp = received_message.Split(',');
            // 자이로센서 데이터 추출
            // 아두이노 전원 연결 시 자이로센서의 상태에 따라 회전값의 초기 설정이 달라져 보정 필요
            gyro = new Vector3(-int.Parse(temp[0]), -int.Parse(temp[2]), -int.Parse(temp[1]));

            // 로터리 엔코더 데이터 추출
            for(int i = 0; i < 6; ++i){
                // enc[i] = int.Parse(temp[3 + i]);
                enc[i] = 3; // 로터리 엔코더가 동작하지 않아 게임 메커니즘 확인을 위해 임의 설정
            }
        }
        catch{
            gyro = new Vector3();
            for(int i = 0; i < 6; ++i){
                enc[i] = 0;
            }
        }
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
