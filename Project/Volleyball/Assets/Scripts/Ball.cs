/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
using UnityEngine;

public class Ball : MonoBehaviour
{

    private Rigidbody rigidBody;
    private Folie.Ball ball;


    public void init(Folie.Ball ball)
    {
        this.ball = ball;
        this.ball.shootAt += ball_shootAt;
    }

    private void Start()
    {
        rigidBody = GetComponent<Rigidbody>();
    }

    private void ball_shootAt(float pos_x, float pos_z)
    {
        //shootAt(new Vector3(pos_x, 0, pos_z));
    }

    private void shootAt(Vector3 point)
    {
        var velocity = ballisticVelocity(point, 45);

        rigidBody.velocity = velocity;
    }

    private Vector3 ballisticVelocity(Vector3 destination, float angle)
    {
        var dir = destination - transform.position;
        var height = dir.y;
        dir.y = 0;

        var dist = dir.magnitude;

        var a = angle * Mathf.Deg2Rad;

        dir.y = dist * Mathf.Tan(a);
        dist += height / Mathf.Tan(a);

        var velocity = Mathf.Sqrt(dist * Physics.gravity.magnitude / Mathf.Sin(2 * a));

        return velocity * dir.normalized;
    }


}