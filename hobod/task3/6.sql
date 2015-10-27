add jar /opt/cloudera/parcels/CDH/jars/hive-contrib-0.13.1-cdh5.3.2.jar;
USE s09311;
set hive.exec.dynamic.partition.mode=nonstrict;

DROP TABLE IF EXISTS UserLogs0;
CREATE EXTERNAL TABLE UserLogs0 (
	ip STRING,
	date STRING,
 	request STRING,
 	size STRING,
 	statusCode STRING,
 	browserInfo STRING
)
ROW FORMAT SERDE 'org.apache.hadoop.hive.contrib.serde2.RegexSerDe'
WITH SERDEPROPERTIES (
 	"input.regex" =   '^(\\S*)\\t\\t\\t(\\d{8})\\S*\\t(\\S*)\\t(\\S*)\\t(\\S*)\\t(\\S*).*$',
    "output.format.string" = "%1$s %2$s %3$s %4$s %5$s %6$s"
)
STORED AS TEXTFILE
LOCATION '/data/user_logs/user_logs_L';

DROP TABLE IF EXISTS UserLogsRCF;
CREATE EXTERNAL TABLE UserLogsRCF (
	ip STRING,
	request STRING,
	size STRING,
	statusCode STRING,
	browserInfo STRING
) 
PARTITIONED BY (date STRING)
STORED AS RCFILE;

FROM UserLogs0 ul
INSERT OVERWRITE TABLE UserLogsRCF PARTITION(date)
       SELECT ul.ip, ul.request, ul.size, ul.statusCode, ul.browserInfo, ul.date;

--------

DROP TABLE IF EXISTS RegionCount;
CREATE EXTERNAL TABLE RegionCount (
	request STRING,
	region STRING,
	count INT
) 
STORED AS RCFILE;

FROM UserLogsRCF JOIN IPData ON (UserLogsRCF.ip = IPData.ip)
INSERT OVERWRITE TABLE RegionCount
       SELECT request, region, COUNT(*) GROUP BY request, region;

DROP TABLE IF EXISTS MaxRegionCount;
CREATE EXTERNAL TABLE MaxRegionCount (
	region STRING,
	MaxCount INT
) 
STORED AS TEXTFILE;

FROM RegionCount
INSERT OVERWRITE TABLE MaxRegionCount
       SELECT region, 0.8*MAX(count) GROUP BY region;

SELECT RegionCount.region, request, count, maxCount FROM RegionCount, MaxRegionCount
WHERE count>maxCount;