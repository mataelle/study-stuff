add jar /opt/cloudera/parcels/CDH/jars/hive-contrib-0.13.1-cdh5.3.2.jar;
USE s09311;

DROP TABLE IF EXISTS RegionCount;
CREATE EXTERNAL TABLE RegionCount (
	region STRING,
	count INT
) 
STORED AS TEXTFILE;

FROM UserLogs JOIN IPData ON (UserLogs.ip = IPData.ip)
INSERT OVERWRITE TABLE RegionCount
       SELECT region, COUNT(*) GROUP BY region;

-- SELECT * FROM RegionCount LIMIT 10;


DROP TABLE IF EXISTS AvgRegionCount;
CREATE EXTERNAL TABLE AvgRegionCount (
	AvgCount INT
) 
STORED AS TEXTFILE;

FROM RegionCount
INSERT OVERWRITE TABLE AvgRegionCount
       SELECT AVG(count);

SELECT region, count, avgCount FROM RegionCount, AvgRegionCount
WHERE count>avgCount;