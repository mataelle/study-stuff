add jar /opt/cloudera/parcels/CDH/jars/hive-contrib-0.13.1-cdh5.3.2.jar;
USE s09311;

DROP TABLE IF EXISTS sampled;
CREATE EXTERNAL TABLE sampled (
	ip STRING,
	date STRING,
 	request STRING,
 	size STRING,
 	statusCode STRING,
 	browserInfo STRING
) 
STORED AS TEXTFILE;

FROM UserLogs TABLESAMPLE(40 percent)
INSERT OVERWRITE TABLE sampled
    SELECT *;

SELECT 100/40*COUNT(*) FROM sampled JOIN IPData ON (sampled.ip = IPData.ip) JOIN UserData ON (sampled.ip = UserData.ip)
GROUP BY region, sex
