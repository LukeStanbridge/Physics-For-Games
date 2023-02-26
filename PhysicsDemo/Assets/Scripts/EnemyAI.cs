using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.AI;

public class EnemyAI : MonoBehaviour
{
    //assignables
    public NavMeshAgent agent;
    public Transform player;
    public LayerMask whatIsGround, whatIsPlayer;
    public float health;
    public SpawnEnemy spawnEnemy;

    //patrolling
    public Vector3 walkPoint;
    public Transform rushPointMin;
    public Transform rushPointMax;
    private Vector3 rushPoint;
    private bool walkPointSet;
    public float walkPointRange;
    private Animator anim;
    public Ragdoll ragdoll;

    //attacking
    public float timeBetweenAttacks;
    private bool alreadyAttacked;
    public GameObject projectile;

    //states
    public float sightRange, attackRange;
    public bool playerInSightRange, playerInAttackRange;

    private void Awake()
    {
        player = GameObject.Find("Player").transform;
        agent = GetComponent<NavMeshAgent>();
        rushPointMin = GameObject.Find("RushPointMin").transform;
        rushPointMax = GameObject.Find("RushPointMax").transform;
        spawnEnemy = GameObject.Find("Ground").GetComponent<SpawnEnemy>();
        anim = gameObject.GetComponent<Animator>();
        

        GetRandomRushPoint();
    }

    private void Update()
    {
        anim.Play("Running");
        RushTower();
    }

    private void RushTower()
    {
        if (agent.enabled)
        agent.SetDestination(rushPoint);
    }

    public void TakeDamage(int damage)
    {
        health -= damage;
        agent.enabled = false;
        ragdoll.ToggleRagdoll(true);

        if (health <= 0) DestroyEnemy();
    }

    private void DestroyEnemy()
    {
        Destroy(gameObject, 5f);
    }

    private void GetRandomRushPoint()
    {
        Vector3 randomPoint = new Vector3(Random.Range(rushPointMin.transform.position.x, rushPointMax.transform.position.x), 2, rushPointMax.transform.position.z);
        rushPoint = randomPoint;
    }

    private void OnTriggerEnter(Collider other)
    {
        //add to invaders and destroy enemies that succesfully invade
        spawnEnemy.score++;
        if (other.CompareTag("Finish")) Invoke(nameof(DestroyEnemy), 0.5f);
    }
}
