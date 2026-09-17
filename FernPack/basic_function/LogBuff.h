/*
这里写明使用该日志处理系统的使用方法
该日志文件仅需一次include即可使用
在include前加上 LOG_BUFF_SIMPLE_USAGE 的宏定义使得使用便捷
内置的不同语言版本通过 LOG_BUFF_SETTINGS_LANGUAGE 宏定义选择。
支持语言：
Chinese_Complicated	==	简体文言文
(default)			==	纯英文

日志等级分为 FATAL、ERR、WARN、INFO、DEBUG、APPEND 六级，分别对应 崩、谬、诫、禀、校、登 六个汉字，严重程度从严重到不严重。
*/
/*
日志API须在高级保存选项中选择“编码=简体中文（GB2312）-代码页（936）”之后方可使用

日志API以及内联写法：
LOG_APPEND(msg)		Fern::LogBuff::onlyLogInstance().log(Fern::Level::APPEND, msg, __FILE__, __LINE__, __FUNCTION__)	//APPEND级别日志
LOG_DEBUG(msg)		Fern::LogBuff::onlyLogInstance().log(Fern::Level::DEBUG , msg, __FILE__, __LINE__, __FUNCTION__)	//DEBUG 级别日志
LOG_INFO(msg)		Fern::LogBuff::onlyLogInstance().log(Fern::Level::INFO  , msg, __FILE__, __LINE__, __FUNCTION__)	//INFO  级别日志
LOG_WARN(msg)		Fern::LogBuff::onlyLogInstance().log(Fern::Level::WARN  , msg, __FILE__, __LINE__, __FUNCTION__)	//WARN  级别日志
LOG_ERROR(msg)		Fern::LogBuff::onlyLogInstance().log(Fern::Level::ERR   , msg, __FILE__, __LINE__, __FUNCTION__)	//ERROR 级别日志
LOG_FATAL(msg)		Fern::LogBuff::onlyLogInstance().log(Fern::Level::FATAL , msg, __FILE__, __LINE__, __FUNCTION__)	//FATAL 级别日志

日志功能实现对应外部API

D
单例类获取			LogBuff& onlyLogInstance();

H
后台线程启动		start(const std::string& logFile, size_t maxSizeMB, int maxBackupCount);	
后台线程停止		stop();

R
日志记录接口		log(Level level, const std::string& message, const std::string& file, int line, const std::string& func);

S
设置最低日志等级	setLogLevel(Level level)
设置是否控制台输出日志	setConsoleOutput(bool enable);
设置日志彩色输出	setColorOutput(bool enable)		

*/






#pragma once



#define LOG_BUFF

//默认设置
#ifndef LOG_BUFF_SETTINGS_LANGUAGE
	#define LOG_BUFF_SETTINGS_LANGUAGE						1//Chinese_Complicaed
#endif

#ifndef LOG_BUFF_SETTINGS_MAX_FILE_SIZE_MB
	#define LOG_BUFF_SETTINGS_MAX_FILE_SIZE_MB				10
#endif

#ifndef LOG_BUFF_SETTINGS_MAX_BACKUP_COUNT
	#define LOG_BUFF_SETTINGS_MAX_BACKUP_COUNT				5
#endif

#ifndef LOG_BUFF_SETTINGS_CONSOLE_COLORED_OUTPUT
	#define LOG_BUFF_SETTINGS_CONSOLE_COLORED_OUTPUT		true
#endif

#ifndef LOG_BUFF_SETTINGS_CONSOLE_OUTPUT
	#define LOG_BUFF_SETTINGS_CONSOLE_OUTPUT				true
#endif

#ifndef LOG_BUFF_SETTINGS_SIMPLE_USAGE
	#define LOG_BUFF_SETTINGS_SIMPLE_USAGE					true
#endif

#ifndef LOG_BUFF_SETTINGS_MINIMUM_LEVEL
	#define LOG_BUFF_SETTINGS_MINIMUM_LEVEL					Fern::Level::DEBUG
#endif


