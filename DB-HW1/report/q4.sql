SELECT Id, AVG(kills) AS avgKills
FROM (SELECT Id, matchId, kills FROM player_statistic) AS player
	 JOIN 
	 (SELECT matchId FROM `match` WHERE numGroups <= 10) AS matchIdOnly
	 USING(matchId)
GROUP BY Id
ORDER BY AVG(kills) DESC
LIMIT 20;
