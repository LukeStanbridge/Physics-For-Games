using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WeaponRecoil : MonoBehaviour
{
    public Vector3 upRecoil;
    Vector3 originalRotation;

    
    void Start()
    {
        originalRotation = transform.localEulerAngles;
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Mouse0))
            AddRecoil();
        else if (Input.GetKeyUp(KeyCode.Mouse0))
            StopRecoil();
    }

    private void AddRecoil()
    {
        transform.localEulerAngles += upRecoil;
    }

    private void StopRecoil()
    {
        transform.localEulerAngles = originalRotation;
    }
}
