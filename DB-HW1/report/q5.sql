SELECT matchType, AVG(matchDuration)
FROM `match`
GROUP BY matchType
ORDER BY AVG(matchDuration);
