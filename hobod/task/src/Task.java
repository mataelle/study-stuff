package ru.mipt;

import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.util.ToolRunner;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.reduce.IntSumReducer;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import java.lang.InterruptedException;
import java.io.IOException;

public class Task extends Configured implements Tool{

    public static void main(String[] args) throws Exception {
        ToolRunner.run(new Task(), args);
    }

    @Override
    public int run(String[] args) throws Exception {
        Configuration conf = this.getConf();
        Job job = new Job(conf);
        job.setJarByClass(Task.class);
        job.setMapperClass(TaskMapper.class);
        job.setReducerClass(TaskReducer.class);

        job.setInputFormatClass(KeyValueTextInputFormat.class);
        job.setOutputFormatClass(TextOutputFormat.class);
        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(Text.class);

        job.setNumReduceTasks(4);

        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));

        return job.waitForCompletion(true) ? 0 : -1;
    }

    public static class TaskMapper extends Mapper<Text, Text, Text, Text> {
        private String currentPersona = "";
        private Integer wordsCount = 0;

        @Override
        public void map(Text personRaw, Text wordsRaw, Context context)
                throws IOException, InterruptedException
        {
            String persona = personRaw.toString();
            String[] words = wordsRaw.toString().split("[\\p{Punct}\\s ]+");
            if (currentPersona.equals(persona) || currentPersona.equals("")) {
                wordsCount += words.length;
                currentPersona = persona;
            } else {
                 context.write(new Text(currentPersona), new Text(wordsCount.toString() + " 1"));
                currentPersona = persona;
                wordsCount = words.length;
            }
        }
        @Override
        protected void cleanup(Context context)
                throws IOException, InterruptedException
        {
            context.write(new Text(currentPersona), new Text(new Integer(wordsCount).toString() + " 1"));
        }

    }

    public static class TaskReducer extends Reducer<Text, Text, Text, Text> {
        @Override
        protected void reduce(Text persona, Iterable<Text> numbersRaw, Context context)
                throws IOException,InterruptedException
        {
            Integer WordsCount = 0;
            Integer ReplicsCount = 0;
            for (Text value: numbersRaw) {
                String[] numbers = value.toString().split(" ");
                WordsCount += Integer.valueOf(numbers[0]);
                ReplicsCount += Integer.valueOf(numbers[1]);
//                context.write(persona, value);
            }
            String dataWrite = (WordsCount.toString()) + " " + (ReplicsCount.toString()) + " " + (new Double(WordsCount*1.0/ReplicsCount).toString());
            context.write(persona, new Text(dataWrite));
        }
    }
}

