namespace PickDiff
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
            this.textBoxHost = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxPort = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxUser = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBoxPass = new System.Windows.Forms.TextBox();
            this.buttonConnect = new System.Windows.Forms.Button();
            this.comboBoxChooseDB1 = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.listBoxLogs = new System.Windows.Forms.ListBox();
            this.label6 = new System.Windows.Forms.Label();
            this.comboBoxChooseDB2 = new System.Windows.Forms.ComboBox();
            this.buttonMerge = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.textBoxMergeDBName = new System.Windows.Forms.TextBox();
            this.progressBarMerge = new System.Windows.Forms.ProgressBar();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.label8 = new System.Windows.Forms.Label();
            this.textBoxDB1ServerTag = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.textBoxDB2ServerTag = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // textBoxHost
            // 
            this.textBoxHost.Location = new System.Drawing.Point(90, 13);
            this.textBoxHost.Name = "textBoxHost";
            this.textBoxHost.Size = new System.Drawing.Size(154, 21);
            this.textBoxHost.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft YaHei UI Light", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(36, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(48, 21);
            this.label1.TabIndex = 1;
            this.label1.Text = "Host:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft YaHei UI Light", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.Location = new System.Drawing.Point(36, 40);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 21);
            this.label2.TabIndex = 3;
            this.label2.Text = "Port:";
            // 
            // textBoxPort
            // 
            this.textBoxPort.Location = new System.Drawing.Point(90, 40);
            this.textBoxPort.Name = "textBoxPort";
            this.textBoxPort.Size = new System.Drawing.Size(154, 21);
            this.textBoxPort.TabIndex = 2;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft YaHei UI Light", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label3.Location = new System.Drawing.Point(36, 67);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(47, 21);
            this.label3.TabIndex = 5;
            this.label3.Text = "User:";
            // 
            // textBoxUser
            // 
            this.textBoxUser.Location = new System.Drawing.Point(90, 67);
            this.textBoxUser.Name = "textBoxUser";
            this.textBoxUser.Size = new System.Drawing.Size(154, 21);
            this.textBoxUser.TabIndex = 4;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft YaHei UI Light", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label4.Location = new System.Drawing.Point(36, 94);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(46, 21);
            this.label4.TabIndex = 7;
            this.label4.Text = "Pass:";
            // 
            // textBoxPass
            // 
            this.textBoxPass.Location = new System.Drawing.Point(90, 94);
            this.textBoxPass.Name = "textBoxPass";
            this.textBoxPass.Size = new System.Drawing.Size(154, 21);
            this.textBoxPass.TabIndex = 6;
            // 
            // buttonConnect
            // 
            this.buttonConnect.Location = new System.Drawing.Point(90, 121);
            this.buttonConnect.Name = "buttonConnect";
            this.buttonConnect.Size = new System.Drawing.Size(154, 23);
            this.buttonConnect.TabIndex = 8;
            this.buttonConnect.Text = "Connect";
            this.buttonConnect.UseVisualStyleBackColor = true;
            this.buttonConnect.Click += new System.EventHandler(this.buttonConnect_Click);
            // 
            // comboBoxChooseDB1
            // 
            this.comboBoxChooseDB1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxChooseDB1.FormattingEnabled = true;
            this.comboBoxChooseDB1.Location = new System.Drawing.Point(90, 168);
            this.comboBoxChooseDB1.Name = "comboBoxChooseDB1";
            this.comboBoxChooseDB1.Size = new System.Drawing.Size(154, 20);
            this.comboBoxChooseDB1.TabIndex = 9;
            this.comboBoxChooseDB1.SelectedIndexChanged += new System.EventHandler(this.comboBoxChooseDB1_SelectedIndexChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft YaHei UI Light", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label5.Location = new System.Drawing.Point(38, 168);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(41, 21);
            this.label5.TabIndex = 10;
            this.label5.Text = "DB1:";
            // 
            // listBoxLogs
            // 
            this.listBoxLogs.ItemHeight = 12;
            this.listBoxLogs.Items.AddRange(new object[] {
            "日志列表",
            "按键盘的 C 复制选中行"});
            this.listBoxLogs.Location = new System.Drawing.Point(250, 13);
            this.listBoxLogs.Name = "listBoxLogs";
            this.listBoxLogs.Size = new System.Drawing.Size(649, 352);
            this.listBoxLogs.TabIndex = 12;
            this.listBoxLogs.KeyDown += new System.Windows.Forms.KeyEventHandler(this.listBoxLogs_KeyDown);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft YaHei UI Light", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label6.Location = new System.Drawing.Point(38, 242);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(44, 21);
            this.label6.TabIndex = 14;
            this.label6.Text = "DB2:";
            // 
            // comboBoxChooseDB2
            // 
            this.comboBoxChooseDB2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxChooseDB2.FormattingEnabled = true;
            this.comboBoxChooseDB2.Location = new System.Drawing.Point(90, 242);
            this.comboBoxChooseDB2.Name = "comboBoxChooseDB2";
            this.comboBoxChooseDB2.Size = new System.Drawing.Size(154, 20);
            this.comboBoxChooseDB2.TabIndex = 13;
            this.comboBoxChooseDB2.SelectedIndexChanged += new System.EventHandler(this.comboBoxChooseDB2_SelectedIndexChanged);
            // 
            // buttonMerge
            // 
            this.buttonMerge.Location = new System.Drawing.Point(90, 342);
            this.buttonMerge.Name = "buttonMerge";
            this.buttonMerge.Size = new System.Drawing.Size(154, 23);
            this.buttonMerge.TabIndex = 15;
            this.buttonMerge.Text = "Merge";
            this.buttonMerge.UseVisualStyleBackColor = true;
            this.buttonMerge.Click += new System.EventHandler(this.buttonMerge_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft YaHei UI Light", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label7.Location = new System.Drawing.Point(1, 308);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(83, 21);
            this.label7.TabIndex = 17;
            this.label7.Text = "MergeDB:";
            // 
            // textBoxMergeDBName
            // 
            this.textBoxMergeDBName.Location = new System.Drawing.Point(90, 308);
            this.textBoxMergeDBName.Name = "textBoxMergeDBName";
            this.textBoxMergeDBName.Size = new System.Drawing.Size(154, 21);
            this.textBoxMergeDBName.TabIndex = 16;
            // 
            // progressBarMerge
            // 
            this.progressBarMerge.Location = new System.Drawing.Point(5, 371);
            this.progressBarMerge.Name = "progressBarMerge";
            this.progressBarMerge.Size = new System.Drawing.Size(894, 13);
            this.progressBarMerge.TabIndex = 18;
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 1;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Microsoft YaHei UI Light", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label8.Location = new System.Drawing.Point(6, 196);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(73, 21);
            this.label8.TabIndex = 20;
            this.label8.Text = "DB1前缀:";
            // 
            // textBoxDB1ServerTag
            // 
            this.textBoxDB1ServerTag.Location = new System.Drawing.Point(90, 196);
            this.textBoxDB1ServerTag.Name = "textBoxDB1ServerTag";
            this.textBoxDB1ServerTag.Size = new System.Drawing.Size(154, 21);
            this.textBoxDB1ServerTag.TabIndex = 19;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Microsoft YaHei UI Light", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label10.Location = new System.Drawing.Point(6, 269);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(76, 21);
            this.label10.TabIndex = 24;
            this.label10.Text = "DB2前缀:";
            // 
            // textBoxDB2ServerTag
            // 
            this.textBoxDB2ServerTag.Location = new System.Drawing.Point(90, 269);
            this.textBoxDB2ServerTag.Name = "textBoxDB2ServerTag";
            this.textBoxDB2ServerTag.Size = new System.Drawing.Size(154, 21);
            this.textBoxDB2ServerTag.TabIndex = 23;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(904, 389);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.textBoxDB2ServerTag);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.textBoxDB1ServerTag);
            this.Controls.Add(this.progressBarMerge);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.textBoxMergeDBName);
            this.Controls.Add(this.buttonMerge);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.comboBoxChooseDB2);
            this.Controls.Add(this.listBoxLogs);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.comboBoxChooseDB1);
            this.Controls.Add(this.buttonConnect);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.textBoxPass);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.textBoxUser);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.textBoxPort);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBoxHost);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "MysqlMerge";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxHost;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxPort;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxUser;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBoxPass;
        private System.Windows.Forms.Button buttonConnect;
        private System.Windows.Forms.ComboBox comboBoxChooseDB1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ListBox listBoxLogs;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.ComboBox comboBoxChooseDB2;
        private System.Windows.Forms.Button buttonMerge;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox textBoxMergeDBName;
        private System.Windows.Forms.ProgressBar progressBarMerge;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox textBoxDB1ServerTag;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.TextBox textBoxDB2ServerTag;
    }
}

