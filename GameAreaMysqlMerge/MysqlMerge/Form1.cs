using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Newtonsoft.Json;
using MySql.Data.MySqlClient;
using System.Threading;
using Newtonsoft.Json;
using Renci.SshNet.Common;

namespace PickDiff
{
    public partial class Form1 : Form
    {

        private class Config
        {
            public class ConflictTable
            {
                public string tableName;
                public string[] columns;
                /// <summary>
                /// 解决方式 1.AddServerTag  2.UseLast 仅复制最后一个数据库的 3.AutoIncrReset 自增的字段在之前的基础上叠加
                /// </summary>
                public string resolveType;
            }

            public string Host;
            public int Port;
            public string User;
            public string Pass;

            public string DB1;
            public string DB1Prefix;
            public string DB2;
            public string DB2Prefix;
            public string MergeDBName;
            public List<ConflictTable> conflictTables=new List<ConflictTable>();
        }

        private JsonConfigParser<Config> mJsonConfigParser = new JsonConfigParser<Config>();

        private string mDBName1;
        private string mDBName2;

        private int mProgressValue = 0;

        private Queue<string> logQueue = new Queue<string>();

        /// <summary>
        /// 数据库对应的前缀 例如1_gamedb  2_gamedb合并的时候某个表有冲突，那么就在数据前面添加  "S1."  这样的前缀。
        /// </summary>
        private Dictionary<string, string> mDBServerTag = new Dictionary<string, string>();

        /// <summary>
        /// 临时表前缀
        /// </summary>
        private string TmpTablePrefix = "tmp_";

        public Form1()
        {
            InitializeComponent();


            mJsonConfigParser.Load("./config.json");
            textBoxHost.Text = mJsonConfigParser.instance.Host;
            textBoxPort.Text = mJsonConfigParser.instance.Port.ToString();
            textBoxUser.Text = mJsonConfigParser.instance.User;
            textBoxPass.Text = mJsonConfigParser.instance.Pass;

            textBoxDB1ServerTag.Text = mJsonConfigParser.instance.DB1Prefix;
            textBoxDB2ServerTag.Text = mJsonConfigParser.instance.DB2Prefix;
            textBoxMergeDBName.Text = mJsonConfigParser.instance.MergeDBName;
        }


        private void SaveConfig()
        {
            mJsonConfigParser.instance.Host = textBoxHost.Text;
            mJsonConfigParser.instance.Port=int.Parse(textBoxPort.Text);
            mJsonConfigParser.instance.User=textBoxUser.Text;
            mJsonConfigParser.instance.Pass=textBoxPass.Text;

            mJsonConfigParser.instance.DB1Prefix= textBoxDB1ServerTag.Text;
            mJsonConfigParser.instance.DB2Prefix=textBoxDB2ServerTag.Text;
            mJsonConfigParser.instance.MergeDBName=textBoxMergeDBName.Text;

            mJsonConfigParser.Save();
        }
 
        public class MyTable
        {
            public string Table { get; set; }
            public List<string> ColumnList { get; set; } = new List<string>();
        }


        private void buttonConnect_Click(object sender, EventArgs e)
        {
            SaveConfig();

            MySqlConnection conn = CreteConnection();
            try
            {
                conn.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
                return;
            }
            

            string newTableCMD = "SHOW DATABASES;";
            MySqlCommand cmd = CreateMySqlCommand(newTableCMD, conn);
            using (MySqlDataReader reader = cmd.ExecuteReader())
            {
                while (reader.Read())
                {
                    string dbName = reader[0].ToString();
                    comboBoxChooseDB1.Items.Add(dbName);
                    comboBoxChooseDB2.Items.Add(dbName);
                }
            }

            conn.Close();

            MessageBox.Show("success connect!");
        }

        private void comboBoxChooseDB1_SelectedIndexChanged(object sender, EventArgs e)
        {
            mDBName1= comboBoxChooseDB1.SelectedItem.ToString();
        }

        /// <summary>
        /// 更新进度条进度
        /// </summary>
        /// <param name="current">当前 从0开始</param>
        /// <param name="total">总的</param>
        private void UpdateProgressValue(int current,int total)
        {
            mProgressValue = (current+1)*100/total;
        }

        private void comboBoxChooseDB2_SelectedIndexChanged(object sender, EventArgs e)
        {
            mDBName2 = comboBoxChooseDB2.SelectedItem.ToString();
        }

        private void buttonMerge_Click(object sender, EventArgs e)
        {
            SaveConfig();

            //收集数据库前缀
            mDBServerTag.Add(mDBName1, textBoxDB1ServerTag.Text);
            mDBServerTag.Add(mDBName2, textBoxDB2ServerTag.Text);

            ThreadPool.QueueUserWorkItem(MergeInThread);
        }

