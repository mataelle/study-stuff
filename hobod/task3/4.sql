add jar /opt/cloudera/parcels/CDH/jars/hive-contrib-0.13.1-cdh5.3.2.jar;
USE s09311;

SELECT region, sex, COUNT(*) FROM UserLogs JOIN IPData ON (UserLogs.ip = IPData.ip) JOIN UserData ON ( UserLogs.ip = UserData.ip )
GROUP BY region, sex