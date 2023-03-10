using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class DancingAI : MonoBehaviour
{
    //assignables
    //public LayerMask wreckingBall;
    public Ragdoll ragdoll;
    public GameObject explosion;

    //patrolling
    private Animator anim;

    private void Awake()
    {
        anim = gameObject.GetComponent<Animator>();
    }

    private void Update()
    {
        anim.Play("Hip Hop Dancing");

        //Collider[] enemies = Physics.OverlapSphere(transform.position, 20, wreckingBall);
        //for (int i = 0; i < enemies.Length; i++)
        //{
        //    if (enemies[i].gameObject.layer == 11)
        //    {
        //        this.gameObject.GetComponent<Rigidbody>().isKinematic = false;
        //        Invoke(nameof(Ragdoll), 0.5f);
        //    }s
        //}
    }

    public void Ragdoll()
    {
        ragdoll.ToggleRagdoll(true);

        DestroyEnemy();
    }

    private void DestroyEnemy()
    {
        Destroy(gameObject, 4f);
    }
    
    private void OnDrawGizmosSelected()
    {
        Gizmos.color = Color.red;
        Gizmos.DrawWireSphere(transform.position, 20);
    }
    
    private void Explode()
    {
        if (explosion != null) Instantiate(explosion, transform.position, Quaternion.identity);
    }
}
