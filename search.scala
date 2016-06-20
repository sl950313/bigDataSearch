
import org.apache.spark.{ SparkConf, SparkContext }
import org.apache.spark.sql.Row;
import org.apache.spark.sql.types.{ StructType, StructField, StringType };

import java.io._

object search {

  def write(line: String, pw: PrintWriter ) {
    if (line.length == 0) {
      return ;
    }
    pw.write(line + "\n");
  }

  def main(args: Array[String]) {
    if (args.length != 2) {
      println("error args");
      return ;
    }
    
    var sqlQuery = args(0);
    var sconf = new SparkConf().setAppName("search")
      .setMaster("spark://114.214.169.173:7077")
      .set("spark.executor.memory", "1g")
      .set("spark.serializer", "org.apache.spark.serializer.KryoSerializer")
    var sc = new SparkContext(sconf);
    var sqlContext = new org.apache.spark.sql.SQLContext(sc);
    var stuInfo = sc.textFile("hdfs://localhost:8030/stu_info/test_data_open_t").map(_.split("\t"));
    println("stu_info.count() = " + stuInfo.count());

    val schemaString = "USERID UNIXTIME UNIXTIME_END LOCATION LOCATION_DETAIL LATITUDE LONGITUDE _FROM";
    var stuInfoSchema =
      StructType(
        schemaString.split(" ").map(fieldName => StructField(fieldName, StringType, true)));
    var stuInfoRDD = stuInfo.map(p => Row(p(0), p(1).trim, p(2).trim, p(3), p(4), p(5).trim, p(6).trim, p(7)));
    var stuInfoDF = sqlContext.createDataFrame(stuInfoRDD, stuInfoSchema);
    stuInfoDF.registerTempTable("STU_INFO");

    /*
    var tmpFile = new PrintWriter(new File("/tmp/scala.tmp"));
    */

    var excute = new excute;
    while (1) {
      val msgInfo = excute.readMsgFromMsgQ();
      println(msgInfo);

      /*
      var results = sqlContext.sql(sqlQuery);
      var tmpRes :String = "";
      results.collect().foreach(a => function(a) {
            tmpRes += a + "\n";
         });
      excute.writeToshmAndRecall(tmpRes); 
      */
    }
    //tmpFile.close();
    
    //sqlContext.udf.register("UserID", (x:String) => getUserID(x));
    //var result = sqlContext.sql("select ");
    //val stuinfoSchema = sqlContext.createDataFrame(stu_info, beanClass)
  }
}
