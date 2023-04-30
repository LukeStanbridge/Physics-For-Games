using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TextFacePlayer : MonoBehaviour
{
    private void Start()
    {
        LeanTween.moveY(this.gameObject, 7, 3).setLoopPingPong();
    }
    void Update()
    {
        transform.LookAt(Camera.main.transform.position);
        transform.Rotate(new Vector3(0, 180, 0));
    }
}
