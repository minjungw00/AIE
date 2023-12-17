using System.Collections;
using System.Collections.Generic;
using UnityEngine;

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
