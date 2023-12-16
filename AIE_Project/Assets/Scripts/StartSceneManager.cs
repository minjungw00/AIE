using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class StartSceneManager : MonoBehaviour
{
    public void ConnectArduino(){
        GameManager.instance.ConnectBluetooth();
    }

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        if(GameManager.instance.bluetoothHelper.isConnected()){
            SceneManager.LoadScene("PlayScene");
        }
    }
}
