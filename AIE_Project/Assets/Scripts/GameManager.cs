using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using ArduinoBluetoothAPI;
using System;
using System.Text;

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
            gyro = new Vector3(int.Parse(temp[0]), int.Parse(temp[1]), int.Parse(temp[2]));
            for(int i = 0; i < 6; ++i){
                enc[i] = int.Parse(temp[3 + i]);
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