#ifdef LOG_BUFF_SETTINGS_SIMPLE_USAGE
#if	   LOG_BUFF_SETTINGS_SIMPLE_USAGE == true
	#define LOG_APPEND(msg)		Fern::LogBuff::onlyLogInstance().log(Fern::Level::APPEND, msg, __FILE__, __LINE__, __FUNCTION__)	//APPEND级别日志
	#define LOG_DEBUG(msg)		Fern::LogBuff::onlyLogInstance().log(Fern::Level::DEBUG , msg, __FILE__, __LINE__, __FUNCTION__)	//DEBUG 级别日志
	#define LOG_INFO(msg)		Fern::LogBuff::onlyLogInstance().log(Fern::Level::INFO  , msg, __FILE__, __LINE__, __FUNCTION__)	//INFO  级别日志
	#define LOG_WARN(msg)		Fern::LogBuff::onlyLogInstance().log(Fern::Level::WARN  , msg, __FILE__, __LINE__, __FUNCTION__)	//WARN  级别日志
	#define LOG_ERROR(msg)		Fern::LogBuff::onlyLogInstance().log(Fern::Level::ERR   , msg, __FILE__, __LINE__, __FUNCTION__)	//ERROR 级别日志
	#define LOG_FATAL(msg)		Fern::LogBuff::onlyLogInstance().log(Fern::Level::FATAL , msg, __FILE__, __LINE__, __FUNCTION__)	//FATAL 级别日志
	#define LOG_INSTANCE		Fern::LogBuff::onlyLogInstance()																	//便捷调用日志单例类
#endif
#endif



#ifdef LOG_BUFF_SETTINGS_LANGUAGE
	#if LOG_BUFF_SETTINGS_LANGUAGE == 1
		#define LOG_OUTPUT_INSTANCE_EXISTED		std::cerr << "[太史公曰]既存一史官也。" << std::endl;
		#define LOG_OUTPUT_FILE_OPEN_FAILED		std::cerr << "[太史公曰]开" << logFile << "卷不成" << std::endl;
		#define LOG_OUTPUT_FILE_OPEN_SUCCESS	std::cout << "[太史公曰]" << logFile << "卷既开" << std::endl;
		#define LOG_OUTPUT_READY_TO_LOG			std::cout << "[太史公曰]史官磨墨，将记史于 " << logFile << std::endl;
		#define LOG_OUTPUT_END_OF_LOG			std::cout << "[太史公曰]记史毕合卷封漆以期流芳百世。" << std::endl;
		#define LOG_OUTPUT_SYSTEM_RUN_FAILED	std::cout << "[临危受命]" << message << std::endl;
		#define LOG_OUTPUT_WORKER_THREAD_START	std::cout << "[太史公曰]史官执笔记所知于卷宗(threadID: " << std::this_thread::get_id() << ")" << std::endl;
		#define LOG_OUTPUT_WORKER_THREAD_END	std::cout << "[太史公曰]史官搁笔" << std::endl;
		#define LOG_OUTPUT_MAX_LOGFILE_DATA		std::cout << "[太史公曰]最大卷宗 " << maxSizeMB << "MB，存 " << maxBackupCount << " 卷备份" << std::endl;
		#define LOG_OUTPUT_ROTATE_FAILED		std::cerr << "[太史公曰]滚卷失败！" << std::endl;
		#define LOG_OUTPUT_COLORED				std::cout << "[太史公曰]彩色史书" << (colorSupported_ ? "已开启" : "不支持") << std::endl;
	#else
		#define LOG_OUTPUT_INSTANCE_EXISTED		std::cerr << "[AsyncLogger] A logger has existed." << std::endl;
		#define LOG_OUTPUT_FILE_OPEN_FAILED		std::cerr << "[AsyncLogger] Failed when opening file:" << logFile << std::endl;
		#define LOG_OUTPUT_FILE_OPEN_SUCCESS	std::cout << "[AsyncLogger] " << logFile << " was successfully opened." << std::endl;
		#define LOG_OUTPUT_READY_TO_LOG			std::cout << "[AsyncLogger] Logger ready.Logging at file:" << logFile << std::endl;
		#define LOG_OUTPUT_END_OF_LOG			std::cout << "[AsyncLogger] Log ended." << std::endl;
		#define LOG_OUTPUT_SYSTEM_RUN_FAILED	std::cout << "[LoggerRunFailed] " << message << std::endl;
		#define LOG_OUTPUT_WORKER_THREAD_START	std::cout << "[AsyncLogger] Worker thread started (ID:" << std::this_thread::get_id() << ")." << std::endl;
		#define LOG_OUTPUT_WORKER_THREAD_END	std::cout << "[AsynkLogger] Worker thread joined." << std::endl;
		#define LOG_OUTPUT_MAX_LOGFILE_DATA		std::cout << "[AcyncLogger] Max log file size " << maxSizeMB << "MB," << maxBackupCount << " backups stored." << std::endl;
		#define LOG_OUTPUT_ROTATE_FAILED		std::cerr << "[AsyncLogger] Failed to rotate log files." << std::endl;
		#define LOG_OUTPUT_COLORED				std::cout << "[AsyncLogger] Colored console output:" << (colorSupported_ ? "on" : "not enabled") << std::endl;
	#endif
