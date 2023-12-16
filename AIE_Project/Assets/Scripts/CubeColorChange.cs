using System.Collections;
using System.Collections.Generic;
using UnityEditor;
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
            curColor[i] = guessColor[i];
        }
        string str = "";
        for(int i = 0; i < 6; ++i){
            str += guessColor[i].ToString();
        }
        Debug.Log(str);
    }

    public void ShuffleGuessColor(){
        for(int i = 0; i < 6; ++i){
            do{
                curColor[i] = Random.Range(0, 5);
            }
            while(guessColor[i] == curColor[i]);
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

    public void GetState(){
        for(int i = 0; i < 6; ++i){
            preState[i] = curState[i];
        }
        
        for(int i = 0; i < 6; ++i){
            curState[i] = GameManager.instance.enc[i];
        }
    }

    public void ChangeColor(){
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

    public bool CompareColor(){
        bool step = true;
        for(int i = 0; i < 6; ++i){
            for(int j = i; j < i + 6; ++j){
                for(int k = 0; k < 6; ++k){
                    if(curColor[k] != guessColor[(j + k) % 6]){
                        step = false;
                        break;
                    } 
                }
                if(step) return true;
                else step = true;
            }
        }
        return false;
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

        ShuffleGuessColor();
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        GetState();
        ChangeColor();
        DrawColor();
    }

    #endregion
}
