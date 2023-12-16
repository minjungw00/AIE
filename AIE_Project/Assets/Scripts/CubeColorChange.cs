using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CubeColorChange : MonoBehaviour
{
    #region Properties & Variables

    public Dictionary<int, Color> colorSet = new Dictionary<int, Color>();

    public List<GameObject> plains;
    public int[] preState;
    public int[] curState;
    public int[] curColor;
    public int[] guessColor;

    #endregion


    #region Methods

    public void ShuffleColor(){
        for(int i = 0; i < 6; ++i){
            guessColor[i] = Random.Range(0, 5);
        }
    }

    public void DrawColor(){
        for(int i = 0; i < 6; ++i){
            Renderer renderer = plains[i].GetComponent<Renderer>();
            if(renderer != null){
                renderer.material.color = colorSet[curColor[i]];
            }
        }
    }

    public void StartGame(){
        ShuffleColor();
    }

    public void GetState(){
        for(int i = 0; i < 6; ++i){
            preState[i] = curState[i];
        }
        
        for(int i = 0; i < 6; ++i){
            curState[i] = GameManager.instance.enc[i];
        }
        // string str1 = preState[0].ToString() + preState[1].ToString() + preState[2].ToString() + preState[3].ToString() + preState[4].ToString() + preState[5].ToString();
        // Debug.Log("Pre:" + str1);
        // string str2 = curState[0].ToString() + curState[1].ToString() + curState[2].ToString() + curState[3].ToString() + curState[4].ToString() + curState[5].ToString();
        // Debug.Log("Cur:" + str2);
    }

    public void CheckState(){
        string str = curColor[0].ToString() + curColor[1].ToString() + curColor[2].ToString() + curColor[3].ToString() + curColor[4].ToString() + curColor[5].ToString();
        Debug.Log(str);
        for(int i = 0; i < 6; ++i){
            if(preState[i] != curState[i]){
                if(curState[i] == 1){
                    --curColor[i];
                    if(curColor[i] == -1){
                        curColor[i] = 4;
                    }
                }
                else if(curState[i] == 2){
                    ++curColor[i];
                    if(curColor[i] == 5){
                        curColor[i] = 0;
                    }
                }
            }
        }
    }

    #endregion


    #region Monobehavior Callbacks

    void Awake(){
        colorSet.Add(0, Color.red); // RED
        colorSet.Add(1, new Color(1f, 0.5f, 0f)); // ORANGE
        colorSet.Add(2, Color.yellow); // YELLOW
        colorSet.Add(3, Color.green); // GREEN
        colorSet.Add(4, Color.blue); // BLUE

        preState = new int[6];
        curState = new int[6];
        curColor = new int[6];
        guessColor = new int[6];
    }

    // Start is called before the first frame update
    void Start()
    {
        foreach(Transform child in this.transform){
            if(child != null){
                plains.Add(child.gameObject);
            }
        }

        ShuffleColor();
        curColor = guessColor;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        GetState();
        CheckState();
        DrawColor();
    }

    #endregion
}
