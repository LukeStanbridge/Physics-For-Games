using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CharacterMover : MonoBehaviour
{
    public float speed = 10;
    public float jumpVelocity = 10f;
    public Vector3 velocity = new Vector3();
    public bool isGrounded;
    public float mass;

    private Camera cam;
    private CharacterController cc;
    private Vector2 moveInput = new Vector2();
    private bool jumpInput;
    private Vector3 hitDirection;

    void Update()
    {
        moveInput.x = Input.GetAxis("Horizontal");
        moveInput.y = Input.GetAxis("Vertical");
        jumpInput = Input.GetButton("Jump");
    }

    // Start is called before the first frame update
    void Start()
    {
        cc = GetComponent<CharacterController>();
        cam = Camera.main;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        Vector3 delta;

        // player movement using WASD or arrow keys

        // find the horizontal unit vector facing forward from the camera
        Vector3 camForward = cam.transform.forward;
        camForward.y = 0;
        camForward.Normalize();

        // use our camera's right vector, which is always horizontal
        Vector3 camRight = cam.transform.right;

        delta = (moveInput.x * camRight + moveInput.y * camForward) * speed;

        if (isGrounded || moveInput.x != 0 || moveInput.y != 0)
        {
            velocity.x = delta.x;
            velocity.z = delta.z;
        }

        // check for jumping
        if (jumpInput && isGrounded)
            velocity.y = jumpVelocity;

        // check if we've hit ground from falling. If so, remove our velocity
        if (isGrounded && velocity.y < 0)
            velocity.y = 0;

        // apply gravity after zeroing velocity so we register as grounded still
        velocity += Physics.gravity * Time.fixedDeltaTime;

        cc.Move(velocity * Time.deltaTime);
        isGrounded = cc.isGrounded;
    }

    void OnControllerColliderHit(ControllerColliderHit hit)
    {
        hitDirection = hit.point - transform.position;
        if (hit.rigidbody)
        {
            hit.rigidbody.AddForceAtPosition(velocity * mass, hit.point);
        }
    }
}
