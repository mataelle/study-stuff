add jar /opt/cloudera/parcels/CDH/jars/hive-contrib-0.13.1-cdh5.3.2.jar;
add jar jar/reversed.jar;
USE s09311;

create temporary function reversed as 'reversed';

SELECT reversed(ip) from IPData;

DROP TEMPORARY FUNCTION reversed;