        private void MergeInThread(Object state)
        {
            bool bRet = CreateMergeDB();

            if (bRet)
            {
                if (MessageBox.Show("创建数据库成功，点击继续复制表结构", "选择操作", MessageBoxButtons.OKCancel) == DialogResult.Cancel)
                {
                    return;
                }
                bRet = CreateAllTable();
            }

            if (bRet)
            {
                if (MessageBox.Show("复制表结构完成，点击继续复制表数据", "选择操作", MessageBoxButtons.OKCancel) == DialogResult.Cancel)
                {
                    return;
                }
                bRet = CopyAllTableData();
            }

            if (bRet)
            {
                if (MessageBox.Show("复制表数据完成", "选择操作", MessageBoxButtons.OKCancel) == DialogResult.Cancel)
                {
                    return;
                }
            }
        }

        private MySqlCommand CreateMySqlCommand(string cmdStr,MySqlConnection conn)
        {
            Log(cmdStr);
            return new MySqlCommand(cmdStr, conn);
        }

        /// <summary>
        /// 执行查询
        /// </summary>
        /// <param name="conn"></param>
        /// <param name="cmdStr"></param>
        /// <returns></returns>
        private bool ExecuteNonQuery(MySqlConnection conn, string cmdStr)
        {
            MySqlCommand cmd = CreateMySqlCommand(cmdStr, conn);

            try
            {
                //执行语句
                int res = cmd.ExecuteNonQuery();
                return true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(cmdStr + Environment.NewLine + ex.ToString());
                conn.Close();
                return false;
            }
        }

        /// <summary>
        /// 创建合服数据库
        /// </summary>
        private bool CreateMergeDB()
        {
            string mergeDBName = textBoxMergeDBName.Text;

            MySqlConnection conn = CreteConnection();

            //创建合服数据库
            string cmdStr = "CREATE DATABASE `" + mergeDBName + "` CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;";

            try
            {
                conn.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
                return false;
            }

            if (ExecuteNonQuery(conn, cmdStr)==false)
            {
                return false;
            }
            conn.Close();
            return true;
        }

        /// <summary>
        /// 创建连接
        /// </summary>
        /// <returns></returns>
        private MySqlConnection CreteConnection()
        {
            string connstr = "data source="+ mJsonConfigParser.instance.Host + ";user id=" + mJsonConfigParser.instance.User + ";password=" + mJsonConfigParser.instance.Pass + ";pooling=false;charset=utf8";//pooling代表是否使用连接池
            MySqlConnection conn = new MySqlConnection(connstr);
            return conn;
        }

        /// <summary>
        /// 创建表并复制表结构
        /// </summary>
        /// <param name="conn"></param>
        /// <param name="mergeDBName"></param>
        /// <param name="tableNameStr"></param>
        /// <param name="dbName"></param>
        /// <returns></returns>
        private bool CloneTableStruct(MySqlConnection conn, string mergeDBName, string tableNameStr,string dbName,bool temp=false)
        {
            //创建表并复制表结构
            string cmdStr = "";
            if(temp)
            {
                cmdStr = "create TEMPORARY table " + mergeDBName + "."+TmpTablePrefix + tableNameStr + " like " + dbName + "." + tableNameStr + ";";
            }
            else
            {
                cmdStr = "create table " + mergeDBName + "." + tableNameStr + " like " + dbName + "." + tableNameStr + ";";
            }

            return ExecuteNonQuery(conn, cmdStr);
        }

        /// <summary>
        /// 创建所有表
        /// </summary>
        /// <returns></returns>
        private bool CreateAllTable()
        {
            Dictionary<string, MyTable> tableStructDic = GetAllTableSruct(mDBName1);//这里用DBName1 和 DBName2都可以，既然是合服，那这两个服的db结构肯定是一样的。

            string mergeDBName = textBoxMergeDBName.Text;

            MySqlConnection conn = CreteConnection();

            try
            {
                conn.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
                return false;
            }

            List<string> tableNameList = tableStructDic.Keys.ToList();
            for (int tableNameIndex = 0; tableNameIndex < tableNameList.Count; tableNameIndex++)
            {
                UpdateProgressValue(tableNameIndex, tableNameList.Count);

                string tableNameStr = tableNameList[tableNameIndex];

                //创建表并复制表结构
                if (CloneTableStruct(conn, mergeDBName, tableNameStr, mDBName1)==false)
                {
                    return false;
                }
            }
            conn.Close();
            return true;
        }

