
namespace FormatReplaceTxt
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.textBoxSrc = new System.Windows.Forms.TextBox();
            this.buttonFormat = new System.Windows.Forms.Button();
            this.textBoxDst = new System.Windows.Forms.TextBox();
            this.textBoxRule = new System.Windows.Forms.TextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBoxDiscardFrom = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // textBoxSrc
            // 
            this.textBoxSrc.Location = new System.Drawing.Point(12, 12);
            this.textBoxSrc.Multiline = true;
            this.textBoxSrc.Name = "textBoxSrc";
            this.textBoxSrc.Size = new System.Drawing.Size(920, 323);
            this.textBoxSrc.TabIndex = 0;
            this.textBoxSrc.Text = resources.GetString("textBoxSrc.Text");
            // 
            // buttonFormat
            // 
            this.buttonFormat.Location = new System.Drawing.Point(12, 405);
            this.buttonFormat.Name = "buttonFormat";
            this.buttonFormat.Size = new System.Drawing.Size(920, 51);
            this.buttonFormat.TabIndex = 1;
            this.buttonFormat.Text = "格式化";
            this.buttonFormat.UseVisualStyleBackColor = true;
            this.buttonFormat.Click += new System.EventHandler(this.buttonFormat_Click);
            // 
            // textBoxDst
            // 
            this.textBoxDst.Location = new System.Drawing.Point(12, 462);
            this.textBoxDst.Multiline = true;
            this.textBoxDst.Name = "textBoxDst";
            this.textBoxDst.Size = new System.Drawing.Size(920, 368);
            this.textBoxDst.TabIndex = 2;
            // 
            // textBoxRule
            // 
            this.textBoxRule.Location = new System.Drawing.Point(81, 341);
            this.textBoxRule.Name = "textBoxRule";
            this.textBoxRule.Size = new System.Drawing.Size(698, 25);
            this.textBoxRule.TabIndex = 3;
            this.textBoxRule.Text = ".addConstant<std::size_t>(\"{0}\",FMOD_RESULT::{0})";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(13, 341);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(62, 25);
            this.textBox1.TabIndex = 5;
            this.textBox1.Text = "格式->";
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(13, 372);
            this.textBox2.Name = "textBox2";
            this.textBox2.ReadOnly = true;
            this.textBox2.Size = new System.Drawing.Size(204, 25);
            this.textBox2.TabIndex = 7;
            this.textBox2.Text = "从这个字符串标记开始抛弃->";
            // 
            // textBoxDiscardFrom
            // 
            this.textBoxDiscardFrom.Location = new System.Drawing.Point(223, 372);
            this.textBoxDiscardFrom.Name = "textBoxDiscardFrom";
            this.textBoxDiscardFrom.Size = new System.Drawing.Size(709, 25);
            this.textBoxDiscardFrom.TabIndex = 6;
            this.textBoxDiscardFrom.Text = "=";
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(785, 341);
            this.textBox3.Name = "textBox3";
            this.textBox3.ReadOnly = true;
            this.textBox3.Size = new System.Drawing.Size(147, 25);
            this.textBox3.TabIndex = 8;
            this.textBox3.Text = "用{0}表示填充";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(944, 842);
            this.Controls.Add(this.textBox3);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.textBoxDiscardFrom);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.textBoxRule);
            this.Controls.Add(this.textBoxDst);
            this.Controls.Add(this.buttonFormat);
            this.Controls.Add(this.textBoxSrc);
            this.Name = "Form1";
            this.Text = "批量格式化";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxSrc;
        private System.Windows.Forms.Button buttonFormat;
        private System.Windows.Forms.TextBox textBoxDst;
        private System.Windows.Forms.TextBox textBoxRule;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox textBoxDiscardFrom;
        private System.Windows.Forms.TextBox textBox3;
    }
}

