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

    private Transform inMano;


    public void init(Folie.Ball ball)
    {
        this.ball = ball;
        this.ball.event_shootAt += ball_event_shootAt;
        this.ball.event_attachToHand += ball_event_attachToHand;
    }

    private void ball_event_attachToHand(string player_name)
    {
        foreach (var p in GB.players)
            if (p.Key.name.Equals(player_name))
            {
                var player = p.Value.GetComponent<Player>();
                inMano = player.mano;
                break;
            }
    }

    private void Start()
    {
        rigidBody = GetComponent<Rigidbody>();
    }

    private void Update()
    {
        if (inMano != null)
            transform.SetPositionAndRotation(inMano.position, inMano.rotation);

        if (ball != null)
        {
            ball.pos_x = transform.position.x;
            ball.pos_y = transform.position.y;
            ball.pos_z = transform.position.z;
        }
    }


    private void ball_event_shootAt(float pos_x, float pos_z)
    {
        inMano = null;
        shootAt(new Vector3(pos_x, 0, pos_z));
    }

    private void shootAt(Vector3 p)
    {
        var dir = p - transform.position;
        var h = dir.y;
        dir.y = 0;

        var dist = dir.magnitude;
        dir.y = dist;
        dist += h;

        var vel = Mathf.Sqrt(dist * Physics.gravity.magnitude);

        rigidBody.velocity = vel * dir.normalized;
    }


}