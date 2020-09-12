using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


public class JsonConfigParser<T> where T : class, new()
{
    public string mJsonConfigPath;

    public T instance;

    public JsonConfigParser()
    {
        
    }

    /// <summary>
    /// 加载json文件
    /// </summary>
    /// <param name="configFilePath"></param>
    /// <returns></returns>
    public T Load(string configFilePath)
    {
        mJsonConfigPath = configFilePath;

        instance = new T();

        if (File.Exists(mJsonConfigPath))
        {
            //读取配置文件
            string jsonStr = File.ReadAllText(mJsonConfigPath);
            instance= JsonConvert.DeserializeObject<T>(jsonStr);
        }
        else
        {
            Save();
        }

        return instance;
    }

    /// <summary>
    /// 保存
    /// </summary>
    public void Save()
    {
        //保存配置文件
        string jsonStr = JsonConvert.SerializeObject(instance);
        File.WriteAllText(mJsonConfigPath, jsonStr);
    }
}

