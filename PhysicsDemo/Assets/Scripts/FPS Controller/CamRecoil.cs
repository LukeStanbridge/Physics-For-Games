using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CamRecoil : MonoBehaviour
{
    [Header("Recoil Settings")]
    public float rotationSpeed = 8;
    public float returnSpeed = 25;
    public Vector3 recoilRotation = new Vector3(2, 2, 2);
    public Vector3 recoilRotationAiming = new Vector3(0.5f, 0.5f, 1.5f);
    public bool aiming;

    private Vector3 currentRotation;
    private Vector3 rotation;

    private void FixedUpdate()
    {
        currentRotation = Vector3.Lerp(currentRotation, Vector3.zero, returnSpeed * Time.deltaTime);
        rotation = Vector3.Slerp(rotation, currentRotation, returnSpeed * Time.deltaTime);
        transform.localRotation = Quaternion.Euler(rotation);
    }

    private void Fire()
    {
        if (aiming) 
        {
            currentRotation += new Vector3(-recoilRotationAiming.x, Random.Range(-recoilRotationAiming.y, -recoilRotationAiming.y), Random.Range(-recoilRotationAiming.z, -recoilRotationAiming.z));
        }
        else
        {
            currentRotation += new Vector3(-recoilRotation.x, Random.Range(-recoilRotation.y, -recoilRotation.y), Random.Range(-recoilRotation.z, -recoilRotation.z));
        }
    }

    private void Update()
    {
        if (Input.GetKey(KeyCode.Mouse0))
        {
            Fire();
        }
        //if(Input.GetKey(KeyCode.Mouse1)) 
        //{
        //    aiming = true;
        //}
        //else
        //{
        //    aiming = false;
        //}
    }
}