#else
		#define LOG_OUTPUT_INSTANCE_EXISTED		std::cerr << "[AsyncLogger] A logger has existed." << std::endl;
		#define LOG_OUTPUT_FILE_OPEN_FAILED		std::cerr << "[AsyncLogger] Failed when opening file:" << logFile << std::endl;
		#define LOG_OUTPUT_FILE_OPEN_SUCCESS	std::cout << "[AsyncLogger] " << logFile << " was successfully opened." << std::endl;
		#define LOG_OUTPUT_READY_TO_LOG			std::cout << "[AsyncLogger] Logger ready.Logging at file:" << logFile << std::endl;
		#define LOG_OUTPUT_END_OF_LOG			std::cout << "[AsyncLogger] Log ended." << std::endl;
		#define LOG_OUTPUT_SYSTEM_RUN_FAILED	std::cout << "[LoggerRunFailed] " << message << std::endl;
		#define LOG_OUTPUT_WORKER_THREAD_START	std::cout << "[AsyncLogger] Worker thread started (ID:" << std::this_thread::get_id() << ")." << std::endl;
		#define LOG_OUTPUT_WORKER_THREAD_END	std::cout << "[AsynkLogger] Worker thread joined." << std::endl;
		#define LOG_OUTPUT_MAX_LOGFILE_DATA		std::cout << "[AcyncLogger] Max log file size " << maxSizeMB << "MB," << maxBackupCount << " backups stored." << std::endl;
		#define LOG_OUTPUT_COLORED				std::cout << "[AsyncLogger] Colored console output:" << (colorSupported_ ? "on" : "not enabled") << std::endl;
#endif


#define COLOR_RESET   "\033[0m"



//引擎名称源于大喷菇菲伦。我说她是十七岁的妈妈有没有懂的
namespace Fern {
	
	enum Level { FATAL = 1, ERR, WARN, INFO, DEBUG, APPEND };

	struct LogMessage {										// 已经格式化好的完整日志内容
		Level level;										// 日志等级
		std::string content;								// 日志文本      
		std::chrono::system_clock::time_point timestamp;	// 时间
		std::string fileName;								// 文件名
		int lineNumber;										// 行号
		std::string functionName;							// 函数名

		LogMessage(Level lvl, const std::string& msg, const std::string& file, int line, const std::string& func) :
			level(lvl),
			content(msg),
			timestamp(std::chrono::system_clock::now()),
			fileName(file),
			lineNumber(line),
			functionName(func) {}
	};

	typedef class LogBuff {
	public:
		static LogBuff& onlyLogInstance() {																				//获取单例类
			static LogBuff inst;
			return inst;
		}
		void start(const std::string& logFile, size_t maxSizeMB, int maxBackupCount);									//启动后台线程
		void stop();																									//停止后台线程
		void log(Level level, const std::string& message, const std::string& file, int line, const std::string& func);	//日志接口调用
		void setLogLevel(Level level);																					//设置最低日志等级
		void setConsoleOutput(bool enable);																				//设置是否控制台输出日志
		void setColorOutput(bool enable);																				//设置日志彩色输出

	private:
		std::queue<LogMessage> messageQueue_;		// 消息队列
		std::mutex queueMutex_;						// 队列互斥锁
		std::condition_variable condition_;			// 条件变量（唤醒后台线程）
		std::atomic<bool> stopFlag_{ false };       // 停止标志（原子变量）
		std::atomic<bool> running_{ false };        // 是否正在运行
		std::thread workerThread_;					// 后台线程对象
		std::ofstream logFile_;						// 日志文件流
		Level minLevel_{ Level::DEBUG };			// 最低日志级别
		bool consoleOutput_{ true };				// 是否输出到控制台
		bool initialized_{ false };					// 是否已初始化

		LogBuff() {
			enableConsoleColors();		//构造时检测并启用彩色
			checkSettings();
		}
		~LogBuff() { stop(); }


		void workerThread();															// 日志线程主循环
		void writeLog(const LogMessage& msg);											// 实际写入日志
		std::string getTimeString(const std::chrono::system_clock::time_point& tp);		// 获取当前时间字符串
		std::string levelToString(Level level) const;									// 级别转字符串
		void checkSettings();															// 检查初始设置

		//日志滚动相关
		void rotateLog();												//日志滚动
		size_t getFileSize();											//获取文件大小
		std::string getBackupFileName(int index) const;					//获取备份文件名
		std::string extractFileName(const std::string& fullPath) const;	//提取文件名
		size_t maxFileSize_{ 0 };										// 最大文件大小（字节）
		std::string currentLogFile_;									// 当前日志文件名
		int maxBackupCount_{ 0 };										// 最大备份数量


		//颜色相关
		void enableConsoleColors();														//设置是否启用彩色
		const char* getLevelColor(Level level) const;									//获取等级对应彩色
		bool isColorSupported() const { return colorSupported_; }						//是否支持彩色
		bool colorSupported_{ false };													//变量是否支持彩色
		bool colorOutput_{ true };														//默认开启彩色输出

	};

