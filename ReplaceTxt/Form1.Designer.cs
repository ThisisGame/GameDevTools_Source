
namespace ReplaceTxt
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
            this.textBoxSrc = new System.Windows.Forms.TextBox();
            this.buttonReplace = new System.Windows.Forms.Button();
            this.textBoxDst = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBoxStart = new System.Windows.Forms.TextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBoxEnd = new System.Windows.Forms.TextBox();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.textBoxReplace = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // textBoxSrc
            // 
            this.textBoxSrc.Location = new System.Drawing.Point(12, 12);
            this.textBoxSrc.MaxLength = 0;
            this.textBoxSrc.Multiline = true;
            this.textBoxSrc.Name = "textBoxSrc";
            this.textBoxSrc.Size = new System.Drawing.Size(920, 323);
            this.textBoxSrc.TabIndex = 0;
            this.textBoxSrc.Text = "<map id=\"200109\" road=\"200109.road.xml\">\r\n<map id=\"200101\" road=\"200101.road.xml\"" +
    ">";
            // 
            // buttonReplace
            // 
            this.buttonReplace.Location = new System.Drawing.Point(12, 404);
            this.buttonReplace.Name = "buttonReplace";
            this.buttonReplace.Size = new System.Drawing.Size(920, 51);
            this.buttonReplace.TabIndex = 1;
            this.buttonReplace.Text = "替换";
            this.buttonReplace.UseVisualStyleBackColor = true;
            this.buttonReplace.Click += new System.EventHandler(this.buttonReplace_Click);
            // 
            // textBoxDst
            // 
            this.textBoxDst.Location = new System.Drawing.Point(12, 462);
            this.textBoxDst.MaxLength = 0;
            this.textBoxDst.Multiline = true;
            this.textBoxDst.Name = "textBoxDst";
            this.textBoxDst.Size = new System.Drawing.Size(920, 368);
            this.textBoxDst.TabIndex = 2;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(13, 341);
            this.textBox2.Name = "textBox2";
            this.textBox2.ReadOnly = true;
            this.textBox2.Size = new System.Drawing.Size(79, 25);
            this.textBox2.TabIndex = 7;
            this.textBox2.Text = "起始字符:";
            // 
            // textBoxStart
            // 
            this.textBoxStart.Location = new System.Drawing.Point(98, 341);
            this.textBoxStart.Name = "textBoxStart";
            this.textBoxStart.Size = new System.Drawing.Size(340, 25);
            this.textBoxStart.TabIndex = 6;
            this.textBoxStart.Text = " road=\"";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(507, 341);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(79, 25);
            this.textBox1.TabIndex = 9;
            this.textBox1.Text = "结束字符:";
            // 
            // textBoxEnd
            // 
            this.textBoxEnd.Location = new System.Drawing.Point(592, 341);
            this.textBoxEnd.Name = "textBoxEnd";
            this.textBoxEnd.Size = new System.Drawing.Size(340, 25);
            this.textBoxEnd.TabIndex = 8;
            this.textBoxEnd.Text = "road.xml\"";
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(13, 372);
            this.textBox4.Name = "textBox4";
            this.textBox4.ReadOnly = true;
            this.textBox4.Size = new System.Drawing.Size(79, 25);
            this.textBox4.TabIndex = 11;
            this.textBox4.Text = "替换字符:";
            // 
            // textBoxReplace
            // 
            this.textBoxReplace.Location = new System.Drawing.Point(98, 372);
            this.textBoxReplace.Name = "textBoxReplace";
            this.textBoxReplace.Size = new System.Drawing.Size(340, 25);
            this.textBoxReplace.TabIndex = 10;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(944, 842);
            this.Controls.Add(this.textBox4);
            this.Controls.Add(this.textBoxReplace);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.textBoxEnd);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.textBoxStart);
            this.Controls.Add(this.textBoxDst);
            this.Controls.Add(this.buttonReplace);
            this.Controls.Add(this.textBoxSrc);
            this.Name = "Form1";
            this.Text = "替换";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxSrc;
        private System.Windows.Forms.Button buttonReplace;
        private System.Windows.Forms.TextBox textBoxDst;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox textBoxStart;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBoxEnd;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TextBox textBoxReplace;
    }
}

