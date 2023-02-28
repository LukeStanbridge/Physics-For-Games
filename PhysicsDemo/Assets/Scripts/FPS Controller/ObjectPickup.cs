using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectPickup : MonoBehaviour
{
    [Header("Pickup Settings")]
    [SerializeField] Transform holdArea;
    [SerializeField] Transform player;
    private GameObject heldObj;
    private Rigidbody heldObjRB;

    [Header("Physics Parameters")]
    [SerializeField] private float pickupRange = 5.0f;
    [SerializeField] private float pickupForce = 150.0f;
    public float dropForwardForce, dropUpwardForce;

    private void Update()
    {
        if (Input.GetKeyUp(KeyCode.F))
        {
            if (heldObj == null)
            {
                RaycastHit hit;
                if (Physics.Raycast(transform.position, transform.TransformDirection(Vector3.forward), out hit, pickupRange))
                {
                    //pickup object
                    PickupObject(hit.transform.gameObject);
                }
            }
            else
            {
                //drop object
                DropObject();
            }
        }
        
        if (heldObj != null)
        {
            //move object
        }
    }

    void PickupObject(GameObject pickObj)
    {
        if (pickObj.GetComponent<Rigidbody>())
        {
            heldObjRB = pickObj.GetComponent<Rigidbody>();
            heldObjRB.useGravity = false;
            heldObjRB.drag = 10;
            heldObjRB.constraints = RigidbodyConstraints.FreezeRotation;
            heldObjRB.transform.parent = holdArea;
            heldObjRB.transform.position = holdArea.position;
            heldObj = pickObj;
        }
    }

    void DropObject()
    {
        heldObjRB.isKinematic = false;
        heldObjRB.useGravity = true;
        heldObjRB.drag = 1;
        heldObjRB.constraints = RigidbodyConstraints.None;
        heldObj.transform.parent = null;
        heldObj = null;

        //Gun carries momentum of player
        heldObjRB.velocity = player.GetComponent<Rigidbody>().velocity;

        //AddForce
        heldObjRB.AddForce(holdArea.forward * dropForwardForce, ForceMode.Impulse);
        heldObjRB.AddForce(holdArea.up * dropUpwardForce, ForceMode.Impulse);
        //Add random rotation
        float random = Random.Range(-1f, 1f);
        heldObjRB.AddTorque(new Vector3(random, random, random) * 10);
    }

    void MoveObject()
    {
        if (Vector3.Distance(heldObj.transform.position, holdArea.position) > 0.1f)
        {
            Vector3 moveDirection = (holdArea.position - heldObj.transform.position);
            heldObjRB.AddForce(moveDirection * pickupForce);
        }
    }
}
