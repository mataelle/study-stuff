add jar /opt/cloudera/parcels/CDH/jars/hive-contrib-0.13.1-cdh5.3.2.jar;
USE s09311;
set hive.exec.dynamic.partition.mode=nonstrict;

DROP TABLE IF EXISTS IPData;
CREATE EXTERNAL TABLE IPData (
 ip STRING,
 region STRING
)
ROW FORMAT SERDE 'org.apache.hadoop.hive.contrib.serde2.RegexSerDe'
WITH SERDEPROPERTIES (
 	"input.regex" =   '^(\\S*)\\s(.*)$',
    "output.format.string" = "%1$s %2$s"
)
STORED AS TEXTFILE
LOCATION '/data/user_logs/ip_data_S';

-- select * from IPData limit 10;

DROP TABLE IF EXISTS UserData;
CREATE EXTERNAL TABLE UserData (
 ip STRING,
 browserInfo STRING,
 sex STRING,
 age STRING
)
ROW FORMAT SERDE 'org.apache.hadoop.hive.contrib.serde2.RegexSerDe'
WITH SERDEPROPERTIES (
 	"input.regex" =   '^(\\S*)\\s(.*)\\s(.*)\\s(.*)$',
    "output.format.string" = "%1$s %2$s %3$s %4$s"
)
STORED AS TEXTFILE
LOCATION '/data/user_logs/user_data_S';

-- select * from UserData limit 10;

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

DROP TABLE IF EXISTS UserLogs;
CREATE EXTERNAL TABLE UserLogs (
	ip STRING,
	request STRING,
	size STRING,
	statusCode STRING,
	browserInfo STRING
) 
PARTITIONED BY (date STRING)
STORED AS SEQUENCEFILE;

FROM UserLogs0 ul
INSERT OVERWRITE TABLE UserLogs PARTITION(date)
       SELECT ul.ip, ul.request, ul.size, ul.statusCode, ul.browserInfo, ul.date;

-- SELECT * FROM UserLogs limit 10;