        /// <summary>
        /// 检查表是否有冲突字段
        /// </summary>
        /// <param name="tableName"></param>
        /// <returns></returns>
        private Config.ConflictTable CheckTableConflict(string tableName)
        {
            var confictTables = mJsonConfigParser.instance.conflictTables;
            for (int i = 0; i < confictTables.Count; i++)
            {
                if(confictTables[i].tableName==tableName)
                {
                    return confictTables[i];
                }
            }
            return null;
        }


        /// <summary>
        /// 复制一个表的数据
        /// </summary>
        /// <param name="conn"></param>
        /// <param name="targetDBName"></param>
        /// <param name="targetTableName"></param>
        /// <param name="srcDBName"></param>
        /// <returns></returns>
        private bool CopyTableData(MySqlConnection conn,string targetDBName,string targetTableName,string srcDBName,string srcTableName)
        {
            string cmdStr = "INSERT INTO " + targetDBName +"."+ targetTableName + " SELECT * FROM " + srcDBName + "." + srcTableName + ";";
            return ExecuteNonQuery(conn, cmdStr);
        }

        /// <summary>
        /// 在临时表中，对字段添加前缀
        /// </summary>
        /// <param name="conn"></param>
        /// <param name="dbName"></param>
        /// <param name="tableName"></param>
        /// <param name="dbName"></param>
        /// <returns></returns>
        private bool AddServerTagInTmpTable(MySqlConnection conn, string dbName, string tableName,string columnName,string serverTag)
        {
            string cmdStr = "update "+ dbName + "."+ tableName+" set "+ columnName +"= CONCAT('"+ serverTag+"',"+ columnName+"); ";
            return ExecuteNonQuery(conn, cmdStr);
        }

        /// <summary>
        /// 删除表
        /// </summary>
        /// <param name="conn"></param>
        /// <param name="dbName"></param>
        /// <param name="tableName"></param>
        /// <returns></returns>
        private bool DropTable(MySqlConnection conn, string dbName, string tableName)
        {
            string cmdStr = "drop table "+ dbName + "."+ tableName + ";";
            return ExecuteNonQuery(conn, cmdStr);
        }

        /// <summary>
        /// 获取自增字段当前最大值
        /// </summary>
        /// <returns></returns>
        private bool GetTableColumnIncrMaxValue(MySqlConnection conn,string dbName,string tableName,string columnName,ref long max)
        {
            string cmdStr = "select max("+columnName+") as ret from "+ dbName + "."+tableName+";";
            MySqlCommand cmd = CreateMySqlCommand(cmdStr, conn);

            try
            {
                //执行语句
                MySqlDataReader reader = cmd.ExecuteReader();
               
                while (reader.Read())
                {
                    string s = reader[0].ToString();
                    long maxValue = 0;
                    
                    long.TryParse(s,out maxValue);

                    max = maxValue;
                }
                reader.Close();
                return true;
            }
            catch (Exception ex)
            {
                MessageBox.Show(cmdStr + Environment.NewLine + ex.ToString());
                conn.Close();
                return false;
            }
        }

        /// <summary>
        /// 对表的字段 增加一个offset
        /// </summary>
        /// <param name="conn"></param>
        /// <param name="dbName"></param>
        /// <param name="tableName"></param>
        /// <param name="columnName"></param>
        /// <param name="offset"></param>
        /// <returns></returns>
        private bool ColumnAddOffset(MySqlConnection conn, string dbName, string tableName, string columnName,  long offset)
        {
            string cmdStr = "update "+ dbName + "." + tableName + " set " + columnName+ "=" + columnName + "+" + offset + ";";

            return ExecuteNonQuery(conn, cmdStr);
        }

