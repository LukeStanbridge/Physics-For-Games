using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class KartController : MonoBehaviour
{
    [SerializeField] private GameObject kart;
    [SerializeField] private Rigidbody rb; 
    private float forwardAmount;
    private float currentSpeed;
    [SerializeField] private float forwardSpeed;

    private void Update()
    {
        kart.transform.position = rb.transform.position;

        if (Input.GetKeyDown(KeyCode.W))
        {
            forwardAmount = 1;
        }
        else
        {
            forwardAmount = 0;
        }
        

        if(forwardAmount > 0)
        {
            DriveForward();
        }
    }

    private void FixedUpdate()
    {
        rb.AddForce(transform.right * currentSpeed, ForceMode.Acceleration);
    }

    private void DriveForward()
    {
        currentSpeed = forwardAmount * forwardSpeed;
    }
}
