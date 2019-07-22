SELECT COUNT(Id) AS numberOfPlayers, MAX(heals) AS maxHeals
FROM (SELECT Id, matchId, damageDealt, winPlacePerc, heals FROM player_statistic WHERE damageDealt = 0 AND winPlacePerc = 1) AS player
	 JOIN
	 (SELECT matchId FROM `match` WHERE matchDuration > ALL (SELECT AVG(matchDuration) AS avgDuration FROM `match`)) AS targetMatch
	 USING(matchId);