	void LogBuff::checkSettings() {
		consoleOutput_ = LOG_BUFF_SETTINGS_CONSOLE_OUTPUT;
		colorOutput_ = LOG_BUFF_SETTINGS_CONSOLE_COLORED_OUTPUT;
		minLevel_ = LOG_BUFF_SETTINGS_MINIMUM_LEVEL;
		return;
	}

	void LogBuff::start(const std::string& logFile, size_t maxSizeMB = LOG_BUFF_SETTINGS_MAX_FILE_SIZE_MB, int maxBackupCount = LOG_BUFF_SETTINGS_MAX_BACKUP_COUNT) {
		std::lock_guard<std::mutex> lock(queueMutex_);

		if (running_) {
			LOG_OUTPUT_INSTANCE_EXISTED
				return;
		}

		// 保存配置
		currentLogFile_ = logFile;
		maxFileSize_ = maxSizeMB * 1024 * 1024;  // 将最大文件大小（MB）转换为字节
		maxBackupCount_ = maxBackupCount;


		// 打开日志文件
		logFile_.open(logFile, std::ios::out | std::ios::app);
		if (!logFile_.is_open()) {
			LOG_OUTPUT_FILE_OPEN_FAILED
				// 即使文件打开失败，仍然继续运行（只输出到控制台）
		}
		else {
			LOG_OUTPUT_FILE_OPEN_SUCCESS
			LOG_OUTPUT_MAX_LOGFILE_DATA
			LOG_OUTPUT_COLORED
		}

		// 重置停止标志
		stopFlag_ = false;
		running_ = true;

		// 创建后台线程
		workerThread_ = std::thread(&LogBuff::workerThread, this);

		LOG_OUTPUT_READY_TO_LOG
	}

	void LogBuff::stop() {
		if (!running_) {
			return;
		}

		// 1. 设置停止标志
		stopFlag_ = true;

		// 2. 唤醒后台线程（让它检查 stopFlag_）
		condition_.notify_one();

		// 3. 等待线程真正结束
		if (workerThread_.joinable()) {
			workerThread_.join();
		}

		// 4. 关闭文件
		if (logFile_.is_open()) {
			logFile_.flush();
			logFile_.close();
		}

		running_ = false;
		LOG_OUTPUT_END_OF_LOG
	}

	void LogBuff::log(Level level, const std::string& message, const std::string& file, int line, const std::string& func) {
		// 1. 丢弃被过滤的等级日志
		if (static_cast<int>(level) > static_cast<int>(minLevel_)) {
			return;
		}

		// 2. 如果日志系统未运行，直接输出到控制台（降级方案）
		if (!running_) {
			if (consoleOutput_) {
				LOG_OUTPUT_SYSTEM_RUN_FAILED
			}
			return;
		}

		// 3. 构造消息对象（含时间戳）
		LogMessage msg(level, message, file, line, func);

		// 4. 入队（加锁保护）
		{
			std::lock_guard<std::mutex> lock(queueMutex_);
			messageQueue_.push(std::move(msg));
		}

		// 5. 唤醒后台线程（通知有新消息）
		condition_.notify_one();
	}

