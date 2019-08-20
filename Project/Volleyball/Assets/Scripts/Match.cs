/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
using System.Collections.Generic;
using UnityEngine;

public class Match : MonoBehaviour
{

    private Folie.Game game = new Folie.Game(25);


    public GameObject TeamA_C1, TeamA_P1, TeamA_B1;
    public GameObject TeamA_C2, TeamA_P2, TeamA_B2;

    public GameObject TeamB_C1, TeamB_P1, TeamB_B1;
    public GameObject TeamB_C2, TeamB_P2, TeamB_B2;

    private Dictionary<Folie.Player, GameObject> playersTeamA, playersTeamB;


    void Start()
    {
        var teamA = game.teamA;
        var teamB = game.teamB;

        playersTeamA = new Dictionary<Folie.Player, GameObject>
        {
            { teamA.getPlayerWithRole(Folie.GB.eRole.c1), TeamA_C1},
            { teamA.getPlayerWithRole(Folie.GB.eRole.p1), TeamA_P1},
            { teamA.getPlayerWithRole(Folie.GB.eRole.b1), TeamA_B1},
            { teamA.getPlayerWithRole(Folie.GB.eRole.b2), TeamA_B2},
            { teamA.getPlayerWithRole(Folie.GB.eRole.p2), TeamA_P2},
            { teamA.getPlayerWithRole(Folie.GB.eRole.c2), TeamA_C2}
        };

        playersTeamB = new Dictionary<Folie.Player, GameObject>
        {
            { teamB.getPlayerWithRole(Folie.GB.eRole.c1), TeamB_C1},
            { teamB.getPlayerWithRole(Folie.GB.eRole.p1), TeamB_P1},
            { teamB.getPlayerWithRole(Folie.GB.eRole.b1), TeamB_B1},
            { teamB.getPlayerWithRole(Folie.GB.eRole.b2), TeamB_B2},
            { teamB.getPlayerWithRole(Folie.GB.eRole.p2), TeamB_P2},
            { teamB.getPlayerWithRole(Folie.GB.eRole.c2), TeamB_C2}
        };

        //game.start();
    }

    void Update()
    {
        updatePositionOfTeams();
    }

    private void updatePositionOfTeams()
    {
        updatePositionOfPlayers(playersTeamA);
        updatePositionOfPlayers(playersTeamB);
    }

    private void updatePositionOfPlayers(Dictionary<Folie.Player, GameObject> team)
    {
        foreach (var p in team)
            updatePositionOfPlayer(p.Value, p.Key);
    }

    private void updatePositionOfPlayer(GameObject obj, Folie.Player player)
    {
        var p = obj.GetComponent<Player>();
        p.playerWalkTo(new Vector3(player.pos_x, player.pos_y, player.pos_z));
    }


}