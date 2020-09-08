using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Threading;
using System.Diagnostics;

namespace FileListSortPrint
{
    public partial class Form1 : Form
    {
        string chooseDirPath = string.Empty;
        Dictionary<string, FileInfo> keyValuePairs = new Dictionary<string, FileInfo>();

        string logStr = string.Empty;

        bool working = false;

        Queue<string> logQueue = new Queue<string>();

        int progress = 0;

        long startTimeStamp = 0;

        enum SortAlgorithm
        {
            /// <summary>
            /// 冒泡
            /// </summary>
            BubbleSort,
            /// <summary>
            /// 计数排序
            /// </summary>
            CountingSort
        }
        SortAlgorithm sortAlgorithm =SortAlgorithm.BubbleSort ;//冒泡

        public Form1()
        {
            InitializeComponent();
        }

        private FileInfo GetFileInfo(Dictionary<string, FileInfo> keyValuePairs,string filePath)
        {
            FileInfo fileInfo = null;
            if (keyValuePairs.ContainsKey(filePath) == false)
            {
                fileInfo = new FileInfo(filePath);
                keyValuePairs.Add(filePath, fileInfo);
            }
            else
            {
                fileInfo = keyValuePairs[filePath];
            }
            return fileInfo;
        }

        private void Log(string str)
        {
            logQueue.Enqueue(str);
            //Console.Write(str);
        }

        private long GetUnixTimeStamp()
        {
            return (DateTime.Now.ToUniversalTime().Ticks - 621355968000000000) / 10000000;
        }

        /// <summary>
        /// 冒泡排序
        /// </summary>
        /// <param name="files"></param>
        private void BubbleSort(string[] files)
        {
            Log("BubbleSort");
            int lastpercent = -1;

            for (int i = 0; i < files.Length; i++)
            {
                for (int j = 0; j < files.Length - i - 1; j++)//每次从左边开始，一次内循环挑出最大的放到最右边
                {

                    FileInfo fileInfo = GetFileInfo(keyValuePairs, files[j]);
                    FileInfo fileInfoRight = GetFileInfo(keyValuePairs, files[j + 1]);

                    if (fileInfo.Length > fileInfoRight.Length)
                    {
                        string tmp = files[j + 1];
                        files[j + 1] = files[j];
                        files[j] = tmp;
                    }
                }

                int percent = i * 100 / files.Length;

                progress = percent;

                if (percent!=lastpercent && percent%5==0)
                {
                    Log("BubbleSort: " + i + "/" + files.Length+" 进度:"+ percent+"%");

                    lastpercent = percent;
                }
            }
        }


        /// <summary>
        /// 计数排序
        /// </summary>
        /// <param name="files"></param>
        private void CountingSort(string[] files)
        {
            Log("CountingSort");
            int lastpercent = -1;

            long maxFileSize = 0;

            //以文件大小作为key 相同大小的文件名 存放在value的list里面。
            Dictionary<long, List<string>> fileSize_fileNames = new Dictionary<long, List<string>>();

            //先遍历一次，找出最大的文件,并且以文件大小作为key，进行分类。
            for (int i = 0; i < files.Length; i++)
            {
                string filePath = files[i];
                FileInfo fileInfo = new FileInfo(filePath);
                long fileSize = fileInfo.Length;

                List<string> fileNames = null;
                if(fileSize_fileNames.ContainsKey(fileSize)==false)
                {
                    fileNames = new List<string>();
                    fileSize_fileNames.Add(fileSize, fileNames);

                    if(maxFileSize<fileSize)
                    {
                        maxFileSize = fileSize;
                    }
                }
                else
                {
                    fileNames = fileSize_fileNames[fileSize];
                }
                fileNames.Add(filePath);

                //更新进度
                int percent = i * 100 / files.Length;

                progress = percent;

                if (percent != lastpercent && percent % 5 == 0)
                {
                    Log("CountingSort 统计文件字节大小: " + i + "/" + files.Length + " 进度:" + percent + "%");

                    lastpercent = percent;
                } 
            }

            //以最大的文件尺寸，创建数组
            int[] coutingArray = new int[maxFileSize+1];

            //填入计数
            foreach (var item in fileSize_fileNames)
            {
                coutingArray[item.Key] = item.Value.Count;

            }

            //coutingArray已经是有序的了。现在只要填入files就可以。
            int fileIndex = 0;
            for (int i = 0; i < coutingArray.Length; i++)
            {
                int fileSize = i;

                if(fileSize_fileNames.ContainsKey(fileSize))
                {
                    List<string> fileNames = fileSize_fileNames[fileSize];

                    foreach (var fileName in fileNames)
                    {
                        files[fileIndex++] = fileName;
                    }
                }

                

                //更新进度
                int percent = i * 100 / coutingArray.Length;

                progress = percent;

                if (percent != lastpercent && percent % 5 == 0)
                {
                    Log("CountingSort 填充: " + i + "/" + files.Length + " 进度:" + percent + "%");

                    lastpercent = percent;
                }
            }
        }


        /// <summary>
        /// 对目录文件进行排序
        /// </summary>
        /// <param name="state"></param>
        private void SortFiles(Object state)
        {
            string dirPath = (string)state;

            Log("搜索目录："+ dirPath);
            string[] files = Directory.GetFiles(dirPath, "*.*", SearchOption.AllDirectories);

            Log("文件数量：" + files.Length);

            startTimeStamp = GetUnixTimeStamp();

            //排序
            switch (sortAlgorithm)
            {
                case SortAlgorithm.BubbleSort:
                    BubbleSort(files);
                    break;
                case SortAlgorithm.CountingSort:
                    CountingSort(files);
                    break;
                default:
                    break;
            }

            long costTime = GetUnixTimeStamp() - startTimeStamp;
            Log("排序耗时:" + costTime);

            //Log("写入txt");
            //保存到txt
            StreamWriter streamWriter = new StreamWriter("./files.txt");
            for (int i = files.Length - 1; i >= 0; i--)
            {
                FileInfo fileInfo = GetFileInfo(keyValuePairs, files[i]);

                streamWriter.WriteLine(fileInfo.Length + " " + files[i]);
            }
            streamWriter.Close();

            
            
            working = false;
            startTimeStamp = 0;

            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (working)
            {
                return;
            }
            FolderBrowserDialog dialog = new FolderBrowserDialog();
            dialog.Description = "请选择文件夹";

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                chooseDirPath = dialog.SelectedPath;

                textBox1.Text ="目标文件夹:"+ chooseDirPath;
            }
        }

        private void buttonSortStart_Click(object sender, EventArgs e)
        {
            if (working)
            {
                return;
            }
            if (string.IsNullOrEmpty(chooseDirPath))
            {
                MessageBox.Show("请先选择目录");
                return;
            }

            if(radioButtonBubbleSort.Checked)
            {
                sortAlgorithm = SortAlgorithm.BubbleSort;
            }
            else if(radioButtonCountingSort.Checked)
            {
                sortAlgorithm = SortAlgorithm.CountingSort;
            }

            working = true;

            Log("开启子线程执行");

            ThreadPool.QueueUserWorkItem(SortFiles, chooseDirPath);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if(logQueue.Count>0)
            {
                textBox1.Text = logQueue.Dequeue();
                //Console.WriteLine("timer1_Tick");
            }

            progressBar1.Value = progress;
        }

        private void radioButtonBubbleSort_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