        /// <summary>
        /// 复制所有表数据
        /// </summary>
        /// <returns></returns>
        private bool CopyAllTableData()
        {
            Dictionary<string, MyTable> tableStructDic = GetAllTableSruct(mDBName1);//这里用DBName1 和 DBName2都可以，既然是合服，那这两个服的db结构肯定是一样的。

            string mergeDBName = textBoxMergeDBName.Text;

            MySqlConnection conn = CreteConnection();
            try
            {
                conn.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
                return false;
            }

            List<string> tableNameList = tableStructDic.Keys.ToList();

            //按顺序对2个DB执行以下操作
            List<string> dbNameList = mDBServerTag.Keys.ToList();
            for (int dbNameIndex = 0; dbNameIndex < dbNameList.Count; dbNameIndex++)
            {
                string dbName = dbNameList[dbNameIndex];
                string serverTag = mDBServerTag[dbName];

                for (int tableNameIndex = 0; tableNameIndex < tableNameList.Count; tableNameIndex++)
                {
                    UpdateProgressValue(tableNameIndex, tableNameList.Count);

                    string tableName = tableNameList[tableNameIndex];
                    string mergeDBTmpTableName = TmpTablePrefix + tableName;

                    Config.ConflictTable conflictTable = CheckTableConflict(tableName);

                    //1.创建临时表
                    if (CloneTableStruct(conn, mergeDBName, tableName, dbName, true)==false)
                    {
                        return false;
                    }


                    if (conflictTable == null)
                    {
                        //2.复制数据到临时表
                        if (CopyTableData(conn, mergeDBName, mergeDBTmpTableName, dbName, tableName) == false)
                        {
                            return false;
                        }
                    }
                    else
                    {
                        //对于会冲突的表,根据配置的处理方式进行处理
                        switch (conflictTable.resolveType)
                        {
                            case "AddServerTag":
                                {
                                    //2.复制数据到临时表
                                    if (CopyTableData(conn, mergeDBName, mergeDBTmpTableName, dbName, tableName) == false)
                                    {
                                        return false;
                                    }

                                    //3.处理冲突 冲突字段添加区服前缀
                                    if (AddServerTagInTmpTable(conn, mergeDBName, mergeDBTmpTableName, conflictTable.columns[0], serverTag) == false)
                                    {
                                        return false;
                                    }
                                }
                                break;
                            case "UseLast":
                                {
                                    //2.复制数据到临时表
                                    //3.处理冲突 仅复制最后一个数据库的
                                    if(dbNameIndex==mDBServerTag.Count-1)
                                    {
                                        if (CopyTableData(conn, mergeDBName, mergeDBTmpTableName, dbName, tableName) == false)
                                        {
                                            return false;
                                        }
                                    }
                                }
                                break;
                            case "AutoIncrReset":
                                {
                                    //2.复制数据到临时表
                                    if (CopyTableData(conn, mergeDBName, mergeDBTmpTableName, dbName, tableName) == false)
                                    {
                                        return false;
                                    }

                                    //3.处理冲突 先获取自增字段当前最大值，然后将临时表对应都加上这个最大值。
                                    long maxValueOfTableColumn = 0;
                                    if(GetTableColumnIncrMaxValue(conn,mergeDBName,tableName,conflictTable.columns[0],ref maxValueOfTableColumn)==false)
                                    {
                                        return false;
                                    }

                                    if(ColumnAddOffset(conn, mergeDBName, mergeDBTmpTableName, conflictTable.columns[0], maxValueOfTableColumn)==false)
                                    {
                                        return false;
                                    }
                                }
                                break;
                            default:
                                break;
                        }
                    }

                    //4.复制临时表数据到正式表
                    if(CopyTableData(conn, mergeDBName, tableName, mergeDBName, mergeDBTmpTableName)==false)
                    {
                        return false;
                    }
                    //5.删除临时表
                    if(DropTable(conn,mergeDBName,mergeDBTmpTableName)==false)
                    {
                        return false;
                    }
                }
            }
            conn.Close();
            return true;
        }

        /// <summary>
        /// 获取所有表名以及字段
        /// </summary>
        /// <param name="dbName"></param>
        /// <returns></returns>
        private Dictionary<string, MyTable> GetAllTableSruct(string dbName)
        {
            MySqlConnection conn = CreteConnection();
            conn.Open();

            //获取所有table以及对应的字段名。
            Dictionary<string, MyTable> dic = new Dictionary<string, MyTable>();
            string newTableCMD = "select table_name, column_name from information_schema.columns where table_schema = '" + dbName + "';";
            MySqlCommand cmd = CreateMySqlCommand(newTableCMD, conn);
            using (MySqlDataReader reader = cmd.ExecuteReader())
            {
                while (reader.Read())
                {
                    string table = reader.GetString("table_name");
                    string column = reader.GetString("column_name");
                    if (dic.ContainsKey(table))
                    {
                        dic[table].ColumnList.Add(column);
                    }
                    else
                    {
                        MyTable t = new MyTable();
                        t.Table = table;
                        t.ColumnList.Add(column);
                        dic.Add(t.Table, t);
                    }
                }
            }
            conn.Close();

            return dic;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            progressBarMerge.Value = mProgressValue;

            if(logQueue.Count>0)
            {
                string log = logQueue.Dequeue();
                listBoxLogs.Items.Add(log);
            }

        }
    
        private void Log(string str)
        {
            logQueue.Enqueue(str);
        }

        /// <summary>
        /// 日志列表按 C 复制选中行
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void listBoxLogs_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyData == Keys.C)
            {
                Clipboard.SetDataObject(listBoxLogs.SelectedItem.ToString());
            }
        }
    }
}