	void LogBuff::workerThread() {
		LOG_OUTPUT_WORKER_THREAD_START

		while (true) {
			LogMessage msg(Level::INFO, "", "", 0, "");

			// ---- 从队列取消息（加锁） ----
			{
				std::unique_lock<std::mutex> lock(queueMutex_);

				// 等待条件：有消息 或 需要停止
				condition_.wait(lock, [this]() {
					return !messageQueue_.empty() || stopFlag_.load();
					});

				// 如果队列为空且需要停止，退出循环
				if (messageQueue_.empty() && stopFlag_) {
					break;
				}

				// 取出消息（FIFO）
				if (!messageQueue_.empty()) {
					msg = std::move(messageQueue_.front());
					messageQueue_.pop();
				}
			}  // 自动释放锁

			// ---- 真正写入日志（在锁外执行，减少锁持有时间） ----
			if (!msg.content.empty()) {
					writeLog(msg);
			}
		}

		// ---- 退出前，处理队列中剩余的消息（确保不丢失） ----
		std::lock_guard<std::mutex> lock(queueMutex_);
		while (!messageQueue_.empty()) {
			LogMessage remaining = std::move(messageQueue_.front());
			messageQueue_.pop();
			writeLog(remaining);  // 直接写入
		}

		LOG_OUTPUT_WORKER_THREAD_END
	}

	void LogBuff::writeLog(const LogMessage& msg) {
		std::string timeStr = getTimeString(msg.timestamp);
		std::string levelStr = levelToString(msg.level);

		//// 格式化完整日志行
		//std::ostringstream oss;
		//oss << timeStr << " [" << levelStr << "] " << msg.content;
		//std::string line = oss.str();

		// 提取文件名（去掉路径，只保留文件名）
		std::string fileName = msg.fileName;
		size_t lastSlash = fileName.find_last_of("/\\");
		if (lastSlash != std::string::npos) {
			fileName = fileName.substr(lastSlash + 1);
		}

		// 格式化日志：时间 [级别] 文件名:行号 (函数名) -> 消息
		std::ostringstream oss;
		oss << timeStr
			<< " [" << levelStr << "] "
			<< msg.content
			<< "\t<" << fileName << ">:" << msg.lineNumber << " (" << msg.functionName << ")";
		std::string line = oss.str();


		// 输出到控制台
		if (consoleOutput_) {
			const char* color = getLevelColor(msg.level);
			if (msg.level <= Level::ERR) {
				// 错误输出到 cerr（也带颜色）
				std::cerr << color << line << COLOR_RESET << std::endl;
			}
			else {
				std::cout << color << line << COLOR_RESET << std::endl;
			}

			/*
			if (msg.level <= Level::ERR) {
				std::cerr << line << std::endl;  // 错误用 cerr
			}
			else {
				std::cout << line << std::endl;
			}
			*/
		}

		// 输出到文件
		if (logFile_.is_open()) {
			logFile_ << line << std::endl;

			// ERR 及以上级别立即刷新
			if (msg.level <= Level::ERR) {
				logFile_.flush();
			}
		}

		if (getFileSize() > maxFileSize_) {
			rotateLog();
		}
	}

	void LogBuff::setLogLevel(Level level) {
		minLevel_ = level;
	}

	void LogBuff::setConsoleOutput(bool enable) {
		consoleOutput_ = enable;
	}

	inline std::string LogBuff::getTimeString(const std::chrono::system_clock::time_point& tp) {
		auto time_t_now = std::chrono::system_clock::to_time_t(tp);
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			tp.time_since_epoch()) % 1000;

		std::tm tm_now;
	#ifdef _WIN32
		localtime_s(&tm_now, &time_t_now);
	#else
		localtime_r(&time_t_now, &tm_now);
	#endif

