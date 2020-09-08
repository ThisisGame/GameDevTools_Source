namespace FileListSortPrint
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.buttonChooseDir = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.buttonSortStart = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.radioButtonBubbleSort = new System.Windows.Forms.RadioButton();
            this.radioButtonCountingSort = new System.Windows.Forms.RadioButton();
            this.SuspendLayout();
            // 
            // buttonChooseDir
            // 
            this.buttonChooseDir.Location = new System.Drawing.Point(4, 142);
            this.buttonChooseDir.Name = "buttonChooseDir";
            this.buttonChooseDir.Size = new System.Drawing.Size(152, 41);
            this.buttonChooseDir.TabIndex = 0;
            this.buttonChooseDir.Text = "选择文件夹";
            this.buttonChooseDir.UseVisualStyleBackColor = true;
            this.buttonChooseDir.Click += new System.EventHandler(this.button1_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(4, 12);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(317, 72);
            this.textBox1.TabIndex = 2;
            this.textBox1.Text = "点击选择文件夹，生成选定目录文件列表，并按文件大小排序，从大到小。";
            // 
            // buttonSortStart
            // 
            this.buttonSortStart.Location = new System.Drawing.Point(162, 142);
            this.buttonSortStart.Name = "buttonSortStart";
            this.buttonSortStart.Size = new System.Drawing.Size(159, 41);
            this.buttonSortStart.TabIndex = 3;
            this.buttonSortStart.Text = "开始";
            this.buttonSortStart.UseVisualStyleBackColor = true;
            this.buttonSortStart.Click += new System.EventHandler(this.buttonSortStart_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 1;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // progressBar1
            // 
            this.progressBar1.BackColor = System.Drawing.SystemColors.MenuHighlight;
            this.progressBar1.Location = new System.Drawing.Point(4, 189);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(317, 23);
            this.progressBar1.TabIndex = 4;
            // 
            // radioButtonBubbleSort
            // 
            this.radioButtonBubbleSort.AutoSize = true;
            this.radioButtonBubbleSort.Checked = true;
            this.radioButtonBubbleSort.Location = new System.Drawing.Point(4, 90);
            this.radioButtonBubbleSort.Name = "radioButtonBubbleSort";
            this.radioButtonBubbleSort.Size = new System.Drawing.Size(191, 16);
            this.radioButtonBubbleSort.TabIndex = 5;
            this.radioButtonBubbleSort.TabStop = true;
            this.radioButtonBubbleSort.Text = "冒泡排序(超慢但占用最少内存)";
            this.radioButtonBubbleSort.UseVisualStyleBackColor = true;
            this.radioButtonBubbleSort.CheckedChanged += new System.EventHandler(this.radioButtonBubbleSort_CheckedChanged);
            // 
            // radioButtonCountingSort
            // 
            this.radioButtonCountingSort.AutoSize = true;
            this.radioButtonCountingSort.Location = new System.Drawing.Point(4, 112);
            this.radioButtonCountingSort.Name = "radioButtonCountingSort";
            this.radioButtonCountingSort.Size = new System.Drawing.Size(317, 16);
            this.radioButtonCountingSort.TabIndex = 6;
            this.radioButtonCountingSort.Text = "计数排序(极速但占用内存，消耗内存=最大文件字节数)";
            this.radioButtonCountingSort.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(325, 220);
            this.Controls.Add(this.radioButtonCountingSort);
            this.Controls.Add(this.radioButtonBubbleSort);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.buttonSortStart);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.buttonChooseDir);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "文件列表排序";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonChooseDir;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button buttonSortStart;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.RadioButton radioButtonBubbleSort;
        private System.Windows.Forms.RadioButton radioButtonCountingSort;
    }
}

