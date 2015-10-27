add jar /opt/cloudera/parcels/CDH/jars/hive-contrib-0.13.1-cdh5.3.2.jar;
USE s09311;

SELECT COUNT(*), date FROM UserLogs GROUP BY date;