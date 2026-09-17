## LogBuff.h

这是我另一个小型库，主要用于日志输出的缓冲和管理喵。  
需要说明的是，这个库其实是另外一个独立的项目。  
由于日期太早，体量太小，所以我就把它直接放在了这个项目里喵。  
我之所以期望开发结果是可以复用的头文件库，就是为了这点饺子醋——我可以直接在其他C++项目中把项目开发结果`#include`进来就可以使用喵。

---

首先要说明的一点是，这个库在输出日志的时候，默认输出中文。所以请将代码保存为代码页936（GBK）格式喵。  
我在两个月以前就因为这样的 BUG 而炸毛了好几次喵。生成输出提示字符串字面量无法解析，导致生成失败。  
出现这种情况请检查高级保存选项，确保保存为代码页936（GBK）格式喵。

## 外部依赖说明
就开发过程而言，此库需要以下标准库依赖：
-  fstream  
-  queue  
-  string  
-  thread  
-  mutex  
-  condition_variavle  
-  atomic  
-  chrono  
-  iostream
-  iomanip  
-  filesystem
-  如果为Windows平台，则有 Windows  

以上依赖存在有注释以后不影响编译及正常运行的情况。  
开发时存在不同方案的比选。这可能导致有一些库在此处没有引用指向。  



## 日志级别说明

|        代码中的写法   | 枚举常数 | 说明                             | 中文简写 | 英文简写|
| :-------------------: | ---: | ------------------------------------ | :--------:|: ---:|
| `Fern::Level::FATAL`  |    1 |  致命错误，程序无法继续运行			| 崩    |  FATAL |
|  `Fern::Level::ERR`   |    2 |  错误，程序可以继续运行但存在异常		| 谬    |  ERROR |
|  `Fern::Level::WARN`  |    3 |  警告，程序可以继续运行但需要注意		| 诫    |  WARN  |
|  `Fern::Level::INFO`  |    4 |  信息，程序运行的一般性信息			| 禀    |  INFO  |
| `Fern::Level::DEBUG`  |    5 |  调试，详细的程序运行信息				| 校    |  DEBUG |
| `Fern::Level::APPEND` |    6 |  追加，程序运行的附加信息				| 登    | APPEND |



## 宏

`LOG_BUFF_SETTINGS_LANGUAGE`  
-  语言设置宏，默认值为`1`，表示输出中文。  
-  如果宏被设置为其他值或未指定，则输出英文。 

`LOG_BUFF_SETTINGS_MAX_FILE_SIZE_MB`  
-  日志文件最大大小设置宏，默认值为`10`，表示日志文件最大为 10 MB 。
-  日志滚动的时候会校验大小，如果超过这个值，则会删除最早的日志文件，保证每一个日志文件的大小不超过这个值。

`LOG_BUFF_SETTINGS_MAX_BACKUP_COUNT`  
-  日志文件最大备份数量设置宏，默认值为`5`，表示最多保留 5 个日志文件。

`LOG_BUFF_SETTINGS_CONSOLE_COLORED_OUTPUT`  
-  控制台彩色输出设置宏，默认值为`true`，表示启用彩色输出。

`LOG_BUFF_SETTINGS_CONSOLE_OUTPUT`  
-  控制台输出设置宏，默认值为`true`，表示启用控制台输出。

`LOG_BUFF_SETTINGS_SIMPLE_USAGE`  
-  简单使用设置宏，默认值为`true`，表示启用简单使用模式。
 
`LOG_BUFF_SETTINGS_MINIMUM_LEVEL`  
-  日志输出最小等级设置宏，默认值为`Fern::Level::DEBUG`，日志级别低于该等级的日志将被忽略。


# API

| 作用 | 写法 |
|---  | --- |
|单例类获取		|	LogBuff& onlyLogInstance();  |
|后台线程启动	|	start(const std::string& logFile, size_t maxSizeMB, int maxBackupCount);	  |
|后台线程停止	|	stop();  |
|日志记录接口	|	log(Level level, const std::string& message, const std::string& file, int line, const std::string& func);  |
|设置最低日志等级|	setLogLevel(Level level)  |
|设置是否控制台输出日志|	setConsoleOutput(bool enable)  |
|设置日志彩色输出	|setColorOutput(bool enable)		  |


`LogBuff& onlyLogInstance();`  
- 
- 参数  
    - 无  
- 返回值  
    - 每一个程序只能生成一次的LogBuff实例
- 注
    - 可以用宏 `LOG_INSTANCE` 完成这个函数的调用。可以将宏展开内联，展开内联结果与该函数写法一致。






注：在简单使用模式下，便捷的日志输出函数宏及获取日志单例类宏为：

| 函数宏 | 作用 | 
|: ---: |: --- :|
|`LOG_APPEND(msg)`	|输出级别为“登”的日志   |
|`LOG_DEBUG(msg)`	|输出级别为“校”的日志	|
|`LOG_INFO(msg)`	|输出级别为“禀”的日志	|
|`LOG_WARN(msg)`	|输出级别为“诫”的日志	|
|`LOG_ERROR(msg)`	|输出级别为“谬”的日志	|
|`LOG_FATAL(msg)`	|输出级别为“崩”的日志	|
|`LOG_INSTANCE`		|获取日志单例实例		|

推荐开启简单使用模式即保持默认。





