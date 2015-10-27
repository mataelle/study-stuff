import org.apache.hadoop.hive.ql.exec.UDF;
import org.apache.hadoop.io.Text;
import java.lang.StringBuilder;

public class reversed extends UDF {
		public Text evaluate(final Text s) {
			if( s == null) {
				return null;
			}
			else return new Text(new StringBuilder(s.toString()).reverse().toString());
		}	
}
