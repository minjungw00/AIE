using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

/*
메인 게임 메커니즘 관련 스크립트

GameState
Idle : 기본 상태
Memorise : 아두이노 로터리 엔코더의 버튼이 입력되면 색상 암기 모드로 전환해 색을 섞고,
            플레이어는 지정된 시간동안 큐브의 색상을 암기함
Solve : 아두이노의 로터리 엔코더의 다이얼을 조절해 주어진 색상을 맞추면 성공
        이후 Idle로 전환하면서 점수(해결한 시간)를 출력
*/

public class PlaySceneManager : MonoBehaviour
{
    #region Properties and Variables

    public enum GameState{
        Idle,
        Memorise,
        Solve
    }

    public GameObject cube;
    public GameState gameState;

    public TextMeshProUGUI timerText;
    public TextMeshProUGUI stateText;
    public TextMeshProUGUI noticeText;

    public List<float> scores;

    public float time;

    #endregion


    #region Methods

    public void ChangeModeToMemorise(){
        cube.GetComponent<CubeColorChange>().ShuffleColor();
        time = 10.0f;

        stateText.text = "Memorise Cube's Colors...";
        noticeText.text = "";

        gameState = GameState.Memorise;
    }

    public void ChangeModeToSolve(){
        cube.GetComponent<CubeColorChange>().ShuffleGuessColor();
        time = 0.0f;

        stateText.text = "Find Colors!";
        noticeText.text = "";

        gameState = GameState.Solve;
    }

    public void ChangeModeToIdle(){
        RecodeScore(time);

        stateText.text = "You Find All Colors!";
        noticeText.text = "Press Cube's Button to Start";

        gameState = GameState.Idle;
    }

    public int RecodeScore(float t){
        scores.Add(time);
        scores.Sort();
        if(scores.Count > 10){
            scores.RemoveAt(10);
        }
        int rank = scores.FindIndex(x => Math.Abs(t - x) < 0.000000001);
        return rank + 1;
    }

    public bool CheckPressButton(){
        int[] enc = GameManager.instance.enc;
        for(int i = 0; i < 6; ++i){
            if(enc[i] == 3) return true;
        }
        return false;
    }

    public void Shuffle(){
        cube.GetComponent<CubeColorChange>().ShuffleGuessColor();
        StartCoroutine(cube.GetComponent<CubeColorChange>().SendColorData());
    }

    #endregion


    #region Monobehavior Callbacks

    // Start is called before the first frame update
    void Start()
    {
        gameState = GameState.Idle;

        time = 0.0f;
        
        timerText.text = (Mathf.Floor(time * 10f) / 10f).ToString();
        stateText.text = "Find Color";
        noticeText.text = "Press Cube's Button to Start";
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if(gameState == GameState.Idle){
            if(CheckPressButton()) ChangeModeToMemorise();
        }
        else if(gameState == GameState.Memorise){
            time -= Time.deltaTime;
            timerText.text = (Mathf.Floor(time * 10f) / 10f).ToString();

            if(time <= 0.0f){
                ChangeModeToSolve();
            }
        }
        else if(gameState == GameState.Solve){
            time += Time.deltaTime;
            timerText.text = (Mathf.Floor(time * 10f) / 10f).ToString();

            if(cube.GetComponent<CubeColorChange>().CompareColor()){
                ChangeModeToIdle();
            }
        }
    }

    #endregion
}
