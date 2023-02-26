using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class SpawnEnemy : MonoBehaviour
{
    public Transform spawnPointMin;
    public Transform spawnPointMax;
    public GameObject spawnEnemy;
    private Vector3 spawnPostion;
    [SerializeField] private float timer;
    [SerializeField] private float timerReset;
    public TextMeshProUGUI invaderDisplay;
    public int score;

    private void Update()
    {
        timer -= Time.deltaTime;
        if (spawnPointMin != null && spawnPointMax != null && timer <= 0) 
        {
            GetRandomSpawnPoint();
            Instantiate(spawnEnemy, spawnPostion, transform.rotation * Quaternion.Euler(0f, 180f, 0f));
            timer = timerReset;
        }

        if (invaderDisplay != null)
            invaderDisplay.SetText("Invaders: " + score);
    }

    private void GetRandomSpawnPoint()
    {
        Vector3 randomPoint = new Vector3(Random.Range(spawnPointMin.transform.position.x, spawnPointMax.transform.position.x), 0, spawnPointMax.transform.position.z);
        spawnPostion = randomPoint;
    }
}
