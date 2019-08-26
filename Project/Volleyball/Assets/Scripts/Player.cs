﻿/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
using System.Collections;
using UnityEngine;

public class Player : Folie.Player
{

    private Folie.CoRunner CoRunner;

    protected override void Start()
    {
        base.Start();

        CoRunner = new Folie.CoRunner(this);

        Yielder.Yielder.waiter(this, 3, () =>
        {
            Posizionati();
        });
    }

    void Posizionati()
    {
        if (this.startingPosition == Folie.GB.ePosition.p1 && this.team == Folie.GB.eTeam.A)
        {
            var pos_PosizioneInCampo = (Vector2)Folie.GB.getCoordinatesFromPosition(campo, startingPosition);
            var pos_PosizionePalla = Folie.REF.ball.transform.position;

            CoRunner.Enqueue(Cammina(pos_PosizioneInCampo));
            CoRunner.Enqueue(Cammina(pos_PosizionePalla));

            CoRunner.Run();
        }
    }

    IEnumerator Cammina(Vector3 destination)
    {
        agent.destination = destination;

        yield return new WaitUntil(() =>
        {
            return agent.destination.Equals(transform.position);
        });
    }


}