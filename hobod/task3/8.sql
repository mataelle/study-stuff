add jar /opt/cloudera/parcels/CDH/jars/hive-contrib-0.13.1-cdh5.3.2.jar;
add file script.py;
USE s09311;
DROP VIEW IF EXISTS newView;
DROP VIEW IF EXISTS newView1;
CREATE VIEW newView AS
SELECT TRANSFORM (request) USING 'python script.py' as (request, requestNew) from UserLogs; 
CREATE VIEW newView1 AS
SELECT ul2.ip, ul2.date,  nv.requestNew, ul2.size, ul2.statusCode, ul2.browserInfo  from UserLogs as ul2, newView as nv;
select * from newView1 limit 10;
