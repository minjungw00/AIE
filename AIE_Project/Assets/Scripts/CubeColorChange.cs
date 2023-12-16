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

    #endregion


    #region Monobehavior Callbacks

    void Awake(){
        colorSet.Add(0, Color.red);
        colorSet.Add(1, new Color(1f, 0.5f, 0f));
        colorSet.Add(2, Color.yellow);
        colorSet.Add(3, Color.green);
        colorSet.Add(4, Color.blue);

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
    void Update()
    {
        DrawColor();
    }

    #endregion
}
