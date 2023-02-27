using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class CameraController : MonoBehaviour
{
    public Transform target;
    public float speed = 180;
    public float distance = 4;
    public float currentDistance;

    // Start is called before the first frame update
    void Start()
    {
        currentDistance = distance;
    }

    // Update is called once per frame
    void Update()
    {
        // right drag rotates the camera
        if (Input.GetMouseButton(1))
        {
            Vector3 angles = transform.eulerAngles;
            float dx = Input.GetAxis("Mouse Y");
            float dy = Input.GetAxis("Mouse X");
            // look up and down by rotating around X-axis
            angles.x = Mathf.Clamp(angles.x + dx * speed * Time.deltaTime, 0, 70);
            // spin the camera round
            angles.y += dy * speed * Time.deltaTime;
            transform.eulerAngles = angles;
        }

        RaycastHit hit;
        if (Physics.Raycast(target.position, -transform.forward, out hit, distance))
        {
            // snap the camera right in to where the collision happened
            currentDistance = hit.distance;
        }
        else
        {
            // relax the camera back to the desired distance
            currentDistance = Mathf.MoveTowards(currentDistance, distance, Time.deltaTime);
        }

        // look at the target point
        transform.position = target.position - distance * transform.forward;
    }
}
