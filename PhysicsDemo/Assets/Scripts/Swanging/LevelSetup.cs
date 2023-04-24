using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LevelSetup : MonoBehaviour
{
    [SerializeField] private Transform level1;
    [SerializeField] private Transform level2;
    [SerializeField] private Transform level3;
    [SerializeField] private Transform level4;
    [SerializeField] private ParticleSystem openLevel;
    [SerializeField] private ParticleSystem closedLevel;
    private ParticleSystem level1Particles;
    private ParticleSystem level2Particles;
    private ParticleSystem level3Particles;
    private ParticleSystem level4Particles;

    private bool level1Open = false;
    private bool level2Open = false;
    private bool level3Open = false;
    private bool level4Open = false;
    private bool level1Complete = false;
    private bool level2Complete = false;
    private bool level3Complete = false;
    private bool level4Complete = false;

    [SerializeField] DancingAI dancingNPC;

    // Start is called before the first frame update
    void Start()
    {
        level1Particles = Instantiate(closedLevel, level1.transform.position, closedLevel.transform.rotation);
        level2Particles = Instantiate(closedLevel, level2.transform.position, closedLevel.transform.rotation);
        level3Particles = Instantiate(closedLevel, level3.transform.position, closedLevel.transform.rotation);
        level4Particles = Instantiate(closedLevel, level4.transform.position, closedLevel.transform.rotation);
    }

    // Update is called once per frame
    void Update()
    {
        if (dancingNPC.startGame)
        {
            level1Open = true;
            if (level1Open)
            {
                level1.GetComponent<BoxCollider>().enabled = true;
                if (level1Particles != null) Destroy(level1Particles.gameObject);
                level1Particles = changeParticles(level1);
                dancingNPC.startGame = false;
            }
        }
        else if (level1Complete)
        {
            level2Open = true;
            level1.GetComponent<BoxCollider>().enabled = false;
            level2.GetComponent<BoxCollider>().enabled = true;
        }
    }

    ParticleSystem changeParticles(Transform levelPosition)
    {
        return Instantiate(openLevel, levelPosition.transform.position, openLevel.transform.rotation);
    }
}
