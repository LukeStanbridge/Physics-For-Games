using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CustomBullet : MonoBehaviour
{
    //assignables
    public Rigidbody rb;
    public GameObject explosion;
    public LayerMask whatIsEnemies;
    public GameObject ignore;

    //stats
    [Range(0f, 1f)]
    public float bounciness;
    public bool useGravity;

    //damage
    public int explosionDamage;
    public float explosionRange;
    public float explosionForce;
    public float explosionUpwards;

    //lifetime
    public int maxCollisions;
    public float maxLifetime;
    public bool explodeOnTouch = true;
    public bool explodeOnImpact = true;

    private int collisons;
    private PhysicMaterial physicsMat;

    private void Start()
    {
        Setup();
    }

    private void Update()
    {
        //when to explode
        if (collisons > maxCollisions) Explode();

        //count down lifetime when not in slow mo
        if (!Input.GetKey(KeyCode.LeftShift))
        {
            maxLifetime -= Time.deltaTime;
            if (maxLifetime <= 0f) Explode();
        }
    }

    private void Explode()
    {
        //instantiate explosion
        if (explosion != null) Instantiate(explosion, transform.position, Quaternion.identity);

        //check for enemies
        Collider[] enemies = Physics.OverlapSphere(transform.position, explosionRange, whatIsEnemies);
        for (int i = 0; i < enemies.Length; i++)
        {
            //Get component of enemy and call Take Damage
            if(enemies[i].GetComponent<TurretAI>()) enemies[i].GetComponent<TurretAI>().TakeDamage(explosionDamage);
            if (enemies[i].GetComponent<PlayerController>())
            {
                enemies[i].GetComponent<PlayerController>().TakeDamage(explosionDamage);
            }

            //add explosion force if enemy has rigidbody
            if (enemies[i].GetComponent<Rigidbody>() && !enemies[i].gameObject.CompareTag("Player"))
            {
                enemies[i].GetComponent<Rigidbody>().AddExplosionForce(explosionForce, transform.position, explosionRange, explosionUpwards);
            }

            if (enemies[i].GetComponent<Ragdoll>()) enemies[i].GetComponent<Ragdoll>().ToggleRagdoll(true);

            Collider[] ragdollColliders = Physics.OverlapSphere(transform.position, explosionRange);
            foreach (Collider c in ragdollColliders)
            {
                if (c.gameObject.tag == "Ragdoll")
                {
                    Rigidbody rb = c.GetComponent<Rigidbody>();
                    if (rb != null)
                    {
                        rb.AddExplosionForce(explosionForce, transform.position, explosionRange, explosionUpwards);
                    }
                }
            }

        }

        //add a little delay, just to make verything works okay
        Invoke("Delay", 0.01f);
    }

    private void Delay()
    {
        Destroy(gameObject);
    }

    private void OnCollisionEnter(Collision collision)
    {
        //dont count collisons with other bullets
        if (collision.collider.CompareTag("Bullet")) return;

        collisons++;

        //explode if bullet hits enemy directly and explodeOnTouch is activated
        if (collision.collider.CompareTag("Enemy") && explodeOnTouch)
        {
            Explode();
            return;
        }

        if (collision.collider.gameObject.layer == 8 && explodeOnImpact)
        {
            Explode();
            return;
        }
        if (collision.collider.gameObject.layer == 9 && explodeOnImpact)
        {
            Explode();
            return;
        }

        if (ignore != null)
        {
            if (collision.collider.CompareTag("Player") && explodeOnTouch && collision.collider.tag != ignore.tag)
            {
                Explode();
                return;
            }
        }
    }

    private void Setup()
    {
        //create new physics material
        physicsMat = new PhysicMaterial();
        physicsMat.bounciness = bounciness;
        physicsMat.frictionCombine = PhysicMaterialCombine.Minimum;
        physicsMat.bounceCombine = PhysicMaterialCombine.Maximum;

        //assign material to collider
        GetComponent<SphereCollider>().material = physicsMat;
        //set gravity
        rb.useGravity = useGravity;
    }

    private void OnDrawGizmosSelected()
    {
        Gizmos.color = Color.red;
        Gizmos.DrawWireSphere(transform.position, explosionRange);
    }
}
