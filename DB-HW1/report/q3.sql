SELECT matchType, COUNT(matchType) AS count
FROM `match`
WHERE matchType LIKE "%fpp%"
GROUP BY matchType
ORDER BY COUNT(matchType);
