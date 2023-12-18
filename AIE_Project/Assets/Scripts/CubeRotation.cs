using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
큐브 회전 관련 스크립트

자이로센서의 값을 통해 큐브의 회전값 설정
*/

public class CubeRotation : MonoBehaviour
{
    public Vector3 rot;
    public float duration = 0.02f;
    public float time;

    // Start is called before the first frame update
    void Start()
    {
       time = 0.0f; 
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        time += Time.deltaTime;
        if(time >= duration){
            rot = GameManager.instance.gyro;
            transform.rotation = Quaternion.Euler(rot);
            time = 0.0f;
        }
    }
}
