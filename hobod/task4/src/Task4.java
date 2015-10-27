package ru.mipt;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.io.ImmutableBytesWritable;
import org.apache.hadoop.hbase.mapreduce.TableMapReduceUtil;
import org.apache.hadoop.hbase.mapreduce.TableReducer;
import org.apache.hadoop.hbase.util.Bytes;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

public class Task4 extends Configured implements Tool
{
    public static void main(String[] args) throws Exception {
        ToolRunner.run(new Task4(), args);
    }

    @Override
    public int run(String[] args) throws Exception
    {
        Configuration conf = HBaseConfiguration.create (getConf());
        HBaseAdmin admin = new HBaseAdmin(conf);

        String tableName = "UsersTable";
        if (!admin.tableExists(tableName))
            createTable(admin, tableName, null, "main");
        else
        {
            admin.disableTable(tableName);
            admin.deleteTable(tableName);
            createTable(admin, tableName, null, "main");
        }

        HTable table = new HTable(conf, tableName);
        Job job = Job.getInstance(conf);

        job.setJarByClass(Task4.class);
        job.setMapperClass(TaskMapper.class);
        TableMapReduceUtil.addDependencyJars(job);
        TableMapReduceUtil.addHBaseDependencyJars(conf);
        TableMapReduceUtil.initTableReducerJob(tableName, TaskReducer.class, job);

        job.setReducerClass(TaskReducer.class);

        job.setInputFormatClass(KeyValueTextInputFormat.class);
        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(IntWritable.class);

        FileInputFormat.addInputPath(job, new Path(args[0]));
        return job.waitForCompletion(true) ? 0 : -1;
    }

    public static class TaskMapper extends Mapper <Text, Text, Text, IntWritable>
    {
        @Override
        public void map(Text userId, Text info, Context context)
                throws IOException, InterruptedException
        {
            String[] infoSplited = info.toString().split("\\s+");

            Calendar date = new GregorianCalendar();
            date.setTimeInMillis (Long.parseLong (infoSplited[0])*1000);

            SimpleDateFormat format = new SimpleDateFormat("dd.MM.yyyy");

            context.write (new Text (format.format (date.getTime()) + " " + infoSplited[1]), new IntWritable(1));
        }
    }

    public static class TaskReducer extends TableReducer<Text, IntWritable, ImmutableBytesWritable>
    {
        public void reduce(Text datePage, Iterable<IntWritable> ones, Context context) throws IOException, InterruptedException
        {
            String[] datePageSplited = datePage.toString ().split (" ");
            int count = 0;
            for (IntWritable one : ones) count +=1;

            Put newPut = new Put(Bytes.toBytes(datePageSplited[0])); // date
            newPut.add(Bytes.toBytes("main"), Bytes.toBytes(datePageSplited[1]), Bytes.toBytes(Integer.toString(count)));

            context.write(null, newPut);
//            context.write(new Text(datePageSplited[0]), new Text(datePageSplited[1] + " "+ Integer.toString(count)));
        }
    }

    public static void createTable(HBaseAdmin admin, String table, byte[][] splitKeys, String... colfams) throws IOException {
        HTableDescriptor td = new HTableDescriptor(table);
        for (String cf : colfams) {
            HColumnDescriptor coldef = new HColumnDescriptor(cf);
            td.addFamily(coldef);
        }
        if (splitKeys != null) {
            admin.createTable(td, splitKeys);
        } else {
            admin.createTable(td);
        }
    }

}
