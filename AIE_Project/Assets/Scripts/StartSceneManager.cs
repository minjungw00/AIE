using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

/*
시작 화면 관련 스크립트

게임을 시작하기 전에 블루투스를 연결, 연결에 성공하면 메인 게임 실행
*/

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
        // 아두이노가 연결되면 메인 게임 실행
        if(GameManager.instance.bluetoothHelper.isConnected()){
            SceneManager.LoadScene("PlayScene");
        }
    }
}