		std::ostringstream oss;
		oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S")
			<< "." << std::setfill('0') << std::setw(3) << ms.count();
		return oss.str();
	}

	inline std::string LogBuff::levelToString(Level level) const {
		switch (level) {

#ifdef LOG_BUFF_SETTINGS_LANGUAGE

#if LOG_BUFF_SETTINGS_LANGUAGE == 1
		case Level::DEBUG:	return "校";
		case Level::INFO:	return "禀";
		case Level::WARN:	return "诫";
		case Level::ERR:	return "谬";
		case Level::FATAL:	return "崩";
		case Level::APPEND:	return "登";
		default:			return "诡";
#else
		case Level::DEBUG:	return "DEBUG ";
		case Level::INFO:	return "INFO  ";
		case Level::WARN:	return "WARN  ";
		case Level::ERR:	return "ERROR ";
		case Level::FATAL:	return "FATAL ";
		case Level::APPEND:	return "APPEND";
		default:			return "UNKOWN";
#endif

#else
		case Level::DEBUG:	return "DEBUG ";
		case Level::INFO:	return "INFO  ";
		case Level::WARN:	return "WARN  ";
		case Level::ERR:	return "ERROR ";
		case Level::FATAL:	return "FATAL ";
		case Level::APPEND:	return "APPEND";
		default:			return "UNKOWN";
#endif
		}
	}


	inline void LogBuff::rotateLog() {
		// 1. 关闭当前文件
		if (logFile_.is_open()) {
			logFile_.close();
		}

		// 2. 删除最旧的备份文件
		std::string oldestBackup = getBackupFileName(maxBackupCount_);
		if (std::filesystem::exists(oldestBackup)) {
			std::filesystem::remove(oldestBackup);
		}

		// 3. 依次重命名：app.4.log → app.5.log ... app.1.log → app.2.log
		for (int i = maxBackupCount_ - 1; i >= 1; --i) {
			std::string from = getBackupFileName(i);
			std::string to = getBackupFileName(i + 1);
			if (std::filesystem::exists(from)) {
				std::filesystem::rename(from, to);
			}
		}

		// 4. 重命名当前文件 → app.1.log
		if (std::filesystem::exists(currentLogFile_)) {
			std::string backup1 = getBackupFileName(1);
			std::filesystem::rename(currentLogFile_, backup1);
		}

		// 5. 重新打开新文件
		logFile_.open(currentLogFile_, std::ios::out | std::ios::app);
		if (!logFile_.is_open()) {
			LOG_OUTPUT_ROTATE_FAILED
		}
	}

	inline size_t LogBuff::getFileSize() {
		if (!logFile_.is_open()) { return 0; }
		logFile_.flush();  // 确保所有数据已写入
		return static_cast<size_t>(std::filesystem::file_size(currentLogFile_));

	}

	inline std::string LogBuff::getBackupFileName(int index) const {
		// 在文件名中插入 .N
		// app.log → app.1.log
		size_t dotPos = currentLogFile_.find_last_of('.');
		if (dotPos == std::string::npos) {
			return currentLogFile_ + "." + std::to_string(index);
		}
		return currentLogFile_.substr(0, dotPos) + "." + std::to_string(index) + currentLogFile_.substr(dotPos);
	}

	inline std::string LogBuff::extractFileName(const std::string& fullPath) const {
		size_t lastSlash = fullPath.find_last_of("/\\");
		if (lastSlash != std::string::npos) {
			return fullPath.substr(lastSlash + 1);
		}
		return fullPath;
	}



	inline void LogBuff::enableConsoleColors() {
		colorSupported_ = false;

#ifdef _WIN32
		// Windows: 启用虚拟终端处理（需要 Windows 10+）
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hConsole != INVALID_HANDLE_VALUE) {
			DWORD mode = 0;
			if (GetConsoleMode(hConsole, &mode)) {
				// 尝试启用 ANSI 转义码支持
				mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
				if (SetConsoleMode(hConsole, mode)) {
					colorSupported_ = true;
				}
			}
		}
#else
		// Linux/macOS: 检测是否为终端
		if (isatty(STDOUT_FILENO)) {
			colorSupported_ = true;
		}
#endif
	}

	inline const char* LogBuff::getLevelColor(Level level) const {
		if (!colorSupported_ || !colorOutput_) {
			return "";
		}

		switch (level) {
		case Level::FATAL:		return "\033[31;1m";  // 红色 + 粗体
		case Level::ERR:		return "\033[31m";    // 红色
		case Level::WARN:		return "\033[33m";    // 黄色
		case Level::INFO:		return "\033[32m";    // 绿色
		case Level::DEBUG:		return "\033[90m";    // 灰色
		case Level::APPEND:		return "\033[95m";    // 紫色
		default:				return "\033[0m";     // 重置
		}
	}

	void LogBuff::setColorOutput(bool enable) {
		colorOutput_ = enable;
	}

}



	

	

	

	

	

	

	