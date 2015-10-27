package ru.mipt;

import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.util.ToolRunner;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.FSDataInputStream;

import java.io.*;
import java.lang.InterruptedException;
import java.net.URI;
import java.util.Arrays;
import java.util.List;

import org.apache.hadoop.filecache.DistributedCache;

public class Task2 extends Configured implements Tool{

    public static void main(String[] args) throws Exception {
        ToolRunner.run(new Task2(), args);
    }

    @Override
    public int run(String[] args) throws Exception {
        Configuration conf = this.getConf();
        DistributedCache.addCacheFile(new URI("/data/matrix/C/matrix_20000x50.csv"), conf);
        // DistributedCache.addCacheFile(new URI("/data/matrix/C"),job.getConfiguration());
//        FileSystem fs = FileSystem.get(conf);
//        Path hdfsPath = new Path("/data/matrix/C/matrix_20000x50.csv");
//        DistributedCache.addCacheFile(hdfsPath.toUri(), conf);
//        DistributedCache.addCacheFile(new URI(args[1]), conf);

        Job job = new Job(conf);

        job.setJarByClass(Task2.class);
        job.setMapperClass(TaskMapper.class);
        job.setReducerClass(TaskReducer.class);

        job.setInputFormatClass(TextInputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);
        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(Text.class);

        job.setNumReduceTasks(4);

        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));

        return job.waitForCompletion(true) ? 0 : -1;

    }

    public static class TaskMapper extends Mapper<LongWritable, Text, Text, Text> {

        @Override
        public void map(LongWritable offset, Text aItem, Context context)
                throws IOException, InterruptedException {
            String[] aItemSplited = aItem.toString().split("\\s*[,\\s]\\s*");
            context.write(new Text(aItemSplited[0]), new Text(aItemSplited[1]+" "+aItemSplited[2]));
        }

    }

    public static class TaskReducer extends Reducer<Text, Text, Text, Text> {
         
        private static int[][] c = new int[20000][50];
        @Override
        protected void setup(Context context) throws IOException, InterruptedException {
            for (int i=0;i<20000;i++) {
                for (int j=0;j<50;j++) {
                    c[i][j] = 0;
                }
            }
            URI[] cacheFiles = context.getCacheFiles();
            if(cacheFiles != null) {
                Path pt = new Path(cacheFiles[0]);
                FileSystem fs = FileSystem.get(new Configuration());
                BufferedReader br = new BufferedReader(new InputStreamReader(fs.open(pt)));
                String line;
                while ((line = br.readLine()) != null) {
                    String[] itemsB = line.split("\\s*[,\\s]\\s*");
                    c[Integer.parseInt(itemsB[0])][Integer.parseInt(itemsB[1])] = Integer.parseInt(itemsB[2]);
                }
            } else {
                System.err.println("No cache files");
            }

        }

        @Override
        protected void reduce(Text number, Iterable<Text> pairs, Context context)
                throws IOException,InterruptedException
        {
            int[] aRow = new int[20000];
            for (int i=0;i<20000;i++) {
                aRow[i] = 0;
            }
            for (Text pair : pairs) {
                try {
                    String[] parts = pair.toString().split("\\s+");
                    aRow[Integer.parseInt(parts[0])] = Integer.parseInt(parts[1]);
                }catch(Exception e){
                    System.err.println("Cant't parse: "+pairs);
                }
            }

            for(int i=0;i<50;i++) {
                int resNum = 0;
                for(int j=0;j<20000;j++) {
                    resNum = (resNum + aRow[j]*c[j][i]) % 97;
                }
                context.write(number, new Text(Integer.toString(i)+" "+Integer.toString(resNum)));
            }
            // String res = "";
            // for (int i=0;i<20000;i++) {
            //     res+=(" "+Integer.toString(a[i]));
            // }
            // context.write(number, new Text(res));

        }
    }
}

