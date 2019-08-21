/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
using UnityEngine;
using UnityEngine.AI;

public class Player : MonoBehaviour
{

    private NavMeshAgent agent;
    private GameObject mano;
    private GameObject ball;
    private Folie.Player player;

    internal bool initComplete = false;


    private void Start()
    {
        agent = GetComponent<NavMeshAgent>();
        mano = GameObject.FindGameObjectWithTag("Mano");

        initComplete = true;
    }

    private void Update()
    {
        //
    }

    internal void init(GameObject ball, Folie.Player player)
    {
        this.ball = ball;
        this.player = player;
        this.player.event_moveAt += player_event_moveAt;
        this.player.event_rotate += player_event_rotate;
    }

    private void player_event_rotate(float rot_y)
    {
        transform.Rotate(Vector3.up, rot_y, Space.Self);
    }

    private void player_event_moveAt(float pos_x, float pos_z)
    {
        agent.destination = new Vector3(pos_x, 0, pos_z);        
    }


}