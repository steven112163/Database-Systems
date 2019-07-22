SELECT teamRoadKills, AVG(winPlacePerc) AS avgWinPlacePerc
FROM (SELECT matchId, SUM(roadKills) AS teamRoadKills, AVG(winPlacePerc) AS winPlacePerc FROM player_statistic GROUP BY groupId, matchId) AS player JOIN (SELECT matchId FROM `match` WHERE matchType LIKE "squad%") AS matchIdOnly USING(matchId)
GROUP BY teamRoadKills
ORDER BY teamRoadKills DESC;
